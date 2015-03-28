#pragma once

#include <map>
#include <string>

#include "bin_tree.h"

namespace dot
{
    enum edge_t
    {
        none,
        left_edge,
        right_edge
    };

    using node_t = const Bin_Tree_Element*;

    class nodeid_t
    {
    public:
        node_t node;
        node_t parent;
        edge_t which_child;

        nodeid_t(node_t node = nullptr,
                 node_t up = nullptr,
                 edge_t which_child = none) :
        node(node),
        parent(node ? node->up : up),
        which_child(which_child == none and parent ?
                    (parent->left == node ? left_edge : right_edge) : which_child)
        {
        }

        bool operator<(const nodeid_t& other) const
        {
            return
                node < other.node or
                (node == other.node and parent < other.parent) or
                (node == other.node and parent == other.parent and which_child < other.which_child);
        }
    };

    class edgeid_t : public nodeid_t
    {
    public:
        edgeid_t(node_t from,
                 edge_t direction) :
        nodeid_t(from ? (direction == left_edge ? from->left : from->right) : nullptr,
                 from,
                 direction)
        {
        }
    };



    using color_t = std::string;

    /* Список доступных цветов и цветовых схем: http://www.graphviz.org/content/color-names */
    static const color_t unmarked = "";
    static const color_t added = "green3";
    static const color_t found = "magenta3";
    static const color_t not_found = "red3";
    static const color_t visited = "blue3";
    static const color_t in_prev = "grey";



    class highlight_t
    {
    private:
        using nodes_t = std::map<nodeid_t, color_t>;
        using edges_t = std::map<edgeid_t, color_t>;

    public:
        void node(nodeid_t node,
                  const color_t& color)
        {
            nodes[node] = color;
        }

        void edge(node_t from,
                  edge_t direction,
                  const color_t& color)
        {
            edges[edgeid_t(from, direction)] = color;
        }

        void leaf(nodeid_t leaf,
                  const color_t& color)
        {
            node(leaf, color);
            edge(leaf.node, left_edge, color);
            edge(leaf.node, right_edge, color);
            node(nodeid_t(nullptr, leaf.node, left_edge), color);
            node(nodeid_t(nullptr, leaf.node, right_edge), color);
        }

        color_t node(nodeid_t node) const
        {
            const auto i = nodes.find(node);
            if (i != nodes.end())
                return i->second;
            else
                return unmarked;
        }

        color_t edge(node_t from,
                     edge_t direction) const
        {
            const auto i = edges.find(edgeid_t(from, direction));
            if (i != edges.end())
                return i->second;
            else
                return unmarked;
        }

    private:
        nodes_t nodes;
        edges_t edges;
    };



    void dump_tree(const Bin_Tree_Element *root,
                   highlight_t& highlight,
                   const std::string& prefix,
                   const std::string& label = "");
}