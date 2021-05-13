#include "../../includes/solvers/hamiltonian.h"

//Implementation for hamiltonian_solver
Hamiltonian_Solver::Hamiltonian_Solver(int board_size, int x_val, int y_val)
{
    std::cout << "solver initialized" << std::endl;
    b_size = board_size;
    head_X = x_val;
    head_Y = y_val;
}

//Create the hamiltonian path directions
std::string Hamiltonian_Solver::get_hamiltonian_path()
{
    //2D array to keep track of visited squares.
    int visited[b_size][b_size] = {};

    //Set the head-square as visited
    visited[head_X][head_Y] = 1;

    int temp_x = head_X, temp_y = head_Y;

    std::string path = "";

    std::cout << "debug point 1" << std::endl;

    //Repeat the process until you visit all the squares
    while(true)
    {
        //Move up one step
        int y_coord = (temp_y - 1) > -1 ? (temp_y - 1) : b_size - 1;
        if (!visited[temp_x][y_coord])
        {
            temp_y = y_coord;
            visited[temp_x][temp_y] = 1;
            path += 'U';
        }
        else {
            //We know we're in the last square of the hamiltonian path
            path += 'R';
            break;
        }

        //Keep on moving towards right until blocked.
        while(true)
        {
            int x_coord = (temp_x + 1) % b_size;
            if (!visited[x_coord][temp_y])
            {
                temp_x = x_coord;
                visited[temp_x][temp_y] = 1;
                path += 'R';
            }
            else
            {   
                //Cant go right then break out of the loop.
                break;
            }
        }        
    }

    return path;
}

//Return the length of our hamiltonian path.
int Hamiltonian_Solver::h_path_length()
{
    return path.length();
}
