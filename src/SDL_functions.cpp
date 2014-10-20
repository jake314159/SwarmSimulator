
#include "SDL_functions.h"

int logSDLError(char* error)
{
	printf("%s: %s\n", error, SDL_GetError());
    return 1;
}

int init_SDL_all()
{
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    //init image loader
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
	    logSDLError("IMG_Init");
	    return 1;
    }

    if (TTF_Init() != 0){
	    logSDLError("TTF_Init");
	    return 1;
    }
    return 0;
}

SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, file);
	if (texture == NULL)		
		logSDLError("LoadTexture");
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}


void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = clip->w;
    dst.h = clip->h;
	//Query the texture to get its width and height to use
	//SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, clip, &dst);
}

SDL_Texture* renderText(char* message, TTF_Font* font,
	SDL_Color color, SDL_Renderer *renderer)
{
	//Open the font
	/*TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}	*/

	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
	if (surf == NULL){
		TTF_CloseFont(font);
		logSDLError("TTF_RenderText");
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL){
		logSDLError("CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	//TTF_CloseFont(font);
	return texture;
}
