#ifndef H_UI
#define H_UI

#include <stdbool.h>
#include <stdlib.h>

/*
 *  App declarations
 */

  typedef struct KeyMessage {
    unsigned code;
  } KeyMessage;

  typedef struct MouseMessage {
    int x;
    int y;
    unsigned button;
  } MouseMessage;

  typedef struct Message {
    unsigned type;

    union {
      KeyMessage key;
      MouseMessage mouse;
    } of;
  } Message;

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
 *  Control base declarations
 */

  typedef struct Control {
    size_t typeSize;
    unsigned id;
    int x1;
    int y1;
    int x2;
    int y2;

    // Virtual methods
    void (*onDraw)( struct Control* control );
  } Control;

/*
 *  Button declarations
 */

  typedef struct Button {
    size_t typeSize;
    unsigned id;
    int x1;
    int y1;
    int x2;
    int y2;

    // Virtual methods
    void (*onDraw)( struct Control* control );
  } Button;

  Button* CreateButton( int x1, int y1, int x2, int y2, char* caption );

  void DrawButton( Control* button );

/*
 *  MenuBar base declarations
 */

  typedef struct ControlBar {
    size_t typeSize;
    unsigned id;
    int x1;
    int y1;
    int x2;
    int y2;
    Control* control;
    size_t controlCount;

    // Virtual methods
    void (*onDraw)( struct ControlBar* controlBar );
  } ControlBar;

/*
 *  View base declarations
 */

  typedef struct View {
    size_t typeSize;
    unsigned id;
    int x1;
    int y1;
    int x2;
    int y2;

    // Virtual methods
    void (*onDraw)( struct View* view );
  } View;

#endif
