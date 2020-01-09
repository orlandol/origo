#include "criterr.h"
#include "config.h"
#include "bytegen.h"
#include "bytevm.h"
#include "ui.h"

int main( int argc, char* argv[] ) {
  App editApp = CreateApp("Origo", 0, 0);

  FreeApp( &editApp );

  return 0;
}
