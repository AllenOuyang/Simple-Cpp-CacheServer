#ifndef _FIND_HPP
#define _FIND_HPP
#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "picosha2.hpp"

//using namespace std;

class Find
{
public:
    std::string dist;
    std::vector<std::string> ip;
    std::string findserver(std::string key);
    void renew(std::string distribution);
};

#endif