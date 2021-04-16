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
        void update_position();
        void update_direction();
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
    //Loop throught the block_list from first to last and update each block's position
    for(int i=0; i<length; i++) 
    {
        block_list[i]->update_block_position();
    }
}

void Snake::update_direction()
{
    //For updating directions of each block start from the end block up to the second and replace each
    //block's direction with the block ahead of it and set the first one's equal to the current_head_direction
    for(int i=length-1; i>=1; i--)
    {
        Direction prev_direction = block_list[i-1]->get_block_direction();
        block_list[i]->set_block_direction(prev_direction);
    }
    block_list[0]->set_block_direction(current_head_direction);
}