
#include <stdlib.h>
#include <stdbool.h>

#include "ui.h"

#include "Windows.h"

/*
 * Console state implementation
 */

  typedef struct Win32ConsoleState {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    HANDLE buffer;
    DWORD mode;
  } Win32ConsoleState;

  Win32ConsoleState* SaveState( HANDLE consoleHandle ) {
    return NULL;
  }

  void FreeState( Win32ConsoleState** consoleState ) {
  }

  bool RestoreState( Win32ConsoleState* consoleState ) {
    return false;
  }

/*
 * App internals
 */
  typedef struct Win32App {
    int foo;
  } Win32App;

/*
 *  App implementation
 */
  App CreateApp( char* title, unsigned width, unsigned height ) {
    Win32App* newApp = NULL;

    newApp = calloc(1, sizeof(Win32App));

    return (App)newApp;
  }

  void FreeApp( App* app ) {
    if( app ) {
      if( (*app) ) {
        free( (*app) );
        (*app) = NULL;
      }
    }
  }

  void ExitApp( App app ) {
  }
