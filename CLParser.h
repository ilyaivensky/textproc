/*
 * CLParser.h
 *
 * Parser for command line arguments
 *
 *  Created on: Apr 26, 2014
 *      Author: ilyaivensky
 */

#ifndef CLPARSER_H_
#define CLPARSER_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

class CLParser
{
public:

    CLParser(int argc, char * argv[], bool switches_on = false);
    ~CLParser(){}

    string get_arg(int i);
    string get_arg(const string & s);

private:

    int argc;
    vector<string> argv;

    bool switches_on;
    map<string, string> switch_map;
};

#endif /* CLPARSER_H_ */
