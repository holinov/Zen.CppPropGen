#include "stdafx.h"

#include <iostream>
#include <utility>

#include "generator.h"
#include "ezOptionParser.hpp"

using namespace std;
using namespace ez;

void printBanner()
{
    cout << endl << "CppProperty generator launched." << endl;
}

typedef struct cinfo_s
{
    string classname;
    string templatename;
    string outname;
} classinfo;

struct
{
    string templatePath;
    vector<string> classes;
    bool templateSet;

    vector<classinfo> classinfos;
    bool needsExit;
} p;

void configureOptions(int argc, char const *argv[])
{
    ezOptionParser opt;
    //launchParams p = {0};
    p.needsExit = false;
    p.templateSet = false;
    opt.overview = "Property getter\\setter generator for C++";
    opt.syntax = "cpppropgen [OPTIONS]";
    opt.example = "cpppropgen -h\r\ncpppropgen -c class1,class2 -c class3\r\n\r\n";
    opt.footer = "cpppropgen v1.0.0 Copyright (C) 2014 Kholinow Ilya\nThis program is free and without warranty.\n";

    opt.add(
        "", // Default.
        0, // Required?
        0, // Number of args expected.
        0, // Delimiter if expecting multiple args.
        "Print this usage message in one of three different layouts.", // Help description.
        "-h",     // Flag token.
        "-help", // Flag token.
        "--help", // Flag token.
        "--usage" // Flag token.
    );

    opt.add(
        "",
        0,
        -1,
        ',',
        "Set class list to process",
        "-c",
        "--class"
    );

    opt.add(
        "",
        0,
        1,
        0,
        "Set template path",
        "-t",
        "--template"
    );

    opt.add(
        "",
        0,
        -1,
        ',',
        "Set files to process",
        "-f",
        "--files"
    );

    bool configured = false;

    opt.parse(argc, argv);

    if (opt.isSet("-f"))
    {
        vector< vector<string> > files;
        opt.get("-f")->getMultiStrings(files);
        for (int j = 0; j < files.size(); ++j)
        {
            vector<string> cv = files[j];
            for (int i = 0; i < cv.size(); ++i)
            {
                std::string fileName = cv[i];
                size_t  from = fileName.rfind(cPathSeparator) + 1,
                        to = fileName.rfind('.');
                size_t len = to - from;
                string classname = fileName.substr(from, len);

                //когда указан файл вход и выход совпадают
                classinfo ci  = {classname, fileName, fileName};
                p.classinfos.push_back(ci);
            }
        }
        configured = true;
    }

    if (opt.isSet("-t"))
    {
        string tmpl;
        opt.get("-t")->getString(tmpl);
        p.templatePath = tmpl;
        p.templateSet = true;
    }

    if (opt.isSet("-c"))
    {
        vector< vector<string> > classes;
        opt.get("-c")->getMultiStrings(classes);
        for (int j = 0; j < classes.size(); ++j)
        {
            vector<string> cv = classes[j];
            for (int i = 0; i < cv.size(); ++i)
            {
                //p.classes.push_back(cv[i]);
                string classname = cv[i];
                string fileName = classname + ".h";
                string templatename = p.templateSet ? p.templatePath : fileName;
                classinfo ci = {classname, fileName, templatename};
                p.classinfos.push_back(ci);
            }
        }
        configured = true;
    }



    if (opt.isSet("-h")  || !configured)
    {
        std::string usage;
        opt.getUsage(usage);
        std::cout << usage;
        p.needsExit = true;
        return;
    }

    return;
}

int main(int argc, char const *argv[])
{
    configureOptions(argc, argv);
    if (p.needsExit) return 0;

    for (classinfo i : p.classinfos)
    {
        //string classname = i;
        cout << "Generating class " << i.classname << " to file: " << i.outname << " template path: " << i.templatename << endl;

        //string outName = classname + ".h";
        //string templatePath = p.templateSet ? p.templatePath : outName;

        generator g(i.templatename, i.outname);
        membersVector mems;
        g.generate(i.classname, mems);
    }
    return 1;
}