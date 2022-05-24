
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "avl_tree.h"

#define SYMBOLREF(NODE) avl_tree_entry(NODE, Symbol, node)

enum SymbolType {
  symUndefined,
  symNamespace,
  symCopyright,
  symDescription,
  symVersion,
  symVersionString
};

typedef struct Symbol {
  char* name;
  unsigned symtype; // Used with SymbolType enum

  struct avl_tree_node node;
} Symbol;

typedef struct SymbolTable {
  Symbol* root;
} SymbolTable;

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType );
void FreeSymbol( Symbol** symbolPtr );

SymbolTable* CreateSymbolTable();
void ReleaseSymbolTable( SymbolTable** symbolTablePtr );

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right );

unsigned DeclareNamespace( SymbolTable* symbolTable, const char* name );

SymbolTable* symtab = NULL;

void DumpTree( SymbolTable* symbolTable ) {
  Symbol* symbol = NULL;

  if( !(symbolTable && symbolTable->root) ) {
    printf( "DumpTree: Tree empty\n" );
    exit(3);
  }

  avl_tree_for_each_in_postorder(symbol, symbolTable->root,
      Symbol, node) {
    printf( "Node[%s] == %u\n", symbol->name, symbol->symtype );
  }
}

int main( int argc, char** argv ) {
  unsigned result;

  symtab = CreateSymbolTable();

  DeclareNamespace( symtab, "myprog" );

  DumpTree( symtab );

  ReleaseSymbolTable( &symtab );

  return 0;
}

#include "avl_tree.c"

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType ) {
  Symbol* newSymbol = NULL;

  if( symbolName ) {
    newSymbol = calloc(1, sizeof(Symbol));

    if( newSymbol ) {
      newSymbol->name = calloc(1, strlen(symbolName) + 1);

      if( newSymbol->name ) {
        strcpy( newSymbol->name, symbolName );
        newSymbol->symtype = symbolType;

        return newSymbol;
      }

      FreeSymbol( &newSymbol );
    }
  }

  return NULL;
}

void FreeSymbol( Symbol** symbolPtr ) {
  if( symbolPtr ) {
    if( (*symbolPtr) ) {
      if( (*symbolPtr)->name ) {
        free( (*symbolPtr)->name );
        (*symbolPtr)->name = NULL;
      }

      free( (*symbolPtr) );
      (*symbolPtr) = NULL;
    }
  }
}

SymbolTable* CreateSymbolTable() {
  return (SymbolTable*)calloc(1, sizeof(SymbolTable));
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  Symbol* symbol = NULL;

  if( symbolTablePtr ) {
    avl_tree_for_each_in_postorder(symbol, (*symbolTablePtr)->root,
        Symbol, node) {

      avl_tree_remove( symbolTablePtr, &symbol->node );

      FreeSymbol( &symbol );
    }
  }
}

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right ) {

  if( (left && SYMBOLREF(left)->name) &&
    (right && SYMBOLREF(right)->name) ) {

    return strcmp(SYMBOLREF(left)->name, SYMBOLREF(right)->name);
  }

  return -1;
}

unsigned DeclareNamespace( SymbolTable* symbolTable, const char* name ) {
  if( symbolTable == NULL ) { return 1; }
  if( !(name && *name) ) { return 2; }

  Symbol* newNode = CreateSymbol(name, symNamespace);
  if( newNode == NULL ) { return 3; }

  if( avl_tree_insert(&symbolTable->root, &newNode->node,
      CompareSymbols) == NULL ) {
    return 0;
  }

  FreeSymbol( &newNode );

  return 3;
}

