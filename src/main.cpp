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
    // Construct a pipe tree with two limbs
    thePlumbing.add(
        cNode::eType::source, "source1",
        cNode::eType::none, "n1");
    thePlumbing.add(
        cNode::eType::none, "n1",
        cNode::eType::none, "n2");
    thePlumbing.add(
        cNode::eType::none, "n2",
        cNode::eType::none, "n3");
    thePlumbing.add(
        cNode::eType::none, "n3",
        cNode::eType::discharge, "sink1");
    thePlumbing.add(
        cNode::eType::none, "n2",
        cNode::eType::none, "n4");
    thePlumbing.add(
        cNode::eType::none, "n4",
        cNode::eType::discharge, "sink2");

    // initialize graph with pipe tree
    PF.directed();
    for (auto &p : thePlumbing)
        PF.addLink(
            p.start()->name(),
            p.end()->name());
    std::cout << PF.linksText() << "\n";

    // loop over every possible source, sink pair
    for (auto &sd : thePlumbing.SourceDischargePairs())
    {
        // find path from source to sink
        PF.start(PF.find(sd.first));
        PF.end(PF.find(sd.second));
        PF.path();

        // check if path exists
        if ( ! PF.getPath().size())
            continue;

        // print segments in path
        PrintPathPipes();
    }

}

main()
{
    Test();

    cGUI theGUI;
    return 0;
}
