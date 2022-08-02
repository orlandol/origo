
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_tree.h"
#include "symtab.h"

void DumpSymbolTable( SymbolTable* symbolTable ) {
  Symbol* symbol = symbolTable;

  if( symbolTable == NULL ) { return; }

  avl_tree_for_each_in_postorder(symbol, symbolTable->root, Symbol, node) {
    switch( symbol->symType ) {
    case symNamespace:
      printf( "Symbol[Namespace:%s]\n", symbol->name );
      break;

    case symEnum:
      printf( "Symbol[Enum:%s]\n", symbol->name );
      DumpSymbolTable( symbol->sym.Enum.fieldTable );
      break;

    case symEnumField:
      printf( "Symbol[EnumField:%s]\n", symbol->name );
      break;

    default:
      printf( "Symbol[%u:%s]\n", symbol->symType, symbol->name );
    }
  }
}

SymbolTable* symtab = NULL;

void Cleanup() {
  ReleaseSymbolTable( &symtab );
}

int main( int argc, char** argv ) {
  unsigned result = 0;

  atexit( Cleanup );

  symtab = CreateSymbolTable();
  if( symtab == NULL ) {
    printf( "Error creating symbol table\n" );
    exit(1);
  }

  SymbolTable* fieldtab = NULL;
  if( result = DeclareEnum(symtab, "FileMode", &fieldtab) ) {
    printf( "Error[%u] in DeclareEnum\n", result );
    exit(2);
  }

  if( result = CloseEnum(symtab, "FileMode") ) {
    printf( "Error[%u] in CloseEnum\n", result );
    exit(3);
  }

  DumpSymbolTable( symtab );

  Cleanup();

  return 0;
}

#include "avl_tree.c"
#include "symtab.c"
