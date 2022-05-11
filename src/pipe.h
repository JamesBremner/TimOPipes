class cNode;
class cPipe
{
public:
    cPipe(cNode &start, cNode &end);
    cNode &start()
    {
        return myStart;
    }
    cNode &end()
    {
        return myEnd;
    }
    std::string text();

    bool operator==(std::pair<std::string,std::string> startEnd);

private:
    int myID;
    cNode &myStart;
    cNode &myEnd;
    double myPressureLoss;
};
class cNode
{
public:
    enum class eType
    {
        none,
        source,
        discharge,
    };
    cNode(
        eType type,
        const std::string& name );
    int id()
    {
        return myID;
    }
    std::string name()
    {
        return myName;
    }

private:
    eType myType;
    int myID;
    static int myLastID;
    std::string myName;
};

cPipe::cPipe(cNode &start, cNode &end)
    : myStart(start), myEnd(end)
{
}

std::string cPipe::text()
{
    return "Pipe from " + myStart.name() + " to " + myEnd.name();
}

bool cPipe::operator==(std::pair<std::string,std::string> startEnd)
{
    return (
        myStart.name() == startEnd.first &&
        myEnd.name() == startEnd.second);
}

int cNode::myLastID;

cNode::cNode(
    cNode::eType type,
    const std::string& name )
    : myType(type)
    , myName( name )
{
    myID = ++myLastID;
}
