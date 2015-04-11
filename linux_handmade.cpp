#include <SDL.h>

int main(int argc, char *argv[])
{
  int game_running = 1;

  if (int sdl_init_error = (SDL_Init(SDL_INIT_VIDEO) != 0))
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Hero", SDL_GetError(), 0);
    return(sdl_init_error);
  }
  SDL_Window *window = SDL_CreateWindow("Handmade Penguin Hero",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_RESIZABLE); 

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);

  while (game_running)
  {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    // could also use SDL_PollEvent
    SDL_Event event;
    SDL_WaitEvent(&event);
    switch (event.type)
    {
      case SDL_QUIT:
        {
          printf("shutdown!\n");
          game_running = 0;
        } break;

      case SDL_WINDOWEVENT:
        {
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_RESIZED:
              {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                printf("SDL_WINDOWEVENT_RESIZED (%d, %d)\n", event.window.data1, event.window.data2);
              } break;
          }
        } break;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return(0);
}
