#include <iostream>
#include<string>
#include<vector>
#include<queue>

using namespace std;

struct State
{
    vector<int> vec;
    int blankPos;

    bool operator== (const State& _s);

    State() {}
    State(const int& n); //Randomly generate
};

struct Problem
{
    State goalState, initialState;
    int actions[4] {-1, 1, -3, 3};

    vector<State> trasitionModel(const State& _s);
    bool isGoal(const State& _s);

    vector<State> solution;

    Problem(const int& _n);
};

struct Node
{
    State state;
    Node* parent;
    int pathCost;
    
    vector<Node> expand(Problem& problem, vector<State> reachedStates, const bool& isGraphSearch, bool& goalFound);

    void constructSolution(Problem& problem);

    Node();
};


Problem::Problem(const int& _n)
{
    initialState = State(_n);
    for(int i = 0; i < _n; i++)
        goalState.vec.push_back(i);
}
vector<State> Problem::trasitionModel(const State& _s)
{
    vector<State> models;

    int size = _s.vec.size();
    for(int i = 0; i < 4; i++)
    {
        if(_s.blankPos + actions[i] < size && _s.blankPos + actions[i] >= 0) //Swap blank pos
        {
            State s = _s;
            s.vec[s.blankPos] = s.vec[s.blankPos + actions[i]];
            s.vec[s.blankPos += actions[i]] = 0; //Update blank pos
            models.push_back(s);
        }
    }

    return models;
}
bool Problem::isGoal(const State& _s)
{
    return goalState == _s;
}


State::State(const int& n)
{
    for(int i = 0; i < n; i++)
        vec.push_back(n - 1 - i);
    for(int i = 0; i < n; i++)
        if(vec[i] == 0)
        {
            blankPos = i;
            break;
        }
}
bool State::operator== (const State& _s)
{
    return vec == _s.vec;
}


Node::Node()
{
    parent = nullptr;
    pathCost = 0;
}
vector<Node> Node::expand(Problem& problem, vector<State> reachedStates, const bool& isGraphSearch, bool& goalFound)
{
    vector<Node> vecExpanded;
    vector<State> models = problem.trasitionModel(state);

    // for(State s : models)
    // {
    //     cout << "--> ";
    //     for(int i : s.vec)
    //     {
    //         cout << i << ' ';
    //     }
    //     cout << '\n';
    // }

    int modelsSize =  models.size();
    int reachedSize = reachedStates.size();
    
    for(int i = 0; i < modelsSize; i++)
    {
        if(isGraphSearch)
            for(int j = 0; j < reachedSize; j++)
                if(models[i] == reachedStates[j])
                    continue;

        Node node;
        node.state = models[i];
        node.parent = this;
        node.pathCost = pathCost + 1;

        if(problem.isGoal(models[i]))
        {
            cout << "Has goal\n";

            vector<Node> goalVec;
            goalVec.push_back(node);
            goalFound = true;
            return goalVec;
        }

        vecExpanded.push_back(node);
    }
    
    return vecExpanded;
}
void Node::constructSolution(Problem& problem)
{
    Node* tmp = this;
    while (tmp != nullptr)
    {
        problem.solution.push_back(tmp->state);
        tmp = tmp->parent;
    }
}

typedef <class T>
struct LinkedList
{
    T* list;
}

void UCS(Problem& problem)
{
    if(problem.isGoal(problem.initialState))
        return;

    queue<Node> frontier;
    vector<State> reachedStates;

    Node node;
    node.state = problem.initialState;
    frontier.push(node);

    int count = 0;
    while(!frontier.empty())
    {
        node = frontier.front();
        frontier.pop();

        reachedStates.push_back(node.state);

        bool goalFound = false;
        vector<Node> expandedNodes = node.expand(problem, reachedStates, true, goalFound);

        if(goalFound)
        {
            expandedNodes[0].constructSolution(problem);
            return;
        }

        /*
            Sort vector of Nodes according to 2 criteria:
            + Nodes have lower path cost stand in front of nodes have higher.
            + Nodes have state in reachedStates stand at the back and be eliminated.
        */

        int expandedSize = expandedNodes.size();
        for(int i = 0; i < expandedSize; i++)
            frontier.push(expandedNodes[i]);

        cout << count++ << ' ';
    }
}

int main()
{
    Problem problem(9);
    problem.initialState.vec.clear();
    problem.initialState.vec.push_back(1);
    problem.initialState.vec.push_back(2);
    problem.initialState.vec.push_back(0);
    problem.initialState.vec.push_back(3);
    problem.initialState.vec.push_back(4);
    problem.initialState.vec.push_back(5);
    problem.initialState.vec.push_back(6);
    problem.initialState.vec.push_back(7);
    problem.initialState.vec.push_back(8);
    problem.initialState.blankPos = 2;

    cout << "Initial state: ";
    for(int i : problem.initialState.vec)
        cout << i;
    cout << "\nGoal state: ";
    for(int i : problem.goalState.vec)
        cout << i;
    cout << endl;

    UCS(problem);
    cout << "This is solution size: " << problem.solution.size();

    return 0;
}
