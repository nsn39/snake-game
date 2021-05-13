#include <SDL2/SDL.h>
#include "helpers/ltexture.cpp"
#include "solver/hamiltonian.cpp"
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

//Snake board height and width
const int BOARD_WIDTH = 25;
const int BOARD_HEIGHT = 25;
const int GRID_SPACING = 14;
const int TRANSLATE_BY = 62;

//Tracking the score of our player
//+10 for every food eaten.
int SCORE = 0;

//Starts up SDL and creates window
bool init();

//Loads up the font file
bool loadMedia();

//Frees the resources and closes SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Load texture object
LTexture* gTextTexture = NULL;

//Draw the grid of the board
void draw_grid(int, int);

//The coordinates for the food of snake
int food_X, food_Y;

//Selecting a random new location for our food
void new_food_location(Snake*);

//Solver objects
DFS_Solver* solver1 = NULL;
BFS_Solver* solver2 = NULL;
Longest_Solver* solver3 = NULL;
Hamiltonian_Solver* solver4 = NULL;

//The path-string and index for the snake given by the solver.
std::string snake_path;
int snake_path_index = 0;
int snake_path_length;

//Save up the scores in a separate file
void save_score(int, std::string);

bool init()
{
    //Initialization flag
	bool success = true;

    //Initialize random seed
    srand(time(NULL));

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
    //Loading success flag
	bool success = true;
    
	//Open the font
	gFont = TTF_OpenFont( "../static/Antonio-Medium.ttf", 18 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{       
        gTextTexture = new LTexture(gRenderer, gFont);
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture->loadFromRenderedText( "Score: 0", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
        
	}
    
	return success;
}

void close()
{
    //Free loaded images
	gTextTexture->free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

    //Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL
    TTF_Quit();
	SDL_Quit();
}

void draw_board(int width, int height) 
{
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );	
    
    //Drawing our main board rect
    SDL_Rect boardRect = {TRANSLATE_BY, TRANSLATE_BY, GRID_SPACING*width, GRID_SPACING*height};
    SDL_RenderFillRect( gRenderer, &boardRect );
}

void new_food_location(Snake* python)
{
    while(true)
    {
        food_X = rand() % BOARD_WIDTH;
        food_Y = rand() % BOARD_HEIGHT;

        if (python->is_out_of_body(food_X, food_Y))
        {   
            //If the position is out of the snake's body you've got your location.
            break;
        }
    }
}

//Save the score in the corresponding file in scores directory
void save_score(int score, std::string game_mode)
{   
    std::fstream fio;
    std::string file_name = game_mode + ".txt";
    std::string full_path_to_file = "scores/" + file_name;

    fio.open(full_path_to_file, std::ios::app);

    fio << std::to_string(score) << std::endl;
    
    fio.close();
}

int main(int argc, char* args[]) 
{   

    //Save the second argument as a string
    std::string game_mode = args[1];

    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
    else 
    {
        //Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Initializing a new snake of length 6.
            Snake* python = new Snake(6);

            //Select a random location for the food.
            new_food_location(python);

            //First initialize a solver and assign the path string variables.
            if (game_mode != "human")
            {   
                if (game_mode == "dfs")
                {
                    solver1 = new DFS_Solver(BOARD_WIDTH, food_X, food_Y, python);
                    snake_path = solver1->path_to_food();
                    snake_path_length = solver1->path_length();
                }
                else if (game_mode == "bfs")
                {
                    solver2 = new BFS_Solver(BOARD_WIDTH, food_X, food_Y, python);
                    snake_path = solver2->path_to_food();
                    snake_path_length = solver2->path_length();
                }
                else if (game_mode == "longest")
                {
                    solver3 = new Longest_Solver(BOARD_WIDTH, food_X, food_Y, python);
                    snake_path = solver3->path_to_food();
                    snake_path_length = solver3->path_length();
                }
                else if (game_mode == "h")
                {   
                    int head_x_val = python->get_block_x_position(0);
                    int head_y_val = python->get_block_y_position(0);
                    solver4 = new Hamiltonian_Solver(BOARD_WIDTH, head_x_val, head_y_val);
                    snake_path = solver4->get_hamiltonian_path();
                    snake_path_length = solver4->h_path_length();
                }
                
                snake_path_index = 0;
                
                //Setting the direction of snake's head
                if (snake_path[0] == 'L')
                {
                    python->move_left();
                }
                else if (snake_path[0] == 'R')
                {
                    python->move_right();
                }
                else if (snake_path[0] == 'U')
                {
                    python->move_up();
                }
                else if (snake_path[0] == 'D')
                {
                    python->move_down();
                }
            }
            

            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a movement key
                    else if (e.type == SDL_KEYDOWN && game_mode == "human")
                    {   
                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                            python->move_up();
                            break;

                            case SDLK_DOWN:
                            python->move_down();
                            break;

                            case SDLK_LEFT:
                            python->move_left();
                            break;

                            case SDLK_RIGHT:
                            python->move_right();
                            break;

                            default:
                            break;
                        }
                    }
                }

                //If the game_mode is not human then move snake according to path_string
                //std::cout << snake_path_index << snake_path[snake_path_index] << std::endl;
                if (game_mode != "human")
                {
                    
                    if (snake_path[snake_path_index] == 'L')
                    {
                        python->move_left();
                        snake_path_index++;
                    }
                    else if (snake_path[snake_path_index] == 'R')
                    {
                        python->move_right();
                        snake_path_index++;
                    }
                    else if (snake_path[snake_path_index] == 'U')
                    {
                        python->move_up();
                        snake_path_index++;
                    }
                    else if (snake_path[snake_path_index] == 'D')
                    {
                        python->move_down();
                        snake_path_index++;
                    }

                    //For hamiltonian solver, reset the index and start again.
                    if (game_mode == "h" && snake_path_length == snake_path_index)
                    {
                        snake_path_index = 0;
                    }
                }

                //Update the snake's position and block direction
                python->update_position();
                python->update_direction();

                //Check first if the snake has bitten itself
                //If so, then quit the game
                if (python->has_collided())
                {
                    save_score(SCORE, game_mode);
                    quit = true;
                }

                //Check if the head and the food location matches.
                //If matches then 
                //  i) Increment the snake
                //  ii) Select a new location for the food.
                if (python->has_eaten(food_X, food_Y))
                {
                    SCORE += 1;
                    python->increment_snake();
                    new_food_location(python);

                    //Request a new pathstring from the solver
                    //Also set path_string_index to 0.
                    if (game_mode == "dfs")
                    {
                        solver1 = new DFS_Solver(BOARD_WIDTH, food_X, food_Y, python);
                        snake_path = solver1->path_to_food();
                    }
                    else if (game_mode == "bfs")
                    {
                        solver2 = new BFS_Solver(BOARD_WIDTH, food_X, food_Y, python);
                        snake_path = solver2->path_to_food();
                    }
                    else if (game_mode == "longest")
                    {
                        solver3 = new Longest_Solver(BOARD_WIDTH, food_X, food_Y, python);
                        snake_path = solver3->path_to_food();
                    }
                    //No new path needed for hamiltonian solver.

                    snake_path_index = 0;
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //Draw the scoreboard.
                std::string scoreboard_text = "Score : " + std::to_string(SCORE);
                SDL_Color textColor = { 0, 0, 0 };
                bool uselessVariable = gTextTexture->loadFromRenderedText( scoreboard_text, textColor );
                gTextTexture->render( ( SCREEN_WIDTH - gTextTexture->getWidth() ) / 2, 25 );

                //Draw the game board.
                draw_board(BOARD_WIDTH, BOARD_HEIGHT);

                //Draw the food
                int food_position_X = food_X * GRID_SPACING + TRANSLATE_BY;
                int food_position_Y = food_Y * GRID_SPACING + TRANSLATE_BY;
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                SDL_Rect foodRect = {food_position_X, food_position_Y, GRID_SPACING, GRID_SPACING};
                SDL_RenderFillRect( gRenderer, &foodRect );

                //Draw the snake
                int snake_len = python->get_snake_length();
                
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
                for(int i=0; i<snake_len; i++)
                {
                    int block_starting_X = python->get_block_x_position(i) * GRID_SPACING + TRANSLATE_BY;
                    int block_starting_Y = python->get_block_y_position(i) * GRID_SPACING + TRANSLATE_BY;

                    SDL_Rect fillRect = { block_starting_X, block_starting_Y, GRID_SPACING, GRID_SPACING };
                    SDL_RenderFillRect( gRenderer, &fillRect );
                }
                
                //Update screen
                SDL_RenderPresent( gRenderer );

                //Delay for 300 milliseconds.
                SDL_Delay(35);
            }
        }
    }

    return 0;
}