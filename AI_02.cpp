#include <iostream>
#include<vector>
#include<queue>

using namespace std;

//---------------------- Struct to use -------------------------------------------------
struct State
{
    vector<int> vec;
    int blankPos;

    bool operator== (const State& _s);
    bool operator!= (const State& _s);

    State() {}
    State(const int& n); //Randomly generate
};

struct ReachedState
{
    State state;
    int evaluation;

    ReachedState(const State& _s, const int& _e);
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
    int evaluation;
    
    vector<Node> expand(Problem& problem, vector<ReachedState>& reachedStates, const int& _heuristic);

    void constructSolution(Problem& problem);

    Node(const int& _heuristic, const int& _pathCost);
};

struct CompareNode //For Priority Queue
{
    bool operator()(const Node& lnode, const Node& rnode)
    {
        return lnode.evaluation > rnode.evaluation; //Using Min-heap Priority Queue
    }
};

//---------------------------------- Struct's implemetation -------------------------------------------------
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
bool State::operator!= (const State& _s)
{
    return vec != _s.vec;
}


Node::Node(const int& _heuristic, const int& _pathCost)
{
    parent = nullptr;
    evaluation = _heuristic + _pathCost;
    pathCost = _pathCost;
}

vector<Node> Node::expand(Problem& problem, vector<ReachedState>& reachedStates, const int& _heuristic)
{
    vector<Node> vecExpanded;
    vector<State> models = problem.trasitionModel(state);
    
    for(State state : models)
    {
        Node node (_heuristic, pathCost + 1);
        node.state = state;
        node.parent = this;

        bool unreached = true;
        for(ReachedState rstate : reachedStates)
        {
            if(node.state == rstate.state)
            {
                unreached = false;
                if(node.evaluation < rstate.evaluation)
                {
                    vecExpanded.push_back(node);
                    rstate.evaluation = node.evaluation;
                }
                break;
            }
        }

        if(unreached)
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

ReachedState::ReachedState(const State& _s, const int& _e)
{
    state = _s;
    evaluation = _e;
}

//---------------------------------------- Algorithms -----------------------------------------------------
void UCS(Problem& problem)
{
    priority_queue<Node, vector<Node>, CompareNode> frontier;
    vector<ReachedState> reachedStates;

    Node node(0, 0);
    node.state = problem.initialState;
    frontier.push(node);

    int count = 1;
    cout << "Run" << endl;
    while(!frontier.empty())
    {
        std::cout << count++ << ' ';
        node = frontier.top();
    
        if(problem.isGoal(node.state))
        {
            node.constructSolution(problem);
            return;
        }

        frontier.pop();
        reachedStates.push_back(ReachedState(node.state, node.evaluation));

        vector<Node> expandedNodes = node.expand(problem, reachedStates, 0);

        for(Node node : expandedNodes)
            frontier.push(node);
    }
}

int main()
{
    Problem problem(9);
    problem.initialState.vec.clear();
    problem.initialState.vec.push_back(1);
    problem.initialState.vec.push_back(0);
    problem.initialState.vec.push_back(2);
    problem.initialState.vec.push_back(3);
    problem.initialState.vec.push_back(4);
    problem.initialState.vec.push_back(5);
    problem.initialState.vec.push_back(6);
    problem.initialState.vec.push_back(7);
    problem.initialState.vec.push_back(8);
    problem.initialState.blankPos = 1;

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
