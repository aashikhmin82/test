#include <fstream>
#include <iostream>

#include "bin_tree.h"
#include "dot.h"

using namespace dot;
using namespace std;

namespace
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

    void dump_node(ostream& out,
                   nodeid_t name,
                   const Bin_Tree_Element *node,
                   size_t level,
                   const color_t& color)
    {
        out << "  " << name << " [rank = " << level;

        if (color != unmarked)
            out << ", color = \"" << color << "\", fontcolor = \"" << color << '"';

        out << ", label = \"";
        if (node)
            out << node->element << " (" << node->colour << ") \", shape = box]\n";
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
        dump_node(out, node_id, node, level, highlight.node(node_id));
        next_highlight.node(node_id, in_prev);

        if (node)
        {
            nodeid_t left_child;
            nodeid_t right_child;

            if (node->left)
                left_child = dump_node_dot(out, node->left, level + 1, highlight, next_highlight);
            else
            {
                left_child = nodeid_t(nullptr, node, left_edge);
                /* Это нужно чтобы узел "null" в графе был не один общий, а свой в каждом месте. */
                dump_node(out, left_child, node->left, level + 1, highlight.node(left_child));
                next_highlight.node(left_child, in_prev);
            }

            if (node->right)
                right_child = dump_node_dot(out, node->right, level + 1, highlight, next_highlight);
            else
            {
                right_child = nodeid_t(nullptr, node, right_edge);
                dump_node(out, right_child, node->right, level + 1, highlight.node(right_child));
                next_highlight.node(right_child, in_prev);
            }

            dump_edge(out, node_id, left_child, highlight.edge(node, left_edge));
            dump_edge(out, node_id, right_child, highlight.edge(node, right_edge));
            next_highlight.edge(node, left_edge, in_prev);
            next_highlight.edge(node, right_edge, in_prev);
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
