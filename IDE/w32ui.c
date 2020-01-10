
#include <stdlib.h>
#include <stdbool.h>

#include "ui.h"

#include "Windows.h"

/*
 * Console state declarations
 */

  typedef struct Win32ConsoleState {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    HANDLE buffer;
    DWORD mode;
  } Win32ConsoleState;

  Win32ConsoleState* AllocConsoleState();
  void FreeConsoleState( Win32ConsoleState** consoleState );

  bool SaveConsoleState( Win32ConsoleState* consoleState, HANDLE consoleHandle );
  void RestoreConsoleState( Win32ConsoleState* consoleState );

/*
 * Console state implementation
 */

  Win32ConsoleState* AllocConsoleState() {
    return calloc(1, sizeof(Win32ConsoleState));
  }

  void FreeConsoleState( Win32ConsoleState** consoleState ) {
    if( consoleState ) {
      if( (*consoleState) ) {
        if( (*consoleState)->buffer ) {
          CloseHandle( (*consoleState)->buffer );
          (*consoleState)->buffer = NULL;
        }

        free( (*consoleState) );
        (*consoleState) = NULL;
      }
    }
  }

  bool SaveConsoleState( Win32ConsoleState* consoleState,
      HANDLE consoleHandle ) {
    return NULL;
  }

  void RestoreConsoleState( Win32ConsoleState* consoleState ) {
  }

/*
 * App internals
 */
  typedef struct Win32App {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    HANDLE buffer;
  } Win32App;

  Win32ConsoleState* oldStdOut = NULL;

/*
 *  App implementation
 */
  App CreateApp( char* title, unsigned width, unsigned height ) {
    Win32App* newApp = NULL;

    newApp = calloc(1, sizeof(Win32App));

    ///TODO: Save system console state once

    return (App)newApp;
  }

  void FreeApp( App* app ) {
    Win32App* w32App = NULL;

    if( app ) {
      w32App = (*app);

      if( w32App ) {
        if( w32App->buffer ) {
          CloseHandle( w32App->buffer );
          w32App->buffer = NULL;
        }

        free( (*app) );
        (*app) = NULL;
      }
    }
  }

  void ExitApp( App app ) {
    ///TODO: Restore system console state
  }
