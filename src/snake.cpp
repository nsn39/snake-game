#include "../includes/snake.h"

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
    
    if (current_head_direction != RIGHT)
    {
        current_head_direction = LEFT;
        block_list[0]->set_block_direction(LEFT);
    }
    
}

void Snake::move_right()
{
    
    if (current_head_direction != LEFT)
    {
        current_head_direction = RIGHT;
        block_list[0]->set_block_direction(RIGHT);
    }
    
}

void Snake::move_down()
{
    
    if (current_head_direction != UP)
    {
        current_head_direction = DOWN;
        block_list[0]->set_block_direction(DOWN);
    }
    
}

void Snake::move_up()
{
    
    if (current_head_direction != DOWN)
    {
        current_head_direction = UP;
        block_list[0]->set_block_direction(UP);
    }
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

//Checking if a square lies outside the body of the snake
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

//Checking if the head of the snake has collided with any of its rem body block
//If so, then game over
bool Snake::has_collided()
{
    bool ans = false;
    int head_X = get_block_x_position(0);
    int head_Y = get_block_y_position(0);
    for(int i=1; i<length; i++)
    {
        int block_X = get_block_x_position(i);
        int block_Y = get_block_y_position(i);

        if (head_X == block_X && head_Y == block_Y)
        {
            ans = true;
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