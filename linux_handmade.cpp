#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

  LRESULT CALLBACK
MainWindowCallback(HWND hWnd,
    UINT uMSG,
    WPARAM wParam,
    LPARAM lParam)
{

  LRESULT Result = 0;

  {
    switch (uMSG)
    {

      case WM_SIZE:
        {
          OutputDebugString(TEXT("WM_SIZE\n"));
        } break;

      case WM_DESTROY:
        {
          OutputDebugString(TEXT("WM_DESTROY\n"));
        } break;

      case WM_CLOSE:
        {
          OutputDebugString(TEXT("WM_CLOSE\n"));
        } break;

      case WM_ACTIVATEAPP:
        {
          OutputDebugString(TEXT("WM_ACTIVATEAPP\n"));
        } break;

      case WM_PAINT:
        static DWORD PaintOperation;
        {
          PAINTSTRUCT Paint;
          HDC DeviceContext = BeginPaint(hWnd, &Paint);
          int x = Paint.rcPaint.left;
          int y = Paint.rcPaint.top;
          int h = Paint.rcPaint.bottom - y;
          int w = Paint.rcPaint.right - x;
          PaintOperation = (PaintOperation == WHITENESS ? BLACKNESS : WHITENESS);
          PatBlt(DeviceContext, x, y, w, h, PaintOperation);
          EndPaint(hWnd, &Paint);
        } break;

      default:
        {
          Result = DefWindowProc(hWnd, uMSG, wParam, lParam);
        } break;

    }
  }

  return(Result);
}

  int CALLBACK
WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
  // GetModuleHandle(0);
  // MessageBox(0, TEXT("hello, nsa"), TEXT("helloer"), MB_OK|MB_ICONINFORMATION);

  WNDCLASS WindowClass = {};
  WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc = MainWindowCallback;
  WindowClass.hInstance = hInstance;
  WindowClass.lpszClassName = TEXT("HandmadeHeroWindowClass");

  if (RegisterClass(&WindowClass))
  {
    HWND WindowHandle = CreateWindowEx(
        0,
        WindowClass.lpszClassName,
        TEXT("Handmade Hero"),
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);
    if (WindowHandle)
    {
      while (true)
      {
        MSG Message;
        BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
        if (MessageResult > 0)
        {
          TranslateMessage(&Message);
          DispatchMessage(&Message);
        }
        else
        {
          break;
        }
      }

    }
    else
    {
      // Logging
    }
  }
  else
  {
    // Future Logging
  }

  return(0);
}
#endif

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
