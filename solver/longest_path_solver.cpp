#include "../includes/solvers/longest_path_solver.h"

//Construction and methods implementation
Longest_Solver::Longest_Solver(int board_size, int x_val, int y_val, Snake* s_object)
{
    food_X = x_val;
    food_Y = y_val;
    b_size = board_size;

    //First, initialize and BFS_Solver and get the shortest path directions.
    solver = new BFS_Solver(board_size, x_val, y_val, s_object);
    shortest_path = solver->path_to_food();
    s_obj = s_object;
}

std::string Longest_Solver::path_to_food()
{   
    int prev_len = solver->path_length();

    //Then set up a 2D Array for tracking visited squares.
    int visited[b_size][b_size] = {};

    //Vector of list of pair of x and y coordinates for the shortest path.
    std::vector<std::pair<int, int>> s_path_coords;

    //Array of path_directons ; to be concatenated at the end.
    std::vector<std::string> l_path_directions;

    //Fill in the s_path_coords vector
    int head_X = s_obj->get_block_x_position(0);
    int head_Y = s_obj->get_block_y_position(0);

    //Create an std::pair of snake's head block and push to s_path_coords
    std::pair<int, int> temp_pair = std::make_pair(head_X, head_Y);
    s_path_coords.push_back(temp_pair);

    //variables for traversing the grid.
    int temp_x_val = head_X, temp_y_val = head_Y;

    for(int i=1; i<=prev_len; i++)
    {   
        if(shortest_path[i-1] == 'R')
        {
            temp_x_val = (temp_x_val + 1) % b_size;
        }
        else if(shortest_path[i-1] == 'L')
        {
            temp_x_val = (temp_x_val - 1) > -1 ? temp_x_val - 1 : b_size - 1;
        }
        else if (shortest_path[i-1] == 'D')
        {
            temp_y_val = (temp_y_val + 1) % b_size;
        }
        else if(shortest_path[i-1] == 'U')
        {
            temp_y_val = (temp_y_val - 1) > -1 ? temp_y_val - 1 : b_size - 1;
        }
        s_path_coords.push_back(std::make_pair(temp_x_val, temp_y_val));
        visited[temp_x_val][temp_y_val] = 1;
    }

    //Iterate through the snake's body and set all block coordinates as visited.
    for(int i=0; i<s_obj->get_snake_length(); i++)
    {
        int block_x_coords = s_obj->get_block_x_position(i);
        int block_y_coords = s_obj->get_block_y_position(i);
        visited[block_x_coords][block_y_coords] = 1;
    }

    //Iterate throught the shortest path string and keep on increasing the path
    for(int i=0; i<prev_len; i++)
    {
        int first_block_X = s_path_coords[i].first;
        int first_block_Y = s_path_coords[i].second;
        int second_block_X = s_path_coords[i+1].first;
        int second_block_Y = s_path_coords[i+1].second;

        //Decide whether up/left OR down/right
        int ul_count = 0, dr_count = 0;

        //After deciding, set the new squares in the path as visited.
        if(shortest_path[i] == 'L' || shortest_path[i] == 'R')
        {
            int temp_fX = first_block_X;
            int temp_fY = first_block_Y;
            int temp_sX = second_block_X;
            int temp_sY = second_block_Y;

            //check for up direction
            while(true)
            {
                temp_fY = (temp_fY - 1) > -1 ? temp_fY - 1: b_size -1;
                temp_sY = (temp_sY - 1) > -1 ? temp_sY - 1: b_size -1;
                if (!visited[temp_fX][temp_fY] && !visited[temp_sX][temp_sY])
                {
                    ul_count++;
                }
                else
                {
                    break;
                }
            }

            temp_fY = first_block_Y;
            temp_sY = second_block_Y;

            //check for down direction
            while(true)
            {
                temp_fY = (temp_fY + 1) % b_size;
                temp_sY = (temp_sY + 1) % b_size;
                if (!visited[temp_fX][temp_fY] && !visited[temp_sX][temp_sY])
                {
                    dr_count++;
                }
                else
                {
                    break;
                }
            }

            //compare the counts and select to distract up/down
            //then mark the new squares as visited and assign string to l_path_direction
            if (ul_count >= dr_count)
            {
                std::string temp = std::string(ul_count, 'U') + shortest_path[i] + std::string(ul_count, 'D');
                l_path_directions.push_back(temp);

                temp_fX = first_block_X;
                temp_fY = first_block_Y;
                temp_sX = second_block_X;
                temp_sY = second_block_Y;

                //Marking all the new squares a visited.
                for(int j=1; j<=ul_count; j++)
                {
                    temp_fY = (temp_fY - 1) > -1 ? temp_fY - 1 : b_size - 1;
                    temp_sY = (temp_sY - 1) > -1 ? temp_sY - 1 : b_size - 1;
                    visited[temp_fX][temp_fY] = 1;
                    visited[temp_sX][temp_sY] = 1;
                }
            }
            else 
            {
                std::string temp = std::string(ul_count, 'D') + shortest_path[i] + std::string(ul_count, 'U');
                l_path_directions.push_back(temp);
                
                temp_fX = first_block_X;
                temp_fY = first_block_Y;
                temp_sX = second_block_X;
                temp_sY = second_block_Y;

                //Marking all the new squares a visited.
                for(int j=1; j<=dr_count; j++)
                {
                    temp_fY = (temp_fY + 1) % b_size;
                    temp_sY = (temp_sY + 1) % b_size;
                    visited[temp_fX][temp_fY] = 1;
                    visited[temp_sX][temp_sY] = 1;
                }
            }
        }
        else if (shortest_path[i] == 'U' || shortest_path[i] == 'D')
        {
            int temp_fX = first_block_X;
            int temp_fY = first_block_Y;
            int temp_sX = second_block_X;
            int temp_sY = second_block_Y;

            //check for left direction
            while(true)
            {
                temp_fX = (temp_fX - 1) > -1 ? temp_fX - 1: b_size -1;
                temp_sX = (temp_sX - 1) > -1 ? temp_sX - 1: b_size -1;
                if (!visited[temp_fX][temp_fY] && !visited[temp_sX][temp_sY])
                {
                    ul_count++;
                }
                else
                {
                    break;
                }
            }

            temp_fX = first_block_X;
            temp_sX = second_block_X;

            //check for right direction
            while(true)
            {
                temp_fX = (temp_fX + 1) % b_size;
                temp_sX = (temp_sX + 1) % b_size;
                if (!visited[temp_fX][temp_fY] && !visited[temp_sX][temp_sY])
                {
                    dr_count++;
                }
                else
                {
                    break;
                }
            }

            //compare the counts and select to distract up/down
            //then mark the new squares as visited and assign string to l_path_direction
            if (ul_count >= dr_count)
            {
                std::string temp = std::string(ul_count, 'L') + shortest_path[i] + std::string(ul_count, 'R');
                l_path_directions.push_back(temp);

                temp_fX = first_block_X;
                temp_fY = first_block_Y;
                temp_sX = second_block_X;
                temp_sY = second_block_Y;

                //Marking all the new squares a visited.
                for(int j=1; j<=ul_count; j++)
                {
                    temp_fX = (temp_fX - 1) > -1 ? temp_fX - 1 : b_size - 1;
                    temp_sX = (temp_sX - 1) > -1 ? temp_sX - 1 : b_size - 1;
                    visited[temp_fX][temp_fY] = 1;
                    visited[temp_sX][temp_sY] = 1;
                }
            }
            else
            {
                std::string temp = std::string(dr_count, 'R') + shortest_path[i] + std::string(dr_count, 'L');
                l_path_directions.push_back(temp);

                temp_fX = first_block_X;
                temp_fY = first_block_Y;
                temp_sX = second_block_X;
                temp_sY = second_block_Y;

                //Marking all the new squares a visited.
                for(int j=1; j<=dr_count; j++)
                {
                    temp_fX = (temp_fX + 1) % b_size;
                    temp_sX = (temp_sX + 1) % b_size;
                    visited[temp_fX][temp_fY] = 1;
                    visited[temp_sX][temp_sY] = 1;
                }
            }
        }
    }

    //Concatenate the new_path_directions and return 
    std::string final_l_path = "";
    for(int i=0; i<l_path_directions.size(); i++)
    {
        final_l_path += l_path_directions[i];
    }

    longest_path = final_l_path;
    return longest_path;
}

//Return the length of the longest path string found.
int Longest_Solver::path_length()
{
    return longest_path.length();
}