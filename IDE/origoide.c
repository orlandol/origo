#include "criterr.h"
#include "config.h"
#include "bytegen.h"
#include "bytevm.h"
#include "ui.h"

#include "Windows.h"

#include <stdio.h>

int main( int argc, char* argv[] ) {

  InitializeApp( "Origo" );

  while( AppIsRunning() ) {
    DrawApp();
    HandleMessages();
  }

  FreeResources();

  return 0;
}
