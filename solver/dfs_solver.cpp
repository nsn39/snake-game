#include "../includes/solvers/dfs_solver.h"

//Constructor
DFS_Solver::DFS_Solver(int board_size, int x_val, int y_val, Snake* snake_obj)
{
    b_size = board_size;
    food_X = x_val;
    food_Y = y_val;
    game_snake = snake_obj;
}

//Methods
std::string DFS_Solver::path_to_food()
{
    int head_X = game_snake->get_block_x_position(0);
    int head_Y = game_snake->get_block_y_position(0);
    
    
    //Tracking the visited squares
    int visited[b_size][b_size] = {};

    //2d array of strings to find the path
    std::string path_strings[b_size][b_size];
    path_strings[head_X][head_Y] = "";

    std::stack<std::pair<int, int>> squares_to_traverse;
    squares_to_traverse.push(std::make_pair(head_X, head_Y));
    visited[head_X][head_Y] = 1;

    while(!squares_to_traverse.empty())
    {
        std::pair<int, int> point = squares_to_traverse.top();
        squares_to_traverse.pop();

        int x_pos = point.first;
        int y_pos = point.second;

        int x_temp, y_temp;
        
        //Check if the point popped is our destination
        if (x_pos == food_X && y_pos == food_Y)
        {
            break;
        }


        //Checking for point above
        y_temp = y_pos - 1 > -1 ? y_pos - 1 : b_size - 1;
        if (!visited[x_pos][y_temp] && game_snake->is_out_of_body(x_pos, y_temp))
        {
            visited[x_pos][y_temp] = 1;
            //std::cout << "pushing " << x_pos << " and " << y_temp << std::endl;
            squares_to_traverse.push(std::make_pair(x_pos, y_temp));
            path_strings[x_pos][y_temp] = path_strings[x_pos][y_pos] + "U";
        }

        //Checking for point down
        y_temp = (y_pos + 1) % b_size;
        if (!visited[x_pos][y_temp] && game_snake->is_out_of_body(x_pos, y_temp))
        {
            visited[x_pos][y_temp] = 1;
            //std::cout << "pushing " << x_pos << " and " << y_temp << std::endl;
            squares_to_traverse.push(std::make_pair(x_pos, y_temp));
            path_strings[x_pos][y_temp] = path_strings[x_pos][y_pos] + "D";
        }

        //Checking for point left
        x_temp = x_pos - 1 > -1 ? x_pos - 1 : b_size - 1;
        if (!visited[x_temp][y_pos] && game_snake->is_out_of_body(x_temp, y_pos))
        {
            visited[x_temp][y_pos] = 1;
            //std::cout << "pushing " << x_temp << " and " << y_pos << std::endl;
            squares_to_traverse.push(std::make_pair(x_temp, y_pos));
            path_strings[x_temp][y_pos] = path_strings[x_pos][y_pos] + "L";
        }

        //Checking for point right
        x_temp = (x_pos + 1) % b_size;
        if (!visited[x_temp][y_pos] && game_snake->is_out_of_body(x_temp, y_pos))
        {
            visited[x_temp][y_pos] = 1;
            //std::cout << "pushing " << x_temp << " and " << y_pos << std::endl;
            squares_to_traverse.push(std::make_pair(x_temp, y_pos));
            path_strings[x_temp][y_pos] = path_strings[x_pos][y_pos] + "R";
        }
    }

    path = path_strings[food_X][food_Y];
    p_length = path.length();
    return path_strings[food_X][food_Y];
}

int DFS_Solver::path_length()
{
    return p_length;
}