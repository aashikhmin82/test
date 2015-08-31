#include <iostream>
#include <vector>
#include <map>

using price_t = size_t;
using price_list_t = std::vector<price_t>;
using pivot_t = size_t;
using pivot_list_t = std::vector<pivot_t>;

class dynamic_db
{
    private:
//        std::vector <size_t> price;
        price_list_t price;
//        std::vector <size_t> scheme_plan;
        pivot_list_t scheme_plan;
//        std::vector <size_t> result_scheme;
        pivot_list_t result_scheme;
        std::map<size_t, std::vector<size_t>> db;
        std::map<size_t, size_t> db_util;

    public:
        dynamic_db(const std::vector<size_t>& price_tab) : price(price_tab) {}
        
        void print_price() const
        {
            size_t i=0;
            for (const auto price_val : price)
            {
                std::cout << "[" << i << "] : " << price_val << std::endl;
                ++i;
            }
        }

        void print_scheme() const
        {
            std::cout << "Scheme : ";
            for (const auto& part : scheme_plan)
                std::cout << part << " ";

            std::cout << "\n";
        }

        std::vector<size_t> print_result() const
        {
            size_t result_sum = 0;
            std::cout << "Result Scheme : ";
            for (const auto& part : result_scheme)
            {
                result_sum += price[part];
                std::cout << part << " ";
            }
            std::cout << "\n";

            std::cout << "MAX Result : " << result_sum << std::endl;
            return result_scheme;
        }

        void erase_old_pivot()
        {
//            result_scheme.erase(result_schem.begin(), result_schem.end());
            result_scheme.clear();
            result_scheme.shrink_to_fit();
        }

        void debug_print_db()
        {
//            for (auto it = db.begin(); it != db.end(); ++it)
            for (const auto& db_el : db)
            {
//                std::cout << "[DEBUG] " << (*it).first << " : ";
                std::cout << "[DEBUG] " << db_el.first << " : ";
//                for (auto el : db[(*it).first])
//                for (auto el : db[db_el->first])
                for (const auto& el : db[db_el.first])
                {
                    std::cout << el << " ";
                }
                std::cout << "\n";
            }
        }

        size_t cut(size_t length)
        {
            size_t rest = 0;

            cut_long(length);

            result_scheme.push_back(scheme_plan[0]);
//            if (scheme_plan[1])
            rest = scheme_plan[1];

            if (rest == 0)
            {
                std::cout << "DB_UTIL : " << std::endl;
//                for (std::map<size_t, size_t>::const_iterator i = db_util.begin(), end = db_util.end(); i != end; ++i)
                for (const auto& i : db_util)
                {
                    std::cout << i.first << " : " << i.second << std::endl;
                }
            }
            else
            {
                cut(rest);
            }

            return 1;
        }

        size_t cut_long(size_t length, size_t nest=0)
        {
            scheme_plan.clear();
            size_t cur_nest = nest;
            ++nest;
            size_t part1 = length, part2 = 0;
            size_t max_price = price[length];
            size_t price_debug = 0;

            if (( db_util.find(length) != db_util.end() ) and (cur_nest != 0)) 
            {

                std::cout << "[DEBUG] A1!" << std::endl;
                price_debug = db_util[length];
                std::cout << "[DEBUG BUFFER] Max Price : " << price_debug << std::endl;
                return price_debug;
            }

            size_t tmp_price = 0;
            for (size_t first_cut = 1; first_cut < length; ++first_cut)
            {
                if ( db_util.find(length - first_cut) != db_util.end() )
                {
                    tmp_price = price[first_cut] + db_util[length - first_cut];
                }
                else
                {
                    size_t tmp1 = cut_long(length - first_cut, nest);
                    tmp_price = price[first_cut] + tmp1;
                }

                if (tmp_price > max_price)
                {
                    part1 = first_cut;
                    part2 = length - first_cut;
                    max_price = tmp_price;
                }
            }

            if (cur_nest == 0)
            {
                scheme_plan.push_back(part1);
                scheme_plan.push_back(part2);
                std::cout << "[DEBUG] L2 : " << length << " " << part1 << " " << part2 << std::endl;
                db[length].push_back(part1);
                db[length].push_back(part2);
            }

            if ( db_util.find(length) != db_util.end() )
                price_debug = db_util[length];

            std::cout << "[DEBUG B] Price : " << max_price << "  Length : " << length << std::endl;
            std::cout << "[DEBUG B] Price_debug : " << price_debug << std::endl;

            if ( length != 1 and db_util.find(length) == db_util.end() )
            {
                db_util[length] = max_price;
            }

            return max_price;
        }
};
