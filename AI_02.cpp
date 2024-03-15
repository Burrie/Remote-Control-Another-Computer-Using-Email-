#include<iostream>
#include<vector>
#include<queue>
#include<math.h>
#include<time.h>

using namespace std;

#define ACTION_COST 1 //Transition from one state to another take only 1 step

int n;

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

    vector<State> solution;

    Problem(const int& _n);
};

struct Node
{
    Node* parent;
    State state;
    int pathCost, evaluation, numberOfChildren;
    
    typedef int(* Heuristic) (const State& _s);
    vector<Node*> expand(Problem& problem, vector<ReachedState>& reachedStates, Heuristic heuristic);

    Node(const int& _heuristic, const int& _pathCost);
};

struct CompareNode //For Priority Queue
{
    bool operator()(const Node* lnode, const Node* rnode)
    {
        return lnode->evaluation > rnode->evaluation; //Using Min-heap Priority Queue
    }
};

//---------------------------------- Struct's implemetation -------------------------------------------------
int countInversions(const State& state)
{
    int count = 0;
    int size = state.vec.size();

    for(int i = 1; i < size; i++)
    {
        if(state.vec[i] < 1)
            continue;
        for(int j = i - 1; j >= 0; j--)
        {
            if(state.vec[j] > 0 && state.vec[i] < state.vec[j])
                count++;
        }
    }

    return count;
}
bool isSolvable(const int& n, const State& state)
{
    if((int)sqrt(n) % 2 != 0)
    {
        return countInversions(state) % 2 == 0;
    }
    
    int countRow = -1;
    for(int i = 0; i < state.vec.size(); i++)
    {
        if(state.vec[i] == 0)
            break;
        countRow++;
    }
    countRow /= sqrt(n);

    return (countRow + countInversions(state)) % 2 != 0;
}
Problem::Problem(const int& n)
{
    do
    {
        initialState = State(n);
    }
    while(!isSolvable(n, initialState)); //If initial is not solvable -> regenerate

    for(int i = 1; i < n; i++)
        goalState.vec.push_back(i);
    goalState.vec.push_back(0);
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
        vec.push_back(-1);

    srand((unsigned)time(NULL));
    
    for(int i = 0; i < n; i++)
    {
        int random = rand() % n;
        while(vec[random] != -1)
        {
            if(++random >= n)
                random = 0;
        }
        vec[random] = i;

        if(i == 0)
            blankPos = random;
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
    parent = NULL;
    evaluation = _heuristic + _pathCost;
    pathCost = _pathCost;
    numberOfChildren = 0;
}
typedef int (* Heuristic)(const State& _s);
vector<Node*> Node::expand(Problem& problem, vector<ReachedState>& reachedStates, Heuristic heuristic)
{
    vector<Node*> vecExpanded;
    vector<State> models = problem.trasitionModel(state);
    
    for(State s : models)
    {
        Node* node = new Node(heuristic(s), pathCost + ACTION_COST);
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
    while (leafNode != NULL)
    {
        problem.solution.push_back(leafNode->state);
        leafNode = leafNode->parent;
    }
}

typedef int (* Heuristic)(const State& _s);
void Search(Problem& problem, Heuristic heuristic, double& memoryConsumed, double& exercutionTime)
{
    clock_t start, end;
    start = clock();

    memoryConsumed = 0;

    priority_queue<Node*, vector<Node*>, CompareNode> frontier;
    vector<ReachedState> reachedStates;

    Node* node = new Node(heuristic(problem.initialState), 0);
    node->state = problem.initialState;
    frontier.push(node);
    memoryConsumed += sizeof(node);

    bool goalFound = false;
    while(!frontier.empty())
    {
        node = frontier.top();
    
       if(goalFound == false)
       {
            if(problem.isGoal(node->state))
            {
                constructSolution(problem, node);
                goalFound = true;
                cout << "has goal";
                continue;
            }

            frontier.pop();
            memoryConsumed -= sizeof(node);

            reachedStates.push_back(ReachedState(node->state, node->evaluation));
            memoryConsumed += sizeof(ReachedState(node->state, node->evaluation));

            for (Node* n : node->expand(problem, reachedStates, heuristic))
            {
                frontier.push(n);
                memoryConsumed += sizeof(n);
            }

            while(node->numberOfChildren == 0) //Delete useless leaf node
            {
                node->parent->numberOfChildren--;
                Node* _parent = node->parent;
                delete node;
                node = _parent;

                memoryConsumed -= sizeof(node);
            }
       }
       else //Delete all Nodes left in frontier
       {
            frontier.pop();
            delete node;
       }
    }

    end = clock();
    exercutionTime = double(end - start) * 1000 / double(CLOCKS_PER_SEC);
}

int Heuristic_UCS(const State& _s)
{
    return 0;
}

int Heuristic_AStar(const State& _s) //Using "Inversion Distance" Heuristic
{
    int verticalID = countInversions(_s) / (int)sqrt(n) + countInversions(_s) % (int)sqrt(n);
    int horizontalID = 0;

    return verticalID + horizontalID;
}

int main()
{
    n = 9;
    Problem problem(n);

    cout << "Initial state: ";
    for(int i : problem.initialState.vec)
        cout << i;
    cout << '\n' << countInversions(problem.initialState);
    cout << "\nGoal state: ";
    for(int i : problem.goalState.vec)
        cout << i;
    cout << endl << endl;

    double memoryConsumed, exercutionTime;
    Search(problem, Heuristic_UCS, memoryConsumed, exercutionTime);

    int solutionSize = problem.solution.size();
    if(solutionSize == 0)
        cout << "There is no solution";
    else
    {
        cout << "Solution path: ";
        for(int i = solutionSize - 1; i >= 0; i--)
        {
            cout << "\n--> ";
            int count = 0;
            for(int num : problem.solution[i].vec)
            {
                cout << num << ' ';
                if(++count % 3 == 0)
                    cout << "\n    ";
            }
        }

        cout << "\nExercution time: " << exercutionTime << " (Miliseconds).";
        cout << "\nMemory: " << memoryConsumed / (1024*1024) << " (MB)."; //From Bytes -> MegaBytes
    }

    return 0;
}
