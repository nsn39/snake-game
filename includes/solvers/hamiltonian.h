#include "../../solver/longest_path_solver.cpp"

//Solving snake game by creating a hamiltonian cycle for the snake's head
//Abstraction for Hamiltonian_Solver
class Hamiltonian_Solver
{
    private:
        std::string path;
        int head_X;
        int head_Y;
        int b_size;

    public:
        Hamiltonian_Solver(int, int, int);
        std::string get_hamiltonian_path();
        int h_path_length();
};