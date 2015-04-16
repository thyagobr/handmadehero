#include <SDL.h>
#include <iostream>

bool game_running = true;
SDL_Renderer *renderer;
SDL_Texture *texture;
int texture_w;
int texture_h;
int pitch;
void *pixel_memory;

void render(int x_offset, int y_offset)
{
  if (texture)
  {
    SDL_DestroyTexture(texture);
  }
  
  texture = SDL_CreateTexture(renderer, 
      SDL_PIXELFORMAT_ARGB8888, 
      SDL_TEXTUREACCESS_STREAMING, 
      texture_w, 
      texture_h);

  if (SDL_LockTexture(texture, NULL, &pixel_memory, &pitch) < 0) {
    printf("Wtf? %s\n", SDL_GetError());
  }

  Uint32 *pixel;
  Uint8 *row = (Uint8 *) pixel_memory;
  // SDL_Color purple2 = 0x912CEE;
  for (int j = 0; j < texture_h; ++j) {
    pixel = (Uint32 *)((Uint8 *) pixel_memory + j * pitch);
    for (int i = 0; i < texture_w; ++i) {
      x_offset = 0;

      Uint8 alpha = 255;
      Uint8 red = j + y_offset;
      Uint8 green = 0;
      Uint8 blue = i + x_offset;

      *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | (blue));

    }
  }
  SDL_UnlockTexture(texture);
}

int main(int argc, char *argv[])
{
  int x_offset = 0;
  int y_offset = 0;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Hero", SDL_GetError(), 0);
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Handmade Penguin Hero",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      1280,
      720,
      SDL_WINDOW_RESIZABLE); 


  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);

  while (game_running)
  {

    SDL_GetWindowSize(window, &texture_w, &texture_h);
    render(x_offset, y_offset);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);

    ++x_offset;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
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
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return(0);
}
