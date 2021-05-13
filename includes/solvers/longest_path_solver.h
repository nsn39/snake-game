#include "../../src/solver/dfs_solver.cpp"
#include <vector>
#include <utility>
#include <iostream>

//Abstraction for longest path creator
class Longest_Solver
{
    private:
        int food_X, food_Y, b_size;
        std::string shortest_path;
        std::string longest_path;
        BFS_Solver* solver;
        Snake* s_obj;
    public:
        Longest_Solver(int, int, int, Snake*);
        std::string path_to_food();
        int path_length();
};
