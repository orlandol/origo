#ifndef H_UI
#define H_UI

#include <stdbool.h>
#include <stdlib.h>

/*
 *  App declarations
 */

  // App color variables
  extern unsigned colorAppBackground;

  // App state
  bool InitializeApp( char* title );
  void FreeResources();

  bool AppIsRunning();

  void ExitApp( unsigned exitCode );

  // App UI
  void DrawApp();

  // App events
  void HandleMessages();

/*
 *  Control declarations
 */

  typedef struct Control {
    int x1;
    int y1;
    int x2;
    int y2;
  } Control;

  typedef void (*DrawControlFunc)( Control* control );

/*
 *  MenuBar declarations
 */

  typedef struct ControlBar {
    Control* control;
    size_t controlCount;
    int x1;
    int y1;
    int x2;
    int y2;
  } ControlBar;

  typedef void (*DrawControlBarFunc)( ControlBar* controlBar );

/*
 *  View declarations
 */

  typedef struct View {
    int x1;
    int y1;
    int x2;
    int y2;
  } View;

  typedef void (*DrawViewFunc)( View* view );
  typedef bool (*HandleMessageFunc)( View* view );

#endif
