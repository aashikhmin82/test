#pragma once

#include <cstddef>
#include <string>
#include <memory>

using namespace std;

class class_huffman_tree_element
{
    private:
        size_t count;
        uint16_t letter;
        std::shared_ptr<class_huffman_tree_element> l_up = nullptr,
            l_down = nullptr,
            r_down = nullptr,
            r_up = nullptr;

    public:
        class_huffman_tree_element(size_t symbol_count, uint16_t uniq_letter) : count(symbol_count), letter(uniq_letter) 
        {
        }

        const size_t sym_count() const
        {
            return count;
        }

        const uint16_t sym_letter() const
        {
            return letter;
        }
        
        uint16_t& new_sym_letter()
        {
            return letter;
        }
        
        std::shared_ptr<class_huffman_tree_element>& left_up()
        {
            return l_up;
        }

        const std::shared_ptr<class_huffman_tree_element> left_up() const
        {
            return l_up;
        }

        std::shared_ptr<class_huffman_tree_element>& right_up()
        {
            return r_up;
        }

        const std::shared_ptr<class_huffman_tree_element> right_up() const
        {
            return r_up;
        }

        std::shared_ptr<class_huffman_tree_element>& weak_left_down()
        {
            return l_down;
        }

        const std::shared_ptr<class_huffman_tree_element> left_down() const
        {
            return l_down;
        }

        std::shared_ptr<class_huffman_tree_element>& weak_right_down()
        {
            return r_down;
        }

        const std::shared_ptr<class_huffman_tree_element> right_down() const
        {
            return r_down;
        }
};
