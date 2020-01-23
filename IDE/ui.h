#ifndef H_UI
#define H_UI

/*
 *  App declarations
 */
  typedef void* PAppImpl;
  typedef PAppImpl App;

  App CreateApp( char* title, unsigned width, unsigned height );
  void FreeApp( App* appPtr );

  void ExitApp( App app, unsigned exitCode );

#endif
