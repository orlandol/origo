#include <stdio.h>
#include <stdlib.h>

/// Declarations ///

#include "avl_tree.h"

enum SymbolType {
  symUndefined,
  symNamespace = 1111,
  symCopyright = 1234
};

typedef struct SymbolTable {
  unsigned symtype;

  struct avl_tree_node node;
} SymbolTable, Symbol;

#define SYMBOLREF(nodeVar) avl_tree_entry(nodeVar, Symbol, node)

int CompareSymbols( const struct avl_tree_node* node1,
  const struct avl_tree_node* node2 );

void Insert( SymbolTable** symbolTable, unsigned symbolType );

/// Main program ///
SymbolTable* symtab = NULL;

void Cleanup() {
  printf( "Cleaning up...\n" );
}

void DumpSymbolTable( SymbolTable* symbolTable ) {
  Symbol* symbol = symbolTable;

  avl_tree_for_each_in_postorder(symbol, symbolTable, Symbol, node) {
    printf( "Symbol[%u]\n", symbol->symtype );
  }
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  printf( "Running basics program...\n" );

  Insert( &symtab, symNamespace );
  Insert( &symtab, symCopyright );

  DumpSymbolTable( symtab );

  return 0;
}

/// Implemenetation ///

#include "avl_tree.c"


int CompareSymbols( const struct avl_tree_node* node1,
  const struct avl_tree_node* node2 ) {

  if( node1 && node2 ) {
    printf( "CompareSymbols[%u, %u]\n", SYMBOLREF(node1)->symtype, SYMBOLREF(node2)->symtype );

    return (int)(SYMBOLREF(node1)->symtype - SYMBOLREF(node2)->symtype);
  }

  return 1;
}

void Insert( SymbolTable** symbolTable, unsigned symbolType ) {
  Symbol* newNode = calloc(1, sizeof(Symbol));

  if( newNode == NULL ) {
    printf( "Insert() error: 1\n" );
    exit(1);
  }

  newNode->symtype = symbolType;

  if( avl_tree_insert(symbolTable, &newNode->node, CompareSymbols) != NULL ) {
    free( newNode );
    newNode = NULL;

    printf( "Insert() error: 2\n" );
    exit(1);
  }
  
  printf( "Insert() succeeded[%u]\n", newNode->symtype );
}
