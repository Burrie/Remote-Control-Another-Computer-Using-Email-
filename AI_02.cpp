#include <iostream>
#include<vector>
#include<queue>

using namespace std;

#define ACTION_COST 1 //Transition from one state to another take only 1 step

//---------------------- Struct to use -------------------------------------------------
struct State
{
    int blankPos;
    vector<int> vec;

    bool operator== (const State& _s);
    State& operator=(const State& _s);

    State() {}
    State(const int& n); //Randomly generate
};

struct ReachedState
{
    int evaluation;
    State state;

    ReachedState(const State& _s, const int& _e);
};

struct Problem
{
    State goalState, initialState;
    int actions[4] {-1, 1, -3, 3};

    vector<State> trasitionModel(const State& _s);
    bool isGoal(const State& _s);

    queue<State> solution;

    Problem(const int& _n);
};

struct Node
{
    Node* parent;
    State state;
    int pathCost, evaluation, numberOfChildren;
    
    vector<Node*> expand(Problem& problem, vector<ReachedState>& reachedStates, const int& _heuristic);

    Node(const int& _heuristic, const int& _pathCost);
    ~Node();
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
        vec.push_back(n - 1 - i); //Fix initial state
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
State& State::operator=(const State& _s)
{
    vec = _s.vec;
    blankPos = _s.blankPos;

    return *this;
}


Node::Node(const int& _heuristic, const int& _pathCost)
{
    parent = nullptr;
    evaluation = _heuristic + _pathCost;
    pathCost = _pathCost;
    numberOfChildren = 0;
}
Node::~Node()
{
    parent->numberOfChildren--;
    if(parent->numberOfChildren == 0)
        delete parent;
}
vector<Node*> Node::expand(Problem& problem, vector<ReachedState>& reachedStates, const int& _heuristic)
{
    vector<Node*> vecExpanded;
    vector<State> models = problem.trasitionModel(state);
    
    for(State s : models)
    {
        Node* node = new Node(_heuristic, pathCost + ACTION_COST);
        node->state = s;
        node->parent = this; //To backtrack when the goal is found

        bool shouldAdd = true; //Classify Node to add to frontier
        for(ReachedState rstate : reachedStates)
        {
            if(node->state == rstate.state)
            {
                shouldAdd = false;
                if(node->evaluation < rstate.evaluation)
                {
                    shouldAdd = true;
                    rstate.evaluation = node->evaluation;
                }
                break;
            }
        }

        if(shouldAdd)
        {
            numberOfChildren++;
            vecExpanded.push_back(node);
        }
    }
    
    return vecExpanded;
}

ReachedState::ReachedState(const State& _s, const int& _e)
{
    state = _s;
    evaluation = _e;
}

//---------------------------------------- Algorithms -----------------------------------------------------

void constructSolution(Problem& problem, Node* leafNode)
{
    while (leafNode != nullptr)
    {
        problem.solution.push(leafNode->state);
        Node* toDelete = leafNode;
        leafNode = leafNode->parent;
        delete toDelete;
    }
}

void Search(Problem& problem, const int& heuristic)
{
    priority_queue<Node*, vector<Node*>, CompareNode> frontier;
    vector<ReachedState> reachedStates;

    Node* node = new Node(heuristic, 0);
    node->state = problem.initialState;
    frontier.push(node);

    while(!frontier.empty())
    {
        node = frontier.top();
    
        if(problem.isGoal(node->state))
        {
            constructSolution(problem, node);
            cout << "Finish";
            return;
        }

        frontier.pop();
        reachedStates.push_back(ReachedState(node->state, node->evaluation));

        for (Node* n : node->expand(problem, reachedStates, 0))
            frontier.push(n);

        if(node->numberOfChildren == 0) //Delete useless leaf node
            delete node;
    }
}

void UCS(Problem& problem)
{
    Search(problem, 0);
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
