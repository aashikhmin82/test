#pragma once

bool fsm_check_match(const std::string& heystack, const std::string& needle);
bool match_with_quadratic_complexity(const std::string& heystack, const std::string& needle);
bool regex_check_match(const std::string& heystack, const std::string& needle);
bool boyer_moore_match(const std::string& heystack, const std::string& needle);
