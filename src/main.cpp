#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "pipe.h"
#include "cPathFinder.h"

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        lb.move(50, 50, 100, 30);
        lb.text("Hello World");

        show();
        run();
    }

private:
    wex::label &lb;
};

cPlumbing thePlumbing;
raven::graph::cPathFinder PF;

std::vector<std::string> ParseSpaceDelimited(
    const std::string &l)
{
    std::vector<std::string> token;
    std::stringstream sst(l);
    std::string a;
    while (getline(sst, a, ' '))
        token.push_back(a);

    token.erase(
        remove_if(
            token.begin(),
            token.end(),
            [](std::string t)
            {
                return (t.empty());
            }),
        token.end());

    return token;
}

void read(const std::string &fname)
{
    std::ifstream f(fname);
    if (!f.is_open())
        throw std::runtime_error(
            "Cannot open input file");
    std::string line;
    while (getline(f, line))
    {

        auto tokens = ParseSpaceDelimited(line);
        if (tokens.size() != 4)
        {
            std::cout << line << "\n";
            throw std::runtime_error(
                "Bad inpiut line");
        }
        cNode::eType src_type = cNode::eType::none;
        cNode::eType dst_type = cNode::eType::none;
        if( tokens[1] == "source")
            src_type = cNode::eType::source;
        if( tokens[3] == "discharge")
            dst_type = cNode::eType::discharge;
        thePlumbing.add(
            src_type, tokens[0],
            dst_type, tokens[2] );
    }
}

void PrintPathPipes()
{
    auto path = PF.getPath();
    for (int k = 0; k < path.size() - 1; k++)
    {
        auto it = thePlumbing.find(
            PF.userName(path[k]),
            PF.userName(path[k + 1]));
        if (it == thePlumbing.end())
            throw std::runtime_error(
                "Cannot find path segment " + std::to_string(path[k]) + " " + std::to_string(path[k + 1]));
        std::cout << it->text() << "\n";
    }
}

void Test()
{
    // initialize graph with pipe tree
    PF.directed();
    for (auto &p : thePlumbing)
        PF.addLink(
            p.start()->name(),
            p.end()->name());
    //std::cout << PF.linksText() << "\n";

    // loop over every possible source, sink pair
    for (auto &sd : thePlumbing.SourceDischargePairs())
    {
        // find path from source to sink
        PF.start(PF.find(sd.first));
        PF.end(PF.find(sd.second));
        PF.path();

        // check if path exists
        if (!PF.getPath().size())
            continue;

        // mark source and sink as connected
        thePlumbing.Connected(sd);

        // print segments in path
        PrintPathPipes();
    }

    // Notify any unconnected sources and sinks
    auto u = thePlumbing.unConnected();
    if (u.length())
        std::cout << "The sources & sinks are not connected\n"
                  << u << "\n";
    else
        std::cout << "All sources and sinks are connected\n";
}

main()
{
    read("pipe.txt");
    Test();

    // cGUI theGUI;
    return 0;
}
