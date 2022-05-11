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
    std::vector<cNode> theNodes;
    std::vector<cPipe> thePipes;

    // create pipe tree with one limb
    theNodes.push_back(cNode(cNode::eType::source, "source"));
    for (int k = 0; k < 3; k++)
        theNodes.push_back(cNode(cNode::eType::none, std::to_string(k+2)));
    theNodes.push_back(cNode(cNode::eType::discharge, "sink"));
    for (int k = 0; k < theNodes.size() - 1; k++)
        thePipes.push_back(cPipe(theNodes[k], theNodes[k + 1]));

    // for( auto& n : theNodes )
    //     std::cout << n.name() << " ( " << n.id() << " )\n"; 
    // for( auto& p : thePipes )
    //     std::cout << p.text() << "\n";

    // initialize graph with pipe tree
    raven::graph::cPathFinder PF;
    PF.directed();
    for( auto& p : thePipes )
    PF.addLink(
        p.start().name(),
        p.end().name());

    // find path from source to discharge
    PF.start( PF.find("source"));
    PF.end( PF.find("sink"));
    PF.path();
 

    // print segments in path
    auto path = PF.getPath();
    for( int k = 0; k < path.size()-1; k++ ) {
        auto it = std::find(
            thePipes.begin(), thePipes.end(),
            std::make_pair( 
                PF.userName(path[k]),
                PF.userName(path[k+1]) ) );
        if( it == thePipes.end() )
            throw std::runtime_error(
                "Cannot find path segment "
                + std::to_string( path[k] )
                + " " + std::to_string( path[k+1] ) );
        std::cout << it->text() << "\n";
    }
}

main()
{
    Test();

    cGUI theGUI;
    return 0;
}
