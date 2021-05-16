#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cstring>
using namespace std;


class LTexture
{
    public:
        //Initializes variables
        LTexture();
        //Deallocates memory
        ~LTexture();
        //Loads image at specified path
        bool loadFromFile(string path, SDL_Renderer *gRenderer);
        //Deallocates texture
        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
        //#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText(string textureText,TTF_Font *gFont, SDL_Color textColor, SDL_Renderer *gRenderer);
		//#endif

        void render( SDL_Renderer* gRenderer,int x, int y, SDL_Rect* clip = NULL);
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

#endif // LTEXTURE_H

