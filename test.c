
#include <stdio.h>

int main( int argc, char* argv[] ) {

  for( size_t length = 0; length < 25; length++ ) {
    printf( "(length:%u + 8) & (~7) = %u\n", length,
        (length + 8) & (~7) );
  }

  return 0;
}
