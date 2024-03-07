#include <iostream>

void AStarSearch(char** state, const int& n)
{

}

void UCS(char** state, const int& n)
{

}

void printState(char** board, const int& n)
{
    std::cout << "----------------------------\n";
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
            std::cout << board[i][j] << ' ';
        std::cout << '\n';
    }
}

char** generateState(const int& n)
{
    char** board = new char*[n];
    for(int i = 0; i < n; i++)
        board[i] = new char[n];

    /*
        Something goes here...
    */

    return board;
}

void Start()
{
    int n;
    char choice;
    
    do
    {
        std::cout << "N: ";
        std::cin >> n;
    }while (n <= 1);
    
    do
    {
        std::cout << "\n1: UCS, 2: A*\nYour choice of algorithm to use: ";
        std::cin >> choice;
    }while (choice != '1' && choice != '2');

    char** initialState = generateState(n);

    if(choice == '1') UCS(initialState, n);
    else AStarSearch(initialState, n);
}

int main()
{
    Start();
    
    return 0;
}