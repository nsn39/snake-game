//Constant for board size must match with that of another file
const int BOARD_SIZE = 25;
const int MAX_BLOCKS = 2000;

//Enumerations for block positions
enum Direction
{
    UP, 
    DOWN, 
    LEFT,
    RIGHT
};

//Abstraction for a position vector
class Position
{
    public:
        int x_val, y_val;
    public:
        Position(int x, int y) : x_val(x), y_val(y) {}

        int get_X() 
        {
            return x_val;
        }

        int get_Y() 
        {
            return y_val;
        }
};

//Abstraction for a snake block
class Block
{
    private:
        Position* block_position;
        Direction block_direction;
    
    public:
        Block(int x_val, int y_val, Direction d_val)
        {
            block_position = new Position(x_val, y_val);
            block_direction = d_val;
        }

        int get_X();
        int get_Y();
        Direction get_block_direction();
        void set_block_direction(Direction);
        void update_block_position();
};


//Abstraction for a snake.
class Snake
{
    private:
        int length;
        Direction current_head_direction;
        Direction prev_frame_last_block_dir;
        int prev_frame_last_block_pos_x, prev_frame_last_block_pos_y;
        //array of block objects.
        Block* block_list[MAX_BLOCKS];

    public:
        Snake(int);
        void move_left();
        void move_right();
        void move_down();
        void move_up();
        int get_snake_length();
        int get_block_x_position(int);
        int get_block_y_position(int);
        void increment_length();
        void increment_snake();
        void update_position();
        void update_direction();
        bool has_eaten(int, int);
        bool has_collided();
        bool is_out_of_body(int, int);
};