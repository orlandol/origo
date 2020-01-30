#include "criterr.h"
#include "config.h"
#include "bytegen.h"
#include "bytevm.h"
#include "ui.h"

#include "Windows.h"

#include <stdio.h>

int main( int argc, char* argv[] ) {
  App editApp = CreateApp("Origo", 0, 0);

  while( AppIsOpen(editApp) ) {
    HandleMessages( editApp );
    DrawApp( editApp );
  }

  FreeApp( &editApp );

  return 0;
}
