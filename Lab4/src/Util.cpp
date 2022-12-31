#include "Util.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(std::string str, const char del)
{
    std::stringstream ss(str);
    std::string tmp;
    std::vector<std::string> ans;

    str.erase(0, str.find_first_not_of(" \r\n\t\v\f"));
    str.erase(str.find_last_not_of(" \r\n\t\v\f") + 1);

    if (str.empty()) return ans;

    while (getline(ss, tmp, del)) ans.push_back(tmp);

    return ans;
}
