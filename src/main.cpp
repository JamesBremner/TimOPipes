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

void Test()
{
    cPlumbing thePlumbing;
    
    thePlumbing.add(
        node_t( new cNode(
            cNode::eType::source,
            "source1")),
         node_t( new cNode(
            cNode::eType::none,
            "n1")));
       thePlumbing.add(
         node_t( new cNode(
            cNode::eType::none,
            "n1")),
         node_t( new cNode(
            cNode::eType::none,
            "n2"))); 
        thePlumbing.add(
         node_t( new cNode(
            cNode::eType::none,
            "n2")),
         node_t( new cNode(
            cNode::eType::none,
            "n3")));
        thePlumbing.add(
         node_t( new cNode(
            cNode::eType::none,
            "n3")),
         node_t( new cNode(
            cNode::eType::discharge,
            "sink1")));

    // for( auto& n : theNodes )
    //     std::cout << n.name() << " ( " << n.id() << " )\n"; 
    // for( auto& p : thePipes )
    //     std::cout << p.text() << "\n";

    // initialize graph with pipe tree
    raven::graph::cPathFinder PF;
    PF.directed();
    for (auto &p : thePlumbing)
        PF.addLink(
            p.start()->name(),
            p.end()->name());
    std::cout << PF.linksText() << "\n";

    // find path from source to discharge
    PF.start(PF.find("source1"));
    PF.end(PF.find("sink1"));
    PF.path();

    // print segments in path
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

main()
{
    Test();

    cGUI theGUI;
    return 0;
}
