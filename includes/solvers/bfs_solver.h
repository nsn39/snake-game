#include "../../snake.cpp"
#include <queue>
#include <string>
#include <utility>
#include <iostream>

//Abstraction for a BFS Snake Solver
class BFS_Solver
{
    private:
        int food_X, food_Y, b_size;
        std::string path;
        int p_length;
        Snake* game_snake;
    public:
        BFS_Solver(int, int, int, Snake*);
        std::string path_to_food();
        int path_length();
};