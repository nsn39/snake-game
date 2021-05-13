#include "../../src/solver/bfs_solver.cpp"
#include <stack>
#include <string>
#include <utility>
#include <iostream>

//Abstraction for a DFS Snake Solver
class DFS_Solver
{
    private:
        int food_X, food_Y, b_size;
        std::string path;
        int p_length;
        Snake* game_snake;
    public:
        DFS_Solver(int, int, int, Snake*);
        std::string path_to_food();
        int path_length();
};