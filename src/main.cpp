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

struct
{
    string templatePath;
    vector<string> classes;
    bool templateSet;
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
    opt.example = "cpppropgen -h\n\ncpppropgen -c class1";
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

    bool configured = false;

    opt.parse(argc, argv);

    if (opt.isSet("-c"))
    {
        vector< vector<string> > classes;
        opt.get("-c")->getMultiStrings(classes);
        for (int j = 0; j < classes.size(); ++j)
        {
            vector<string> cv = classes[j];
            for (int i = 0; i < cv.size(); ++i)
            {
                p.classes.push_back(cv[i]);
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
    //printBanner();
    configureOptions(argc, argv);
    if (p.needsExit) return 0;

    for (int i = 0; i < p.classes.size(); i++)
    {
        string classname = p.classes[i];
        cout << "Generationg class " << classname << endl;

        string outName = classname + ".h";
        string templatePath = p.templateSet ? p.templatePath : outName;

        generator g(templatePath, outName);
        membersVector mems;
        g.generate(classname, mems);
    }

    /*string templatePath;
    string classname;
    string outName;
    if (argc > 3 || argc == 1)
    {
        cout    << "Ussage: " << endl
                << "[to generate from template] generator <class name> <template path>" << endl
                << "\t <class name>    - name of class" << endl
                << "\t <template path> - path to template" << endl
                << "[to update file] generator <class name>" << endl
                << "\t <class name>    - name of class" << endl;

        return 0;
    }
    else if (argc == 2)
    {
        //Указано только имя класса
        classname = string(argv[1]);
        outName = classname + ".h";
        templatePath = outName;
    }
    else
    {
        //Указаны имя класса и имя шаблона
        templatePath = string(argv[2]);
        classname = string(argv[1]);
        outName = classname + ".h";
    }

    generator g(templatePath, outName);
    membersVector mems;
    //mems.push_back(make_pair("index", "int"));
    //mems.push_back(make_pair("vec", "std::vector<int>"));

    g.generate("test_class", mems);*/
    return 1;
}