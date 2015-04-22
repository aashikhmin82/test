#include <fstream>
#include <iostream>

#include "bin_tree.h"
#include "dot.h"

using namespace dot;
using namespace std;

namespace dot
{
    ostream& operator<<(ostream& out,
                        nodeid_t node)
    {
        out << '"' << node.node;
        if (node.parent)
        {
            out << ',';
            if (node.which_child == left_edge)
                out << "left";
            else
                out << "right";
            out << "_child_of_" << node.parent;
        }
        out << '"';

        return out;
    }



    namespace
    {
        bool accessed(const color_t& color)
        {
            return color == visited or color == not_found or color == added;
        }
    }



    color_t highlight_t::node(nodeid_t node,
                              highlight_t* next_highlight) const
    {
        const auto previous_count = node_counters.find(node);
        size_t current_count = node.node ? node.node->element_counter() : 0;
        color_t result = unmarked;
        bool new_node = previous_count == node_counters.end();
        bool edges_visited = false;

        if (next_highlight)
        {
            next_highlight->node(node, in_prev);
            next_highlight->node_counters[node] = current_count;
        }

        if (node.node)
        {
            color_t l = edge(node.node, left_edge, nullptr);
            color_t r = edge(node.node, right_edge, nullptr);
            edges_visited = accessed(l) or accessed(r);
        }
        else if (node.parent and edge(node.parent, node.which_child, nullptr) == not_found)
            result = not_found;

        if (previous_count->second < current_count)
            result = edges_visited ? visited : found;

        if (new_node)
            result = node_counters.empty() ? unmarked : added;

        const auto i = nodes.find(node);
        if (i != nodes.end() and (i->second != in_prev or result == unmarked))
            return i->second;

        return result;
    }

    color_t highlight_t::edge(node_t from,
                              edge_t direction,
                              highlight_t* next_highlight) const
    {
        const auto edge_id = edgeid_t(from, direction);
        const auto previous_count = edge_counters.find(edge_id);
        size_t current_count = direction == left_edge ? from->left_counter() : from->right_counter();
        color_t result = unmarked;
        bool new_edge = previous_count == edge_counters.end();

        if (next_highlight)
        {
            next_highlight->edge(from, direction, in_prev);
            next_highlight->edge_counters[edge_id] = current_count;
        }
        else if (new_edge)
            return added;

        if (previous_count->second < current_count)
            result = edge_id.node ? visited : not_found;

        if (new_edge)
            result = edge_counters.empty() ? unmarked : added;

        const auto i = edges.find(edge_id);
        if (i != edges.end() and (i->second != in_prev or result == unmarked))
            return i->second;

        return result;
    }
}

namespace
{
    void dump_node(ostream& out,
                   nodeid_t name,
                   const Bin_Tree_Element *node,
                   size_t level,
                   const color_t& color)
    {
        out << "  " << name << " [rank = " << level;

        if (color != unmarked)
            out << ", color = \"" << color << "\", fontcolor = \"" << color << '"';

        if (node and node->colour(false) != "NON")
        {
            out << ", style = \"filled\", fillcolor = \"";
            if (node->colour(false) == "red")
                out << "red3\"";
            else if (node->colour(false) == "black")
                out << "gray90\"";
            else
                out << "violet\"";
        }

        out << ", label = \"";
        if (node)
            out << node->element(false) << "\", shape = box]\n";
        else
            out << "null\"]\n";
    }

    void dump_edge(ostream& out,
                   nodeid_t from,
                   nodeid_t to,
                   const color_t& color)
    {
        out << "  " << from << " -> " << to;
        if (color != unmarked)
            out << " [color = \"" << color << "\"]";
        out << '\n';
    }

    nodeid_t dump_node_dot(ostream& out,
                                        const Bin_Tree_Element *node,
                                        size_t level,
                                        const highlight_t& highlight,
                                        highlight_t& next_highlight)
    {
        nodeid_t node_id(node);
        dump_node(out, node_id, node, level, highlight.node(node_id, &next_highlight));

        if (node)
        {
            nodeid_t left_child;
            nodeid_t right_child;

            if (node->left(false))
                left_child = dump_node_dot(out, node->left(false), level + 1, highlight, next_highlight);
            else
            {
                left_child = nodeid_t(nullptr, node, left_edge);
                /* Это нужно чтобы узел "null" в графе был не один общий, а свой в каждом месте. */
                dump_node(out, left_child, node->left(false), level + 1, highlight.node(left_child, &next_highlight));
            }

            if (node->right(false))
                right_child = dump_node_dot(out, node->right(false), level + 1, highlight, next_highlight);
            else
            {
                right_child = nodeid_t(nullptr, node, right_edge);
                dump_node(out, right_child, node->right(false), level + 1, highlight.node(right_child, &next_highlight));
            }

            dump_edge(out, node_id, left_child, highlight.edge(node, left_edge, &next_highlight));
            dump_edge(out, node_id, right_child, highlight.edge(node, right_edge, &next_highlight));
        }

        return node_id;
    }
}

namespace dot
{
    void dump_tree(const Bin_Tree_Element *root,
                       highlight_t& highlight,
                       const string& prefix,
                       const string& label)
    {
        highlight_t next_highlight;
        /*
         * Лучше:
         * 1) set, но так запись проверки проще и естественней.
         * 2) Завести по отдельному контейнеру для узлов и рёбер, но я решил не усложнять.
         * 3) Наверное, стоит обойтись без static, принимая этот контейнер снаружи или переделав это всё в класс.
         *
         * И ещё, этот метод, в паре с идентификацией узлов по их адресам в памяти,
         * работает нестабильно при освобождении-перевыделении памяти под узлы.
         * Так как новый узел может получить адрес старого (уже напечатанного и потом удалённого).
         * Тогда он будет ошибочно напечатан серым цветом, как будто он старый.
         */

        fstream out(prefix + ".dot", fstream::out | fstream::app);
        out << "digraph {\n  label=\"" << label << "\" fontsize=32\n";
        dump_node_dot(out, root, 0, highlight, next_highlight);
        out << "}\n\n";
        out.close();
        system((string("dot -Tpdf ") + prefix + ".dot > " + prefix + ".pdf").c_str());
        /*
         * У меня под Mint'ом dot создаёт битые PDF-ки, в которых показывается только последняя страница.
         * Если у тебя те же проблемы, используй формат PostScript.
         * Его умеет показывать evince.
         */
        system((string("dot -Tps ") + prefix + ".dot > " + prefix + ".ps").c_str());
        cout << "Dumped to " << prefix << ".dot , " << prefix << ".pdf and " << prefix << ".ps .\n";
        /*
         * Выбрал PDF, потому что в нём на каждой странице будет по графу –
         * файл tree.dot каждый раз открывается на дозапись.
         * В картиночных форматах печатается только первый граф.
         * А в PDF можно смотреть как дерево меняется после проводимых над ним операций.
         */

        swap(highlight, next_highlight);
    }
}
