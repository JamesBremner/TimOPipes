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
    eType type()
    {
        return myType;
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

    /** Add a pipe to the plumbing
     * @param[in] type_src node type where water enters pipe
     * @param[in] src name of node where water enters pipe
     * @param[in] type_dst node type where water leaves pipe
     * @param[in] dst name of node where water leaves pipe
     */
    void add(
        cNode::eType type_src, const std::string &src,
        cNode::eType type_dst, const std::string &dst);

    /** Get all possible pairs of source and discharge nodes
     * @return vevtor of node name pairs
     */
    std::vector<std::pair<std::string, std::string>>
    SourceDischargePairs();

    /** find pipe between two nodes
     * @param[in] start name of node at input
     * @param[in] end name of node at output
     * @return interator to pipe found, or end() if no pipe exists
     */
    std::vector<cPipe>::iterator find(
        const std::string &start,
        const std::string &end);

    /// Conform to STL container
    std::vector<cPipe>::iterator begin()
    {
        return myPipes.begin();
    }
    /// Conform to STL container
    std::vector<cPipe>::iterator end()
    {
        return myPipes.end();
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
            dst)));
}

std::vector<std::pair<std::string, std::string>>
cPlumbing::SourceDischargePairs()
{
    std::vector<std::pair<std::string, std::string>> ret;
    std::vector<std::string> vSource;
    std::vector<std::string> vSink;
    for (auto &p : myPipes)
    {
        if (p.start()->type() == cNode::eType::source)
            vSource.push_back(p.start()->name());
        if (p.end()->type() == cNode::eType::discharge)
            vSink.push_back(p.end()->name());
    }
    for (auto &src : vSource)
        for (auto &dst : vSink)
            ret.push_back(std::make_pair(src, dst));

    return ret;
}

std::vector<cPipe>::iterator
cPlumbing::find(
    const std::string &start,
    const std::string &end)
{
    return std::find(
        myPipes.begin(), myPipes.end(),
        std::make_pair(start, end));
}