
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int logSDLError(char* error);
int init_SDL_all();
SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);
SDL_Texture* renderText(char* message, TTF_Font* font, SDL_Color color, SDL_Renderer *renderer);
