#include <SDL.h>
#include <time.h>
#include <iostream>

bool game_running = true;
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_GameController *controller;
int texture_w;
int texture_h;
int pitch;
void *pixel_memory;
SDL_AudioSpec wanted;

void audio_init(Uint32 samples_per_second, Uint32 buffer_size)
{
  SDL_AudioSpec audio_settings = {0};
  audio_settings.freq = samples_per_second;
  audio_settings.format = AUDIO_S16;
  audio_settings.channels = 2;    /* 1 = mono, 2 = stereo */
  audio_settings.samples = buffer_size;

  /* Open the audio device, forcing the desired format */
  if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
  }
  else 
  {
    printf("Audio systems operational.\n");
  }
}

#define PI 3.14159265

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
    int ymess = j - (texture_h / 2);
    pixel = (Uint32 *)((Uint8 *) pixel_memory + j * pitch);
    for (int i = 0; i < texture_w; ++i) {
      int xmess = i - (texture_w / 2);

      Uint8 alpha = 255;
      Uint8 red = sin(j*PI/180);
      Uint8 green = 0;
      Uint8 blue = sin(i*PI/180);

      // *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | (blue));
      *pixel++ = (ymess * xmess) & 5 * 128;

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

  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i))
    {
      controller = SDL_GameControllerOpen(0);
      if (controller) {
        printf("Controller open and ready!\n");
      } else {
        printf("No controllers detected.\n");
      }
    }
  }

  int SamplesPerSecond = 48000;
  int ToneHz = 256;
  Uint16 ToneVolume = 3000;
  Uint32 RunningSampleIndex = 0;
  int wave_period = SamplesPerSecond / ToneHz;
  int BytesPerSample = sizeof(Uint16) * 2;
  audio_init(SamplesPerSecond, SamplesPerSecond * BytesPerSample / 60);
  bool sound_is_playing = false;
  int sound_offset = 0;
  Uint64 performance_count_freq = SDL_GetPerformanceFrequency();
  Uint64 last_counter = SDL_GetPerformanceCounter();

  while (game_running)
  {

    SDL_GetWindowSize(window, &texture_w, &texture_h);
    render(x_offset, y_offset);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, 0, 0);
    SDL_RenderPresent(renderer);

    int TargetQueueBytes = SamplesPerSecond * BytesPerSample;
    int BytesToWrite = TargetQueueBytes - SDL_GetQueuedAudioSize(1);
    if (BytesToWrite)
    {
      void *SoundBuffer = malloc(BytesToWrite);
      Uint16 *SampleOut = (Uint16 *)SoundBuffer;
      int SampleCount = BytesToWrite/BytesPerSample;
      for(int SampleIndex = 0;
          SampleIndex < SampleCount;
          ++SampleIndex)
      {
        float t = 2.0f * PI * RunningSampleIndex / (float) wave_period;
        Uint16 SampleValue = (Uint16) (sinf(t) * ToneVolume);
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;
        ++RunningSampleIndex;
      }
      SDL_QueueAudio(1, SoundBuffer, BytesToWrite);
      free(SoundBuffer);
    }

    if(!sound_is_playing)
    {
      SDL_PauseAudio(0);
      sound_is_playing = true;
    }


    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type)
      {
        case SDL_QUIT:
          {
            printf("shutdown!\n");
            game_running = 0;
          } break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
          {
            SDL_Keycode keycode = event.key.keysym.sym;
            printf("Got keycode: %d\n", keycode);
            // if (!event.key.repeat) {
            if (keycode == SDLK_d) {
              x_offset += 10;
            }
            if (keycode == SDLK_a) {
              x_offset -= 10;
            }
            if (keycode == SDLK_w) {
              y_offset -= 10;
            }
            if (keycode == SDLK_s) {
              y_offset += 10;
            }
            // }
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
          } break; // case SDL_WINDOWEVENT
      } // switch (event.type)
    } // event loop

    Uint64 end_counter = SDL_GetPerformanceCounter();
    Uint32 delta_counter = (Uint32) end_counter - last_counter;
    Uint32 ms_per_counter = ((delta_counter * 1000) / performance_count_freq);
    Uint32 fps = performance_count_freq / delta_counter;
    printf("ms per counter: %d (delta_counter: %d); frames per second: %d\n", ms_per_counter, delta_counter, fps);
    last_counter = end_counter;

  } // game loop

  SDL_DestroyWindow(window);
  SDL_Quit();
  return(0);
}
