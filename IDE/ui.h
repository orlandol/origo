#ifndef H_UI
#define H_UI

/*
 *  App declarations
 */
  typedef void* App;

  App CreateApp( char* title, unsigned width, unsigned height );
  void FreeApp( App* app );

  void ExitApp( App app );

#endif
