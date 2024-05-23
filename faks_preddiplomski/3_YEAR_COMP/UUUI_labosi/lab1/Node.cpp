#include <string>
#include <list>
#include <map>

class Node {
    private:
        std::string state;
        double cost;
        double costWithHeuristic;
        Node *parent;
    public:
        Node(std::string state, Node *parent);
        Node(std::string state, Node *parent, double cost);
        Node(std::string state, Node *parent, double cost, double costWithHeuristic);
        std::string getState();
        double getCost();
        double getHeuristicCost();
        bool goal(std::list<std::string> goalStates);
        std::list<Node *> expand(std::string succ);
        std::list<Node *> expandHeuristic(std::string succ, std::map<std::string, float> heuristicMap);
        std::list<Node *> path();
        ~Node();

};

Node::Node(std::string state, Node *parent) {
    // za BFS
    this->state = state;
    this->parent = parent;
    this->cost = 0;
    this->costWithHeuristic = this->cost; // const
}

Node::Node(std::string state, Node *parent, double cost) {
    // za UCS
    this->state = state;
    this->parent = parent;
    this->cost = cost;
    this->costWithHeuristic = this->cost; // const
}

Node::Node(std::string state, Node *parent, double cost, double heuristic) {
    // za A*
    this->state = state;
    this->parent = parent;
    this->cost = cost;
    this->costWithHeuristic = this->cost + heuristic;
}

std::string Node::getState() {
    return this->state;
}

double Node::getCost() {
    return this->cost;
}

double Node::getHeuristicCost() {
    return this->costWithHeuristic;
}

bool Node::goal(std::list<std::string> goalStates) {
    for (auto it = goalStates.begin(); it != goalStates.end(); ++it) {
        if(!(*it).compare(this->state)) {
            return true;
        }
    }
    return false;
}

std::list<Node *> Node::expand(std::string succ) {
    std::list<Node *> succList;
    int begin = 0, middle, end;

    do
    {
        middle = succ.find_first_of(',', begin);
        end = succ.find_first_of(' ', begin);
        std::string nextState = succ.substr(begin, middle - begin);
        double nextCost = std::stod(succ.substr(middle + 1, end - middle - 1));
        nextCost += this->cost;

        Node *nextNode = new Node(nextState, this, nextCost);
        succList.push_back(nextNode);

        begin = end + 1;
    } while (end < succ.size());

    return succList;
}    
        
std::list<Node *> Node::expandHeuristic(std::string succ, std::map<std::string, float> heuristicMap) {
    std::list<Node *> succList = this->expand(succ);
    for (std::list<Node *>::iterator it = succList.begin(); it != succList.end(); ++it) {
        (*it)->costWithHeuristic = (*it)->getCost() + heuristicMap[(*it)->state];
    }
    
    return succList;
}

std::list<Node *> Node::path() {
    std::list<Node *> pathList;

    Node *currNode = this;

    do {
        pathList.push_front(currNode);
        currNode = currNode->parent;
    } while (currNode->parent != nullptr);
    pathList.push_front(currNode);
    
    return pathList;
}

Node::~Node() {
    delete parent;
}
