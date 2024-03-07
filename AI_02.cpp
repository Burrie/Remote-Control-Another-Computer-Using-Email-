#include<string>
#include<vector>
#include<queue>

using namespace std;

class State
{
    private:
        string state, goalState;
        int n;
    public:
        State();
        State(string s) {state = s;}

        void generateState();
        bool isGoal() {return state == goalState;}
};

class Node
{
    private:
        State state;
        Node* parent;
        int pathCost;
    public:
        Node(const State& _state) {state = _state; parent = nullptr; pathCost = 0;}
        vector<Node> expand();
        int getPathCost() {return pathCost;};
        State getState() {return state;};
};

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
            if(nodes[i].getPathCost() > nodes[i+1].getPathCost())
                {
                    unSorted = true;
                    Node tmp = nodes[i];
                    nodes[i] = nodes[i+1];
                    nodes[i+1] = tmp;
                }
        }
    }
}

Node UCS(State initialState)
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

        reachedStates.push_back(node.getState());

        vector<Node> expandedNodes = node.expand();

        if(expandedNodes[0].getState().isGoal())
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

    return Node(State(""));
}