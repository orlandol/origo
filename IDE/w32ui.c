
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
    // Cache stdin handle
    if( stdInHandle == INVALID_HANDLE_VALUE ) {
      stdOutHandle = GetStdHandle(STD_INPUT_HANDLE);
      if( stdInHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }
    }

    // Cache stdout handle
    if( stdOutHandle == INVALID_HANDLE_VALUE ) {
      stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if( stdOutHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }
    }

    // Cache stderr handle
    if( stdErrHandle == INVALID_HANDLE_VALUE ) {
      stdErrHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if( stdErrHandle == INVALID_HANDLE_VALUE ) {
        return false;
      }
    }

    ///TODO: Save console modes

    return true;
  }

/*
 * App internals
 */
  typedef struct Win32App {
    CONSOLE_SCREEN_BUFFER_INFO bufferOutInfo;
    HANDLE outBuffer;
  } Win32App;

/*
 *  App implementation
 */
  App CreateApp( char* title, unsigned width, unsigned height ) {
    Win32App* newApp = NULL;
    unsigned appError = 0;

    if( InitConsole() == false ) {
      appError = 1;
      goto ReturnError;
    }

    newApp = calloc(1, sizeof(Win32App));
    if( newApp == NULL ) {
      appError = 2;
      goto ReturnError;
    }

    newApp->outBuffer = CreateConsoleScreenBuffer(
      GENERIC_READ | GENERIC_WRITE | FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL,
      CONSOLE_TEXTMODE_BUFFER,
      NULL
    );
    if( newApp->outBuffer == INVALID_HANDLE_VALUE ) {
      appError = 3;
      goto ReturnError;
    }

    return (App)newApp;

  ReturnError:
    FreeApp( &newApp );

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

  void ExitApp( unsigned exitCode ) {
    SetConsoleActiveScreenBuffer( stdOutHandle );

    ///TODO: Close standard console handles(?)
    exit( exitCode );
  }
