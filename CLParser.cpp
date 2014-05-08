/*
 * CLParser.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: ilyaivensky
 */

#include "CLParser.h"

#include <algorithm>

CLParser::CLParser(int argc_, char * argv_[], bool switches_on_) :
	argc(argc_), switches_on(switches_on_)
{
    argv.resize(argc);
    copy(argv_, argv_ + argc, argv.begin());

    // map the switches to the actual
    // arguments if necessary
    if (switches_on && !argv.empty())
    {
        for (auto it1 = argv.begin(), it2 = next(it1); it2 != argv.end(); ++it1, ++it2)
        {
            if ((*it1)[0] == '-')
                switch_map[*it1]=*(it2);
        }
    }
}

string CLParser::get_arg(int i)
{
    if (i >= 0 && i < argc)
        return argv[i];

    return "";
}

string CLParser::get_arg(const string & s)
{
    if (!switches_on)
    	return "";

    auto it = switch_map.find(s);
    if (it != switch_map.end())
        return it->second;

    return "";
}


