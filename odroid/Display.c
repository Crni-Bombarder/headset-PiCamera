#include "Display.h"

SDL_Window* screen;
SDL_Renderer* renderer;
SDL_Texture* texture;

void createWindow(int x, int y, int xt, int yt)
{
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, x, y, 0);
    renderer = SDL_CreateRenderer(screen, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, xt, yt);
}

void displayFrame(Frame* frame)
{
    SDL_UpdateTexture(texture, NULL, frame->data, frame->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void resizeWindow(int x, int y)
{
    SDL_SetWindowSize(screen, x, y);
}

void resizeTexture(int xt, int yt)
{
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, xt, yt);
}

void destroyWindow(void)
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}
