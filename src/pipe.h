#include <memory>
class cNode;
typedef std::shared_ptr<cNode> node_t;
class cPipe
{
public:
    cPipe(
        node_t start,
        node_t end);
    node_t start()
    {
        return myStart;
    }
    node_t end()
    {
        return myEnd;
    }
    std::string text();

    bool operator==(std::pair<std::string, std::string> startEnd);

private:
    int myID;
    node_t myStart;
    node_t myEnd;
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
        const std::string &name);
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

/// A forest of connected pipes
class cPlumbing
{
public:
    /** Add a pipe to the plumbing
     * @param[in] src node where water enters pipe
     * @param[in] dst node where water leaves pipe
     */
    void add(
        node_t src,
        node_t dst);
    void add(
        cNode::eType type_src, const std::string &src,
        cNode::eType type_dst, const std::string &dst);

    std::vector<cPipe>::iterator begin()
    {
        return myPipes.begin();
    }
    std::vector<cPipe>::iterator end()
    {
        return myPipes.end();
    }
    std::vector<cPipe>::iterator find(
        const std::string &start,
        const std::string &end)
    {
        return std::find(
            myPipes.begin(), myPipes.end(),
            std::make_pair(start, end));
    }

private:
    std::vector<cPipe> myPipes;
};

cPipe::cPipe(node_t start, node_t end)
    : myStart(start), myEnd(end)
{
}

std::string cPipe::text()
{
    return "Pipe from " + myStart->name() + " to " + myEnd->name();
}

bool cPipe::operator==(std::pair<std::string, std::string> startEnd)
{
    return (
        myStart->name() == startEnd.first &&
        myEnd->name() == startEnd.second);
}

int cNode::myLastID;

cNode::cNode(
    cNode::eType type,
    const std::string &name)
    : myType(type), myName(name)
{
    myID = ++myLastID;
}

void cPlumbing::add(
    node_t src,
    node_t dst)
{
    myPipes.push_back(cPipe(src, dst));
}
void cPlumbing::add(
    cNode::eType type_src, const std::string &src,
    cNode::eType type_dst, const std::string &dst)
{
    add(
        node_t(new cNode(
            type_src,
            src)),
        node_t(new cNode(
            type_dst,
            dst)) );
}