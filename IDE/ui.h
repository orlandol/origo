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

  typedef void (*DrawControl)( Control* control );

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
    uint8_t fillColor;
    uint8_t focusColor;
    uint8_t textColor;
    uint8_t hotkeyColor;
  } ControlBar;

  typedef void (*DrawControlBar)( ControlBar* controlBar );

/*
 *  View declarations
 */

  typedef struct View {
    int x1;
    int y1;
    int x2;
    int y2;
  } View;

  typedef void (*DrawView)( View* view );

#endif
