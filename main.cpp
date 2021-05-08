#include <SDL_image.h>
#include <SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <SDL_mixer.h>
using namespace std;
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Texture* gTexture;
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
        //Deallocates texture
        void free();

        void render( int x, int y, SDL_Rect* clip = NULL);
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
LTexture gBackgroundTexture;
LTexture gBackgroundPause;
LTexture gmenu;
LTexture ginstruction;
LTexture play_again;
LTexture high_score;
SDL_Rect gSpriteClips[5][7];
LTexture gSpriteSheetTexture[5];
//The music that will be played
Mix_Music *gMusic = NULL;

//The sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

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
bool LTexture::loadFromFile(string path )
{
    free();
    SDL_Texture* newTexture = NULL;
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
void LTexture::render( int x, int y, SDL_Rect * clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip,&renderQuad );
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
    SDL_Rect* current_clip;
    //LTexture gSpriteSheetTexture;
    nhan_vat(int _d = 0,int _x = SCREEN_WIDTH, int _y = 470, int sp_x = 0, int sp_x_1 = 0, int sp_y = 0, int _f = 0, SDL_Rect* tmp = NULL)
    {
        index = _d;
        x = _x;
        y = _y;
        speed_x = sp_x;
        speed_x_1 = sp_x_1;
        speed_y = sp_y;
        frame = _f;
        current_clip = tmp;
    }
    void animation(int n)
    {
        if(frame == n)
        frame = 0;
        current_clip = &gSpriteClips[index][frame];
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
        animation(7);
    }
};
bool init()
{
    bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 | (SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialisze! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{

	    //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC );
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
                //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100 , NULL , 2 , 1024 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}

            }

        }
	}
	return success;
}

bool loadMedia()
{
    bool success = true;
    //Load sprite sheet texture
    if(!gSpriteSheetTexture[0].loadFromFile("dino.png"))
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
    if( !gSpriteSheetTexture[1].loadFromFile( "bat.png" ) )
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
    if( !gSpriteSheetTexture[2].loadFromFile( "cactus2.png" ) )
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

    if( !gSpriteSheetTexture[3].loadFromFile( "cactus3.png" ) )
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
    if( !gSpriteSheetTexture[4].loadFromFile( "cactus4.png" ) )
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
    if( !gBackgroundTexture.loadFromFile( "background.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    if( !gBackgroundPause.loadFromFile( "background_pause.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }

    if( !gmenu.loadFromFile( "menu.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !ginstruction.loadFromFile( "mouse_instruction.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !play_again.loadFromFile( "play_again.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
    if( !high_score.loadFromFile( "high.jpg" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }


    //Load music
	gMusic = Mix_LoadMUS( "sound_bkgr_audio.wav" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV( "sound_jump_sound.wav" );
	if( gScratch == NULL )
	{
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gHigh = Mix_LoadWAV( "sound_lose_sound.wav" );
	if( gHigh == NULL )
	{
		printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gMedium = Mix_LoadWAV( "sound_menu_audio.wav" );
	if( gMedium == NULL )
	{
		printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	gLow = Mix_LoadWAV( "sound_mouse_click.wav" );
	if( gLow == NULL )
	{
		printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
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
    gBackgroundPause.free();
    gmenu.free();
    ginstruction.free();
    play_again.free();
    high_score.free();



    //Free the sound effects
	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

    IMG_Quit();
    SDL_Quit();
}
void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true)
    {
        if ( SDL_WaitEvent(&e) != 0)
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
}

bool mouse_play( int x,int y )
{
    if( x >560 && x<680 && y>300 && y<350 )
    {
        return true;
    }
    return false;
}
bool mouse_exit(int x,int y)
{
    if( x > 420 && x<550 && y > 450 && y<500 )
    {
        return true;
    }
    return false;
}
bool mouse_instruction(int x,int y)
{
    if( x >685 && x<810 && y>455 && y<505 )
    {
        return true;
    }
    return false;
}
bool mouse_back(int x,int y)
{
    if( x >0 && x<145 && y>0 && y<70 )
    {
        return true;
    }
    return false;
}
bool mouse_pause(int x,int y)
{
    if( x>935 && x < 965 && y>10 && y<55 )
    {
        return true;
    }
    return false;
}
bool mouse_again_exit(int x,int y)
{
    if( x>675 && x < 915 && y> 455 && y< 560 )
    {
        return true;
    }
    return false;
}
bool mouse_again(int x,int y)
{
    if( x>355 && x < 570 && y>450 && y<560 )
    {
        return true;
    }
    return false;
}
bool mouse_high(int x,int y)
{
    if( x > 560 && x < 680 && y > 385 && y < 435 )
    {
        return true;
    }
    return false;
}

void render_menu( bool &quit, bool &is_instruction, bool &is_menu,bool &is_play,bool &high_sc,SDL_Event &e, int &mou_x,int &mou_y  )
{
    //Mix_PlayMusic( gMusic, -1 );
    while( quit==false && ( is_instruction == true || is_menu== true || is_play == true || high_sc == true )  )
    {

            if( is_menu== true )
                {

                    gmenu.render(0,0,NULL);
                    SDL_RenderPresent(gRenderer);
                }
            if( is_instruction==true)
            {
                ginstruction.render(0,0,NULL);
                SDL_RenderPresent(gRenderer);
            }
            if( high_sc == true )
            {
                high_score.render(0,0,NULL);
                SDL_RenderPresent(gRenderer);
            }
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type ==SDL_QUIT   )
            {
                quit=true;
            }
            if( e.type == SDL_MOUSEBUTTONDOWN  )
            {

                SDL_GetMouseState( &mou_x,&mou_y );
                //cout << mou_x << " " << mou_y << endl;
                if(e.button.button == SDL_BUTTON_LEFT && mouse_play(mou_x,mou_y)==true)
                {
                    is_play=true;
                    quit = true;
                    is_menu = false;
                    is_instruction = false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_instruction(mou_x,mou_y)==true )
                {
                    is_instruction = true;
                    is_menu = false;
                    is_play = false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_exit(mou_x,mou_y)==true )
                {
                    quit=true;
                    is_play = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_back(mou_x,mou_y) ==true && is_instruction==true )
                {
                    is_instruction=false;
                    is_menu= true;
                    is_play=false;
                    high_sc = false;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_high(mou_x,mou_y) ==true  )
                {
                    is_instruction=false;
                    is_menu= false;
                    is_play=false;
                    high_sc = true;
                }
                if( e.button.button == SDL_BUTTON_LEFT && mouse_back(mou_x,mou_y) ==true && high_sc==true )
                {
                    is_instruction=false;
                    is_menu= true;
                    is_play=false;
                    high_sc = false;
                }
            }
    }
    }
}
void play_game( bool &quit,bool &is_play,SDL_Event &e, int &mou_x,int &mou_y ,bool pause, nhan_vat &no_1 , nhan_vat &k,nhan_vat enemy[],
                bool &is_menu,bool &is_instruction,bool &high_sc  )
{
    while(quit == false )
    {
        int h=0,t=0;
        while( is_play==true  )
        {
            while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }


            if( e.type == SDL_MOUSEBUTTONDOWN  )
            {
                SDL_GetMouseState( &mou_x,&mou_y );
                //cout << mou_x << " " << mou_y << endl;
                if(e.button.button == SDL_BUTTON_LEFT && mouse_pause(mou_x,mou_y)==true)
                {
                    if( pause == true )
                    {
                        pause = false;
                    }
                    else
                    {
                        pause = true;
                    }
                }

            }

            if(e.type == SDL_KEYDOWN )
            {
                switch(e.key.keysym.sym)
                {

                    case SDLK_UP:
                    {
                        no_1.speed_y = 40;
                        while(no_1.speed_y > 0)
                        {
                            if( pause == false )
                            {

                                no_1.y -= no_1.speed_y;
                                no_1.speed_y -= 7;
                                gBackgroundTexture.render(0,0,NULL);
                                rng(k,enemy);
                                no_1.animation( 7);
                                SDL_Delay(50);
                           // cout<<no_1.x<<" "<<no_1.y<<endl;
                            }
                            else if( pause == true)
                            {
                                Mix_PauseMusic();
                                gBackgroundPause.render(0,0,NULL);
                                gSpriteSheetTexture[no_1.index].render(no_1.x, no_1.y, no_1.current_clip);
                                gSpriteSheetTexture[k.index].render(k.x, k.y, k.current_clip);
                                SDL_RenderPresent(gRenderer);
                            }

                        }
                        while(no_1.y < 470)
                        {
                            if( pause == false )
                            {
                                no_1.speed_y += 7;
                                no_1.y += no_1.speed_y;
                                gBackgroundTexture.render(0,0,NULL);
                                rng(k,enemy);
                                no_1.animation(7);
                                SDL_Delay(50);
                           // cout<<no_1.x<<" "<<no_1.y<<endl;
                        }
                        else if( pause == true )
                        {
                           // Mix_PauseMusic();
                            gBackgroundPause.render(0,0,NULL);

                            gSpriteSheetTexture[no_1.index].render(no_1.x, no_1.y, no_1.current_clip);
                            gSpriteSheetTexture[k.index].render(k.x, k.y, k.current_clip);

                            SDL_RenderPresent(gRenderer);
                        }
                            }

                        break;
                    }
                   //left& right
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
        if( pause == false)
        {
            //cout<<no_1.x<<" "<<no_1.y<<endl;
            gBackgroundTexture.render(0,0,NULL);
            rng(k,enemy);
            no_1.animation(7);
            SDL_Delay(50);
        }
         else if( pause== true )
        {
            Mix_PauseMusic();
            gBackgroundPause.render(0,0,NULL);
            gSpriteSheetTexture[no_1.index].render(no_1.x, no_1.y, no_1.current_clip);
            gSpriteSheetTexture[k.index].render(k.x, k.y, k.current_clip);
            SDL_RenderPresent(gRenderer);

        }

        // conndition game over
        if( ( ( k.x >= no_1.x && k.x <( no_1.x+100 ) ) ||( k.x <=no_1.x && ( k.x+100 ) > no_1.x ) ) && ( no_1.y <= k.y && no_1.y+100 >= k.y ) )
        {
            is_play=false;
        }
        }

        SDL_Delay(1000);
        while(is_play==false && quit==false )
        {
            play_again.render(0,0,NULL);
            SDL_RenderPresent(gRenderer);
            while( SDL_PollEvent( &e ) != 0 )
            {
            if( e.type == SDL_MOUSEBUTTONDOWN  )
            {
                SDL_GetMouseState( &mou_x,&mou_y );
                //cout << mou_x << " " << mou_y << endl;
                if(e.button.button == SDL_BUTTON_LEFT && mouse_again_exit(mou_x,mou_y)==true)
                {
                    quit = true;
                }
                if(e.button.button == SDL_BUTTON_LEFT && mouse_again(mou_x,mou_y)==true)
                {
                    is_play = true;
                    quit = false;
                    // set up lai vi tri cho cnv va diem so
                    k.x=1200;
                }
            }

            }
        }
    }
}
int main(int argc, char* argv[])
{
    srand(time(NULL));
    init();
    loadMedia();
    SDL_Event e;
    nhan_vat no_1(0, 200, 470,20, 20,0);
    nhan_vat no_2(1, SCREEN_WIDTH - 100, (SCREEN_HEIGHT-100)/2 + 200, 20,0, 0);
    nhan_vat enemy[4];
    enemy[0] = no_2;
    for(int i = 1; i < 4; i++)
    {
        enemy[i].index = i+1;
    }
    nhan_vat k;
    bool quit = false;
    bool is_play = false;
    bool is_instruction = false;
    bool is_menu=true;
    int mou_x,mou_y;
    bool pause= false;
    bool high_sc=false;
    while( true )
    {
        render_menu( quit,is_instruction,is_menu,is_play,high_sc, e,mou_x,mou_y );
        if( is_play == false && quit == true )
        {
            break;
        }
        if( is_play == true && quit == true )
        {
            quit = false;
            play_game(quit,is_play,e,mou_x,mou_y,pause,no_1,k,enemy,is_menu,is_instruction,high_sc);
            is_instruction = false;
            is_menu = true;
            is_play= false;
            high_sc = false;
            quit = false;
            k.x = 1200;
        }
    }
    waitUntilKeyPressed();
    close();
    return 0;
}
