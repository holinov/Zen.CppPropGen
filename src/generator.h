#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
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
    generator(string templatePath, string resultPath)
        : m_templatePath(templatePath)
        , m_resultPath(resultPath)
    { };

    ~generator() {};

    void generate(string classname, membersVector mv)
    {
        cout << "Start generating for template: " << m_templatePath << endl;

        membersMap mmap;
        for (membersVector::iterator p = mv.begin(); p != mv.end(); ++p)
        {
            string type = p->second;
            string name = p->first;

            mmap[name] = type;
        }


        stringstream inStream;
        ifstream  templateFile(m_templatePath);
        if (templateFile.is_open())
        {
            string line;
            while ( getline (templateFile, line) )
            {
                inStream << line << endl;
            }
            templateFile.close();
        }

        string result = inStream.str();
        
        //Установка имени класса
        size_t pos;
        while ( (pos = result.find(cCNTemplate)) != std::string::npos)
        {
            result.replace(pos, cCNTemplate.size(), classname);
        }

        parseFields(result, mmap);

        cout << "----------- found members" << endl;
        for (membersMap::iterator p = mmap.begin(); p != mmap.end(); ++p)
            std::cout << p->first << " => " << p->second << '\n';

        cout << "-----------" << endl;
        //Генерация полей
        stringstream fieldString;
        fieldString << endl;
        for (membersMap::iterator p = mmap.begin(); p != mmap.end(); ++p)
        {
            fieldString << "\t" << p->second << " m_" << p->first << ";" << endl;
        }
        fieldString << "\t";
        replaceBetween(cPFBounds, fieldString.str(), result);

        //Генерация акксессоров
        stringstream accString;
        for (membersMap::iterator p = mmap.begin(); p != mmap.end(); ++p)
        {
            string type = p->second;
            string name = p->first;

            accString << endl;
            //генерация геттера
            accString   << "\t" << "const " << type << " get_" << name << "(){" << endl
                        << "\t\t" << "return m_" << name << ";" << endl
                        << "\t}" << endl;

            //генерация сеттера
            accString   << "\tvoid set_" << name << "(" << type << " val){" << endl
                        << "\t\tm_" << name << " = val;" << endl
                        << "\t}" << endl;
        }
        accString << "\t";
        replaceBetween(cPMBounds, accString.str(), result);
        //cout << "Result: " << endl << result << endl;

        ofstream ofile (m_resultPath);
        cout << "Writing to file " << m_resultPath << endl;
        if (ofile.is_open())
        {
            ofile << result;
            ofile.close();
        }
    }

private:
    void parseFields(string &where, membersMap &mmap)
    {
        size_t pfBegin = where.find(cPFBounds),
               pfEnd = where.rfind(cPFBounds);

        // если не нашли начало или конец - не производим парсинг
        if (pfBegin == std::string::npos || pfEnd == std::string::npos) return;

        size_t replaceStart = pfBegin + cPFBounds.size();
        size_t len = pfEnd - replaceStart;
        string part = where.substr(replaceStart, len);
        stringstream ps(trim(part));
        string line;
        while ( getline (ps, line) )
        {
            line = trim(line);
            std::vector<std::string> parts = split(line, ' ');
            string type = trim(parts[0]);
            string name = trim(parts[1]);
            name = name.substr(2, name.size() - 2);
            name = name.substr(0, name.size() - 1);
            mmap[name] = type;
        }
    }

    vector<string> split(const string &text, char seperator)
    {
        stringstream textStream(text);
        string segment;
        vector<std::string> seglist;

        while (std::getline(textStream, segment, seperator))
        {
            seglist.push_back(segment);
        }
        return seglist;
    }

    void replaceBetween(const string &bound, const string &newContent, string &where)
    {
        size_t pfBegin = where.find(bound),
               pfEnd = where.rfind(bound);

        // если не нашли начало или конец - не производим замену
        if (pfBegin == std::string::npos || pfEnd == std::string::npos) return;
        size_t replaceStart = pfBegin + bound.size();
        size_t countToReplace = pfEnd - replaceStart;
        where.replace(replaceStart, countToReplace, newContent);
    }

    membersMap m_members;
    string m_templatePath;
    string m_resultPath;
};