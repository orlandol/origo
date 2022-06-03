#include <stdio.h>
#include <stdlib.h>

/// Declarations ///

#include "avl_tree.h"

enum SymbolType {
  symUndefined,
  symNamespace = 1111,
  symCopyright = 1234
};

typedef struct Symbol {
  unsigned symtype;

  struct avl_tree_node node;
} Symbol;

typedef struct SymbolTable {
  Symbol* root;
} SymbolTable;

#define SYMBOLREF(nodeVar) avl_tree_entry(nodeVar, Symbol, node)

int CompareSymbols( const struct avl_tree_node* node1,
  const struct avl_tree_node* node2 );

void Insert( SymbolTable* symbolTable, unsigned symbolType );

Symbol* Lookup( SymbolTable* symbolTable, unsigned symbolType );

/// Main program ///
SymbolTable symtab;

void Cleanup() {
  printf( "Cleaning up...\n" );
}

void DumpSymbolTable( SymbolTable* symbolTable ) {
  Symbol* symbol = symbolTable;

  avl_tree_for_each_in_postorder(symbol, symbolTable->root, Symbol, node) {
    printf( "Symbol[%u]\n", symbol->symtype );
  }
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  printf( "Running basics program...\n" );

  DumpSymbolTable( &symtab );

  Insert( &symtab, symNamespace );
  Insert( &symtab, symCopyright );

Symbol* symbol = Lookup(&symtab, symCopyright);
if( symbol ) {
  printf( "Lookup(%u) == %u\n", symCopyright, symbol->symtype );
}

  DumpSymbolTable( &symtab );

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

void Insert( SymbolTable* symbolTable, unsigned symbolType ) {
  Symbol* newNode = calloc(1, sizeof(Symbol));

  if( newNode == NULL ) {
    printf( "Insert() error: 1\n" );
    exit(1);
  }

  newNode->symtype = symbolType;

  if( avl_tree_insert(&symbolTable->root, &newNode->node, CompareSymbols) != NULL ) {
    free( newNode );
    newNode = NULL;

    printf( "Insert() error: 2\n" );
    exit(1);
  }
  
  printf( "Insert() succeeded[%u]\n", newNode->symtype );
}

Symbol* Lookup( SymbolTable* symbolTable, unsigned symbolType ) {
  Symbol searchSymbol = { .symtype = symbolType };

  if( !(symbolTable && symbolTable->root) ) { return NULL; }
  if( symbolType == 0 ) { return NULL; }

  return SYMBOLREF(avl_tree_lookup_node(symbolTable->root,
      &searchSymbol.node, CompareSymbols));
}
