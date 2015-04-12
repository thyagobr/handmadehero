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
  int texture_w, texture_h;
  SDL_GetWindowSize(window, &texture_w, &texture_h);
  SDL_Texture *texture = NULL;
  void *pixel_map = NULL;

  while (game_running)
  {

    // updating screen
    SDL_RenderClear(renderer);

    if (pixel_map) { 
      free(pixel_map); 
    }
    if (texture) {
      SDL_DestroyTexture(texture);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);
    pixel_map = malloc(texture_w * texture_h * 4);
    
    SDL_UpdateTexture(texture, 0, pixel_map, texture_w * 4);
    SDL_RenderCopy(renderer, texture, 0, 0);

    SDL_RenderPresent(renderer);


    // handling events
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
            case SDL_WINDOWEVENT_SIZE_CHANGED:
              {
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
