/*
#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <SDL_ttf.h>
using namespace std;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* gTexture;
//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture

int score = 0;
class LTexture
{
    public:
        //Initializes variables
        LTexture();
        //Deallocates memory
        ~LTexture();
        //Loads image at specified path
        bool loadFromFile(string path);
        //Creates image from font string
        bool loadFromRenderedText( string textureText, SDL_Color textColor );

        //Deallocates texture
        void free();


        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


        //void render( int x, int y, SDL_Rect* clip = NULL);
        //Gets image dimensions
        int getWidth();
        int getHeight();
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        int mWidth;
        int mHeight;
};
LTexture gTextTexture;
LTexture gBackgroundTexture;
SDL_Rect gSpriteClips[5][7];
LTexture gSpriteSheetTexture[5];
LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool loadMedia()
{
    bool success = true;
    //Load sprite sheet texture

    gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

    if(!gSpriteSheetTexture[0].loadFromFile("dino_walking_.png"))
    {
         printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        gSpriteClips[0][ 0 ].x = 0;
        gSpriteClips[0][ 0 ].y =   0;
        gSpriteClips[0][ 0 ].w = 100;
        gSpriteClips[0][ 0 ].h = 100;

        //Set top right sprite
        gSpriteClips[0][ 1 ].x = 100;
        gSpriteClips[0][ 1 ].y =   0;
        gSpriteClips[0][ 1 ].w = 100;
        gSpriteClips[0][ 1 ].h = 100;

        //Set bottom left sprite
        gSpriteClips[0][ 2 ].x = 200;
        gSpriteClips[0][ 2 ].y = 100;
        gSpriteClips[0][ 2 ].w = 100;
        gSpriteClips[0][ 2 ].h = 100;

        //Set bottom right sprite
        gSpriteClips[0][ 3 ].x = 100;
        gSpriteClips[0][ 3 ].y = 100;
        gSpriteClips[0][ 3 ].w = 100;
        gSpriteClips[0][ 3 ].h = 100;

        gSpriteClips[0][ 4 ].x = 200;
        gSpriteClips[0][ 4 ].y = 200;
        gSpriteClips[0][ 4 ].w = 100;
        gSpriteClips[0][ 4 ].h = 100;

        gSpriteClips[0][ 5 ].x = 0;
        gSpriteClips[0][ 5 ].y =   0;
        gSpriteClips[0][ 5 ].w = 100;
        gSpriteClips[0][ 5 ].h = 100;

        gSpriteClips[0][ 6 ].x = 100;
        gSpriteClips[0][ 6 ].y =   0;
        gSpriteClips[0][ 6 ].w = 100;
        gSpriteClips[0][ 6 ].h = 100;


    }
    if( !gSpriteSheetTexture[1].loadFromFile( "doi.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        //Set top left sprite
        gSpriteClips[1][ 0 ].x = 200;
        gSpriteClips[1][ 0 ].y =   0;
        gSpriteClips[1][ 0 ].w = 100;
        gSpriteClips[1][ 0 ].h = 100;

        //Set top right sprite
        gSpriteClips[1][ 1 ].x = 100;
        gSpriteClips[1][ 1 ].y =   0;
        gSpriteClips[1][ 1 ].w = 100;
        gSpriteClips[1][ 1 ].h = 100;

        //Set bottom left sprite
        gSpriteClips[1][ 2 ].x = 0;
        gSpriteClips[1][ 2 ].y = 0;
        gSpriteClips[1][ 2 ].w = 100;
        gSpriteClips[1][ 2 ].h = 100;

        //Set bottom right sprite
        gSpriteClips[1][ 3 ].x = 200;
        gSpriteClips[1][ 3 ].y = 100;
        gSpriteClips[1][ 3 ].w = 100;
        gSpriteClips[1][ 3 ].h = 100;

        gSpriteClips[1][ 4 ].x = 100;
        gSpriteClips[1][ 4 ].y = 100;
        gSpriteClips[1][ 4 ].w = 100;
        gSpriteClips[1][ 4 ].h = 100;

        gSpriteClips[1][ 5 ].x = 0;
        gSpriteClips[1][ 5 ].y = 100;
        gSpriteClips[1][ 5 ].w = 100;
        gSpriteClips[1][ 5 ].h = 100;

        gSpriteClips[1][ 6 ].x = 200;
        gSpriteClips[1][ 6 ].y = 0;
        gSpriteClips[1][ 6 ].w = 100;
        gSpriteClips[1][ 6 ].h = 100;
    }
    if( !gSpriteSheetTexture[2].loadFromFile( "xr_di.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[2][ i ].x = 0;
            gSpriteClips[2][ i ].y = 0;
            gSpriteClips[2][ i ].w = 100;
            gSpriteClips[2][ i ].h = 100;
        }
    }

    if( !gSpriteSheetTexture[3].loadFromFile( "xr_din.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[3][ i ].x = 0;
            gSpriteClips[3][ i ].y = 0;
            gSpriteClips[3][ i ].w = 100;
            gSpriteClips[3][ i ].h = 100;
        }
    }
    if( !gSpriteSheetTexture[4].loadFromFile( "xr_double.png" ) )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        for(int i = 0; i < 7; i++)
        {
            gSpriteClips[4][ i ].x = 0;
            gSpriteClips[4][ i ].y = 0;
            gSpriteClips[4][ i ].w = 100;
            gSpriteClips[4][ i ].h = 100;
        }
    }
    if( !gBackgroundTexture.loadFromFile( "background_dinox.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    return success;
}


bool LTexture::loadFromFile(string path )
{
    //Get rid of preexisting texture
    free();
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL)
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText( string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();
	SDL_Surface* textSurface = NULL;
	//Render text surface
	textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}

	//Return success
	return mTexture != NULL;
}
void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
int LTexture::getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}
struct nhan_vat
{
    int index;
    int x;
    int y;
    int speed_x;
    int speed_x_1;
    int speed_y;
    int frame;
    //LTexture gSpriteSheetTexture;
    nhan_vat(int _d = 0,int _x = SCREEN_WIDTH, int _y = 470, int sp_x = 0, int sp_x_1 = 0, int sp_y = 0, int _f = 0)
    {
        index = _d;
        x = _x;
        y = _y;
        speed_x = sp_x;
        speed_x_1 = sp_x_1;
        speed_y = sp_y;
        frame = _f;
    }
    void animation(int n)
    {
        if(frame == n)
        frame = 0;
        SDL_Rect* current_clip = & gSpriteClips[index][frame];
        gSpriteSheetTexture[index].render(x, y, current_clip);
      //  SDL_Delay(50);
        SDL_RenderPresent(gRenderer);
        frame++;
    }
    void free_for_all()
    {
        gSpriteSheetTexture[index].free();
    }
    void move_left()
    {
        int k;
        if(x >= 0)
        k = speed_x_1;
        else k = 0;
        x -= k;
    }
    void move_right()
    {
        int k;
        if(x <= SCREEN_WIDTH - 100)
        k = speed_x;
        else k = 0;
        x += k;
    }
    void auto_move(int &score)
    {

        if(x <= -100)
        x = SCREEN_WIDTH;
        x -= 20;
        if(score >= 1000&&score < 2000 && (x>=SCREEN_WIDTH || x <= -100))
        x -= 25;
        else if(score > 2000 && (x>=SCREEN_WIDTH || x <= -100))
        x -= 30;
    }
};
bool init()
{
    bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialisze! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if(!( IMG_Init( imgFlags ) & imgFlags ))
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
	}
	return success;
}

void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    for(int i = 0; i < 5; i++)
    {
        gSpriteSheetTexture[i].free();
    }
    gBackgroundTexture.free();
    IMG_Quit();
    SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0 &&(e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
        return;
        SDL_Delay(0);
    }
}
void rng(nhan_vat &k,nhan_vat enemy[])
{
    if(k.x <= -100)
    {
        k = enemy[rand()%4];
        k.x = SCREEN_WIDTH + rand()%401 + 300;
    }
    if(k.x >= SCREEN_WIDTH)
    {
        k = enemy[rand()%4];
    }
    k.auto_move(score);
    k.animation(7);
}
int main(int argc, char* argv[])
{
    srand(time(NULL));
    init();
    loadMedia();
    SDL_Event e;
    nhan_vat no_1(0, 100, 470,20, 20,0);
    nhan_vat no_2(1, SCREEN_WIDTH - 100, (SCREEN_HEIGHT-100)/2, 20,0, 0);
    nhan_vat enemy[4];
    enemy[0] = no_2;
    for(int i = 1; i < 4; i++)
    {
        enemy[i].index = i+1;
    }
    nhan_vat k;
    bool quit = false;
    int frame_1 = 0, frame_2 = 0;
    while(quit == false )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_UP:
                    {
                        no_1.speed_y = 35;
                        while(no_1.speed_y > 0)
                        {
                            no_1.y -= no_1.speed_y;
                            no_1.speed_y -= 5;
                            gBackgroundTexture.render(0,0,NULL);
                            rng(k,enemy);
                            no_1.animation( 7);
                            SDL_Delay(50);
                            score += 1;
                            cout<<score<<endl;
                        }
                        while(no_1.y < 470)
                        {
                            no_1.speed_y += 5;
                            no_1.y += no_1.speed_y;
                            gBackgroundTexture.render(0,0,NULL);
                            rng(k,enemy);
                            no_1.animation( 7);
                            SDL_Delay(50);
                            score += 1;
                            cout<<score<<endl;
                        }
                        break;
                    }
                 /*   case SDLK_RIGHT:
                    {
                        no_1.move_right();
                        gBackgroundTexture.render(0,0,NULL);
                        rng(k,enemy);
                        no_1.animation(7);
                        SDL_Delay(50);
                        score++;
                        cout<<score<<endl;
                        break;
                    }
                    case SDLK_LEFT:
                    {
                        no_1.move_left();
                        gBackgroundTexture.render(0,0,NULL);
                        rng(k,enemy);
                        no_1.animation(7);
                        SDL_Delay(50);
                        score++;
                        cout<<score<<endl;
                        break;
                    }
                    case SDLK_SPACE:
                    {
                        quit = true;
                        break;
                    }
                    default:
                    break;
                }


            }


        }
        score+= 1;
        cout<<score<<endl;
        gBackgroundTexture.render(0,0,NULL);
        rng(k,enemy);
        no_1.animation(7);
        SDL_Delay(50);
        if( ( ( k.x >= no_1.x && k.x <( no_1.x+100 ) ) ||( k.x <=no_1.x && ( k.x+100 ) > no_1.x ) ) && ( no_1.y <= k.y && no_1.y+100 >= k.y ) )
            {
                quit = true;
            }
    }
    waitUntilKeyPressed();
    close();
    return 0;
}
*/
