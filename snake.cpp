//Constant for board size must match with that of another file
const int BOARD_SIZE = 40;
const int MAX_BLOCKS = 300;

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
        bool is_out_of_body(int, int);
};

//Block methods implementation
int Block::get_X()
{
    return block_position->x_val;
}

int Block::get_Y()
{
    return block_position->y_val;
}

Direction Block::get_block_direction()
{
    return block_direction;
}

void Block::update_block_position()
{
    int x_new, y_new;
    int x_prev = block_position->x_val;
    int y_prev = block_position->y_val;

    switch(block_direction) {
        case UP:
            x_new = x_prev;
            y_new = (y_prev - 1) > -1 ? y_prev - 1 : BOARD_SIZE - 1;
            break;

        case DOWN:
            x_new = x_prev;
            y_new = (y_prev + 1) % BOARD_SIZE;
            break;

        case LEFT:
            y_new = y_prev;
            x_new = (x_prev - 1) > -1 ? x_prev - 1 : BOARD_SIZE - 1 ;
            break;

        case RIGHT:
            y_new = y_prev;
            x_new = (x_prev + 1) % BOARD_SIZE;
            break;
        
        default:
            break;
    }

    block_position = new Position(x_new, y_new);
}

void Block::set_block_direction(Direction d_val)
{
    block_direction = d_val;
}

//Snake methods implementation
Snake::Snake(int init_length)
{
    length = init_length;
    current_head_direction = UP;

    //Fill the blocks_list up to the init_length with new block all pointing top
    for(int i=0; i<init_length; i++) {
        block_list[i] = new Block(20, 20+i, UP);
    }
}

void Snake::move_left()
{
    current_head_direction = LEFT;
}

void Snake::move_right()
{
    current_head_direction = RIGHT;
}

void Snake::move_down()
{
    current_head_direction = DOWN;
}

void Snake::move_up()
{
    current_head_direction = UP;
}

int Snake::get_snake_length()
{   
    return length;
}

int Snake::get_block_x_position(int block_no)
{
    return block_list[block_no]->get_X();
}

int Snake::get_block_y_position(int block_no)
{
    return block_list[block_no]->get_Y();
}

void Snake::update_position()
{   
    //Before updating, save the last block's position
    prev_frame_last_block_pos_x = block_list[length-1]->get_X();
    prev_frame_last_block_pos_y = block_list[length-1]->get_Y();
        
    //Loop throught the block_list from first to last and update each block's position
    for(int i=0; i<length; i++) 
    {
        block_list[i]->update_block_position();
    }
}

void Snake::update_direction()
{   
    //Before updating, save the last block's direction
    prev_frame_last_block_dir = block_list[length-1]->get_block_direction();

    //For updating directions of each block start from the end block up to the second and replace each
    //block's direction with the block ahead of it and set the first one's equal to the current_head_direction
    for(int i=length-1; i>=1; i--)
    {
        Direction prev_direction = block_list[i-1]->get_block_direction();
        block_list[i]->set_block_direction(prev_direction);
    }
    block_list[0]->set_block_direction(current_head_direction);
}

void Snake::increment_length() 
{
    length++;
}

void Snake::increment_snake()
{
    increment_length();
    block_list[length-1] = new Block(prev_frame_last_block_pos_x, 
                                    prev_frame_last_block_pos_y
                                    ,prev_frame_last_block_dir);
    
}

bool Snake::is_out_of_body(int x_val, int y_val)
{
    bool ans = true;
    for(int i=0; i<length; i++) 
    {   
        //For each block we're checking if the coordinates for food is different from them.
        if (x_val == block_list[i]->get_X() && y_val == block_list[i]->get_Y())
        {
            ans = false;
            break;
        }
    }
    return ans;
}

bool Snake::has_eaten(int x_val, int y_val)
{   
    //Checking if the new head position is the same as the food position of the snake.
    if (block_list[0]->get_X() == x_val && block_list[0]->get_Y() == y_val)
    {
        return true;
    }
    else
    {
        return false;
    }
}