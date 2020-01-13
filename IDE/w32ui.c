
#include <stdlib.h>
#include <stdbool.h>

#include "ui.h"

#include "Windows.h"

/*
 * Console state declarations
 */

  typedef struct Win32ConsoleState {
    CONSOLE_SCREEN_BUFFER_INFO consoleOutInfo;
    HANDLE consoleOut;
    DWORD outMode;
    DWORD inMode;
  } Win32ConsoleState;

  Win32ConsoleState* AllocConsoleState();

  bool SaveConsoleState( Win32ConsoleState* consoleState,
      HANDLE consoleOut, HANDLE consoleIn );
  void RestoreConsoleState( Win32ConsoleState* consoleStatePtr );
  void FreeConsoleState( Win32ConsoleState* consoleState );

/*
 * Console state implementation
 */

  HANDLE stdInHandle = INVALID_HANDLE_VALUE;
  HANDLE stdOutHandle = INVALID_HANDLE_VALUE;
  HANDLE stdErrHandle = INVALID_HANDLE_VALUE;

  Win32ConsoleState stdOutStartState = {};

  bool SaveConsoleState( Win32ConsoleState* consoleState,
      HANDLE consoleOut, HANDLE consoleIn ) {
    Win32ConsoleState tempState = {};

    if( (consoleState == NULL) ||
        (consoleOut != INVALID_HANDLE_VALUE) ) {
      goto ReturnError;
    }

    if( GetConsoleMode(consoleOut, &tempState.outMode) == 0 ) {
      goto ReturnError;
    }

    if( consoleIn != INVALID_HANDLE_VALUE ) {
      if( GetConsoleMode(consoleIn, &tempState.inMode) == 0 ) {
        goto ReturnError;
      }
    }

    if( GetConsoleScreenBufferInfo(consoleOut,
        &tempState.consoleOutInfo) == 0 ) {
      goto ReturnError;
    }

    ///TEMP: Release previously allocated handle
    if( consoleState->consoleOut != INVALID_HANDLE_VALUE ) {
      CloseHandle( &consoleState->consoleOut );
      consoleState->consoleOut = INVALID_HANDLE_VALUE;
    }

    ///TODO: Release previous buffer if different size

    *consoleState = tempState;
    return true;

  ReturnError:
    if( consoleState &&
        (consoleState->consoleOut != INVALID_HANDLE_VALUE) ) {
      CloseHandle( consoleState->consoleOut );
    }
    return false;
  }

  void RestoreConsoleState( Win32ConsoleState* consoleState ) {
  }

  void FreeConsoleState( Win32ConsoleState* consoleState ) {
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

    // Cache stdin handle
    if( stdInHandle == INVALID_HANDLE_VALUE ) {
      stdOutHandle = GetStdHandle(STD_INPUT_HANDLE);
      if( stdInHandle == INVALID_HANDLE_VALUE ) {
        appError = 1;
        goto ReturnError;
      }
    }

    // Cache stdout handle
    if( stdOutHandle == INVALID_HANDLE_VALUE ) {
      stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if( stdOutHandle == INVALID_HANDLE_VALUE ) {
        appError = 2;
        goto ReturnError;
      }
    }

    // Cache stderr handle
    if( stdErrHandle == INVALID_HANDLE_VALUE ) {
      stdErrHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if( stdErrHandle == INVALID_HANDLE_VALUE ) {
        appError = 3;
        goto ReturnError;
      }
    }

    // Save console buffer, mode, etc to restore later
    newApp = calloc(1, sizeof(Win32App));
    if( newApp == NULL ) {
      appError = 4;
      goto ReturnError;
    }

    if( stdOutHandle != INVALID_HANDLE_VALUE ) {
      if( SaveConsoleState(&stdOutStartState,
          stdOutHandle, stdInHandle) == FALSE ) {
        appError = 5;
        goto ReturnError;
      }
    }

    ///TODO: Set app console state

    ///TODO: Initialize UI

    return (App)newApp;

  ReturnError:
    FreeConsoleState( &stdOutStartState );

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

  void ExitApp( App app, unsigned exitCode ) {
    RestoreConsoleState( &stdOutStartState );
    FreeConsoleState( &stdOutStartState );
    exit( exitCode );
  }
