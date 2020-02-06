
#include <stdlib.h>
#include <stdbool.h>

#include "ui.h"

#include "Windows.h"

/*
 * System console internals
 */

  HANDLE stdInHandle = INVALID_HANDLE_VALUE;
  HANDLE stdOutHandle = INVALID_HANDLE_VALUE;
  HANDLE stdErrHandle = INVALID_HANDLE_VALUE;

  DWORD stdInMode = 0;
  DWORD stdOutMode = 0;
  DWORD stdErrMode = 0;

  bool InitConsole() {
    ///TODO: Install default critical error handlers

    // Cache stdin handle
    if( stdInHandle == INVALID_HANDLE_VALUE ) {
      stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
      if( stdInHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }

      if( GetConsoleMode(stdInHandle, &stdInMode) == FALSE ) {
        return false;
      }

      ///TODO: Save stdin buffer?
    }

    // Cache stdout handle
    if( stdOutHandle == INVALID_HANDLE_VALUE ) {
      stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if( stdOutHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }

      if( GetConsoleMode(stdOutHandle, &stdOutMode) == FALSE ) {
        return false;
      }

      ///TODO: Save stdout buffer?
    }

    // Cache stderr handle
    if( stdErrHandle == INVALID_HANDLE_VALUE ) {
      stdErrHandle = GetStdHandle(STD_ERROR_HANDLE);
      if( stdErrHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }

      if( GetConsoleMode(stdErrHandle, &stdErrMode) == FALSE ) {
        return false;
      }
    }

    return true;
  }

  void DoneConsole() {
    SetConsoleActiveScreenBuffer( stdOutHandle );

    SetConsoleMode( stdInHandle, stdInMode );
    SetConsoleMode( stdOutHandle, stdOutMode );
    SetConsoleMode( stdErrHandle, stdErrMode );

    ///TODO: Close standard console handles(?)
  }

/*
 * App internals
 */
  typedef struct Win32App {
    CONSOLE_SCREEN_BUFFER_INFO bufferOutInfo;
    HANDLE outBuffer;
    unsigned width;
    unsigned height;
    ControlBar controlBar;
    DrawControlBarFunc drawControlBar;
    View workView;
    DrawViewFunc drawView;
    HandleMessageFunc handleMessage;
  } Win32App;

/*
 *  App implementation
 */

  App CreateApp( char* title, unsigned width, unsigned height ) {
    Win32App* newApp = NULL;
    unsigned appError = 0;
    CONSOLE_SCREEN_BUFFER_INFO stdOutInfo = {};
    COORD appDimensions = {};

    ///TODO: Install app specific critical error handlers

    // Save system console state once
    if( InitConsole() == false ) {
      appError = 1;
      goto ReturnError;
    }

    // Allocate app state
    newApp = calloc(1, sizeof(Win32App));
    if( newApp == NULL ) {
      appError = 2;
      goto ReturnError;
    }

    // Create new display buffer
    newApp->outBuffer = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL,
      CONSOLE_TEXTMODE_BUFFER,
      NULL
    );
    if( newApp->outBuffer == INVALID_HANDLE_VALUE ) {
      appError = 3;
      goto ReturnError;
    }

    // Resize new display buffer to console dimensions
    if( GetConsoleScreenBufferInfo(stdOutHandle, &stdOutInfo) == 0 ) {
      appError = 4;
      goto ReturnError;
    }
    appDimensions.X = (stdOutInfo.srWindow.Right
        - stdOutInfo.srWindow.Left) + 1;
    appDimensions.Y = (stdOutInfo.srWindow.Bottom
        - stdOutInfo.srWindow.Top) + 1;

    newApp->width = appDimensions.X;
    newApp->height = appDimensions.Y;

    if( SetConsoleScreenBufferSize(newApp->outBuffer,
        appDimensions) == 0 ) {
      appError = 5;
      goto ReturnError;
    }

    // Set new buffer as display buffer
    if( SetConsoleActiveScreenBuffer(newApp->outBuffer) == FALSE ) {
      appError = 6;
      goto ReturnError;
    }

    // Enable events for new buffer
    if( SetConsoleMode( stdInHandle, ENABLE_WINDOW_INPUT |
        ENABLE_MOUSE_INPUT ) == FALSE ) {
      appError = 7;
      goto ReturnError;
    }

    return (App)newApp;

  ReturnError:
    FreeApp( &newApp );

    DoneConsole();

    exit( appError );
    return NULL;
  }

  void FreeApp( App* appPtr ) {
    Win32App* w32App = NULL;

    if( appPtr ) {
      w32App = (*appPtr);

      if( w32App ) {
        if( w32App->outBuffer ) {
          CloseHandle( w32App->outBuffer );
          w32App->outBuffer = NULL;
        }

        free( (*appPtr) );
        (*appPtr) = NULL;
      }
    }
  }

  bool AppIsOpen( App app ) {
    return false;
  }

/*
  typedef struct Win32App {
    CONSOLE_SCREEN_BUFFER_INFO bufferOutInfo;
    HANDLE outBuffer;
    unsigned width;
    unsigned height;
    ControlBar controlBar;
    DrawControlBarFunc drawControlBar;
    View workView;
    DrawViewFunc drawView;
    HandleMessageFunc handleMessage;
  } Win32App;
*/

  void DrawApp( App app ) {
    Win32App* w32App = (Win32App*)app;

    if( w32App == NULL ) {
      return;
    }

    if( &(w32App->workView) && &(w32App->drawView) ) {
      w32App->drawView( &(w32App->workView) );
    }

    if( &(w32App->controlBar) && &(w32App->drawControlBar) ) {
      w32App->drawControlBar( &(w32App->controlBar) );
    }
  }

  void HandleMessages( App app ) {
    Win32App* w32App = (Win32App*)app;

    if( w32App == NULL ) {
      return;
    }

    if( &(w32App->workView) && &(w32App->handleMessage) ) {
      if( w32App->handleMessage(&(w32App->workView)) ) {
        return;
      }
    }

    if( &(w32App->controlBar) && &(w32App->handleMessage) ) {
      if( w32App->handleMessage(&(w32App->controlBar)) ) {
        return;
      }
    }
  }

  void ExitApp( App app, unsigned exitCode ) {
    DoneConsole();

    exit( exitCode );
  }
