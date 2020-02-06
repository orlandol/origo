#ifndef H_UI
#define H_UI

#include <stdbool.h>
#include <stdlib.h>

/*
 *  App declarations
 */
  typedef void* PAppImpl;
  typedef PAppImpl App;

  App CreateApp( char* title, unsigned width, unsigned height );
  void FreeApp( App* appPtr );

  bool AppIsOpen( App app );
  void DrawApp( App app );
  void HandleMessages( App app );

  void ExitApp( App app, unsigned exitCode );

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
