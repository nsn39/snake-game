#include <SDL2/SDL.h>
#include "snake.cpp"
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

//Snake board height and width
const int BOARD_WIDTH = 40;
const int BOARD_HEIGHT = 40;
const int GRID_SPACING = 14;
const int TRANSLATE_BY = 62;

//Starts up SDL and creates window
bool init();

//Frees the resources and closes SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Draw the grid of the board
void draw_grid(int, int);

bool init()
{
    //Initialization flag
	bool success = true;

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
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

			}
		}
	}

	return success;
}

void close()
{
    //Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL
	SDL_Quit();
}

void draw_grid(int width, int height) 
{
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );	
    
    //Draw all the horizontal lines
    for(int i=0; i<width+1; i++)
    {
        int start_X = 0 + TRANSLATE_BY;
        int start_Y = (i * GRID_SPACING) + TRANSLATE_BY;
        int end_X = (GRID_SPACING * width) + TRANSLATE_BY;
        int end_Y = (i * GRID_SPACING) + TRANSLATE_BY;
        SDL_RenderDrawLine( gRenderer, start_X, start_Y, end_X, end_Y);
    }

    //Draw all the vertical lines
    for(int i=0; i<height+1; i++)
    {
        int start_X =  (i * GRID_SPACING) + TRANSLATE_BY;
        int start_Y = 0 + TRANSLATE_BY;
        int end_X = (i * GRID_SPACING) + TRANSLATE_BY;
        int end_Y = (GRID_SPACING * height) + TRANSLATE_BY;
        SDL_RenderDrawLine( gRenderer, start_X, start_Y, end_X, end_Y);
    }
}

int main(int argc, char* args[]) 
{   

    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
    else
    {
        //Main loop flag
        bool quit = false;

        //Event handler
        SDL_Event e;

        //Initializing a new snake of length 6.
        Snake* python = new Snake(16);

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
            }

            //Update the snake's position and block direction
            python->update_position();
            python->update_direction();

            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            //Draw the grid
            draw_grid(BOARD_WIDTH, BOARD_HEIGHT);

            //Draw the snake
            int snake_len = python->get_snake_length();
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );

            for(int i=0; i<snake_len; i++)
            {
                int block_starting_X = python->get_block_x_position(i) * GRID_SPACING + TRANSLATE_BY;
                int block_starting_Y = python->get_block_y_position(i) * GRID_SPACING + TRANSLATE_BY;

                SDL_Rect fillRect = { block_starting_X, block_starting_Y, GRID_SPACING, GRID_SPACING };
                SDL_RenderFillRect( gRenderer, &fillRect );
            }

            //Draw the food.
            
            //Update screen
            SDL_RenderPresent( gRenderer );

            //Delay for 300 milliseconds.
            SDL_Delay(100);
        }
    }

    return 0;
}