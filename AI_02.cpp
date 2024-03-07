#include<string>
#include<vector>
#include<queue>

using namespace std;

struct State
{
    vector<int> vec;
    int blankPos;

    State() {}
    State(const int& n);
    State(vector<int> _v, int _blankPos);
};
State::State(const int& n)
{
    for(int i = 0; i < n; i++)
        vec.push_back(0);
    for(int i = 1; i < n; i++)
    {
        int tmp = rand() & n;
        while (vec[tmp] != 0)
        {
            if(++tmp == n)
                tmp = 0;
        }
        vec[tmp] = i;
    }
    for(int i = 0; i < n; i++)
        if(vec[i] == 0)
        {
            blankPos = i;
            break;
        }
}
State::State(vector<int> _v, int _blankPos)
{
    vec = _v;
    blankPos = _blankPos;
}


struct Node
{
    State state;
    Node* parent;
    int pathCost;

    Node(const State& _state);
    
    vector<Node> expand();
};

Node::Node(const State& _state)
{
    state = _state;
    parent = nullptr;
    pathCost = 0;
}

vector<Node> Node::expand()
{
    vector<Node> vecExpanded;

    vector<int> vec = state.vec;
    int actions[] {-1, 1, -3, 3};
    for(int i = 0; i < 4; i++)
    {
        if(state.blankPos + actions[i] < vec.size())
        {
            vector<int> newVec = vec;
            newVec[state.blankPos] = newVec[state.blankPos + actions[i]];
            newVec[state.blankPos + actions[i]] = 0;

            Node node = Node(State(newVec, state.blankPos + actions[i]));
            node.parent = this;
            node.pathCost = pathCost + 1;

            vecExpanded.push_back(node);
        }
    } 
    
    return vecExpanded;
}

void Sort_Expanded_Nodes(vector<Node> &nodes, vector<State> reachedStates)
{
    int reachStatesSize = reachedStates.size();
    int size = nodes.size();

    //Sort
    bool unSorted = true;
    while (unSorted)
    {
        unSorted = false;
        for(int i = 0; i < size - 1; i++)
        {
            if(nodes[i].pathCost > nodes[i+1].pathCost)
                {
                    unSorted = true;
                    Node tmp = nodes[i];
                    nodes[i] = nodes[i+1];
                    nodes[i+1] = tmp;
                }
        }
    }
}

Node UCS(State initialState, int n)
{
    if(initialState.isGoal())
        return Node(initialState);

    queue<Node> frontier;
    vector<State> reachedStates;

    frontier.push(Node(initialState));

    while(!frontier.empty())
    {
        Node node = frontier.front();
        frontier.pop();

        reachedStates.push_back(node.state);

        vector<Node> expandedNodes = node.expand();

        if(expandedNodes[0].state.isGoal())
            return expandedNodes[0];

        /*
            Sort vector of Nodes according to 2 criteria:
            + Nodes have lower path cost stand in front of nodes have higher.
            + Nodes have state in reachedStates stand at the back and be eliminated.
        */
        Sort_Expanded_Nodes(expandedNodes, reachedStates); //Using bubble sort

        int tmp = expandedNodes.size();
        for(int i = 0; i < tmp; i++)
            frontier.push(expandedNodes[i]);
    }

    return Node(State());
}
