#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

  typedef enum _Token {
    typeInt = 1024
  } Token;

  typedef struct _TypeSpec {
    // Optional name
    char*      name;
    // Optional Pointer or reference specifier
    unsigned   pointerType;
    // Required simple/complex type
    unsigned   baseType;
    size_t     typeSize;
    unsigned   typeCode;
    // Optional array specifier
    size_t     dimCount;
    unsigned*  dims;
    size_t     dimOfs;
    // Default value
    unsigned   initType;
    size_t     initSize;
    uint8_t*   initVal;
  } TypeSpec;

  typedef struct _VarSym {
    // Common Symbol Entry fields
    unsigned  symType;
    char*     name;
    // Global variable specifiers
    TypeSpec* typeSpec;
    size_t    dataOfs;
    // Optional initialized value
    unsigned  initType;
    size_t    initSize;
    uint8_t*  initVal;
  } VarSym;

int main( int argc, char* argv[] ) {
  avl_tree_t symTab;
  VarSym*    varEntry;

  avl_initialize( &symTab, (avl_comparator_t)strcmp, free );

  AVL_ALLOC( varEntry, VarSym );
  varEntry->symType = 0x1234;
  avl_insert( &symTab, strdup("foo"), varEntry );
  varEntry = NULL;

  varEntry = avl_search(&symTab, "foo");
  if( varEntry ) {
    printf( "[%x] GlobalVar name: %s\n",
      varEntry->symType, varEntry->name );
  }

  avl_free_data( &symTab, avl_free_data );

  return 0;
}
