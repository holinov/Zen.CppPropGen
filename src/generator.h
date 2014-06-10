#pragma once

#include "stdafx.h"

#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using namespace std;


const static string cCNTemplate = string("$className$");
const static string cPFBounds = string("/*--prop fields--*/");
const static string cPMBounds = string("/*--prop methods--*/");

// trim from start
static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}


typedef std::map<string, string> membersMap;
typedef pair<string, string> memberPair;
typedef std::vector<memberPair> membersVector;

class generator
{
public:
    generator(string templatePath, string resultPath,bool dryRun)
        : m_templatePath(templatePath)
        , m_resultPath(resultPath)
        , m_dryRun(dryRun)
    { };

    ~generator() {};

    void generate(string classname, membersVector mv);

private:
    void parseFields(string &where, membersMap &mmap);

    vector<string> split(const string &text, char seperator);

    void replaceBetween(const string &bound, const string &newContent, string &where);

    membersMap m_members;
    string m_templatePath;
    string m_resultPath;
    bool m_dryRun;
};