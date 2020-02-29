
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
 *  App implementation
 */

  // App color variables
  unsigned colorAppBackground = 0;

  // App state
  bool InitializeApp( char* title ) {
    return false;
  }

  void FreeResources() {
  }

  bool AppIsRunning() {
    return false;
  }

  void ExitApp( unsigned exitCode ) {
    DoneConsole();

    FreeResources();

    exit( exitCode );
  }

  // App UI
  void DrawApp() {
  }

  // App events
  void HandleMessages() {
  }

/*
 *  Control implementation
 */

/*
 *  MenuBar implementation
 */

/*
 *  View implementation
 */
