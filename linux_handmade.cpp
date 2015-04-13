#include <SDL.h>

bool game_running = true;
SDL_Renderer *renderer;
SDL_Texture *texture;
int texture_w, texture_h;
int pitch;
void *pixel_memory;

void render(int x_offset, int y_offset)
{
  SDL_LockTexture(texture, NULL, &pixel_memory, &pitch);

  Uint32 *pixel;
  Uint8 *row = (Uint8 *) pixel_memory;
  // SDL_Color purple2 = 0x912CEE;
  for (int j = 0; j < texture_h; j++) {
    pixel = (Uint32 *) row;
    for (int i = 0; i < texture_w; i++) {

      Uint8 alpha = 0;
      Uint8 red = j + y_offset;
      Uint8 green = 0;
      Uint8 blue = i + x_offset;

      *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | (blue));

    }
    row += pitch;
  }

  SDL_UnlockTexture(texture);
}

int main(int argc, char *argv[])
{

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Handmade Hero", SDL_GetError(), 0);
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Handmade Penguin Hero",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      640,
      480,
      SDL_WINDOW_RESIZABLE); 

  SDL_GetWindowSize(window, &texture_w, &texture_h);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, texture_w, texture_h);

  int x_offset, y_offset = 0;

  while (game_running)
  {

    render(x_offset, y_offset);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);

    ++x_offset;

    // handling events
    // could also use SDL_PollEvent
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
                  SDL_GetWindowSize(window, &texture_w, &texture_h);
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
