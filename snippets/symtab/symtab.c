
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "avl_tree.h"

#define SYMREF(SYMNODE) avl_tree_entry(SYMNODE, SymbolTable, node)

enum SymbolType {
  symUndefined,
  symNamespace,
  symCopyright,
  symDescription,
  symVersion,
  symVersionString
};

typedef struct SymbolTable {
  char* name;
  unsigned symtype; // Used with SymbolType enum

  struct avl_tree_node node;
} SymbolTable, Symbol;

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType );
void FreeSymbol( Symbol** symbolPtr );

SymbolTable* CreateSymbolTable( const char* rootName );
void ReleaseSymbolTable( SymbolTable** symbolTablePtr );

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right );

unsigned DeclareNamespace( SymbolTable* symbolTable, const char* name );

SymbolTable* symtab = NULL;

void DumpTree( SymbolTable* symbolTable ) {
  Symbol* symbol = NULL;

  avl_tree_for_each_in_postorder(symbol, symbolTable,
      SymbolTable, node) {
    printf( "Node[%s] == %u\n", symbol->name, symbol->symtype );
  }
}


int main( int argc, char** argv ) {
  unsigned result;

  symtab = CreateSymbolTable("myprog");

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

SymbolTable* CreateSymbolTable( const char* rootName ) {
  return (SymbolTable*)calloc(1, sizeof(SymbolTable));
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  Symbol* symbol = NULL;

  if( symbolTablePtr ) {
    avl_tree_for_each_in_postorder(symbol, (*symbolTablePtr),
        SymbolTable, node) {

      avl_tree_remove( symbolTablePtr, &symbol->node );

      FreeSymbol( &symbol );
    }
  }
}

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right ) {

  if( (left && SYMREF(left)->name) && (right && SYMREF(right)->name) ) {
    return strcmp(SYMREF(left)->name, SYMREF(right)->name);
  }

  return -1;
}

unsigned DeclareNamespace( SymbolTable* symbolTable, const char* name ) {
//  Parameter symbolTable is allowed to be NULL
  if( !(name && *name) ) { return 2; }

  Symbol* newNode = CreateSymbol(name, symNamespace);
  if( newNode == NULL ) { return 3; }

  if( avl_tree_insert(&symbolTable, &newNode->node,
      CompareSymbols) == NULL ) {
    return 0;
  }

  FreeSymbol( &newNode );

  return 3;
}

