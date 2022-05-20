
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

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right );

unsigned DeclareNamespace( SymbolTable* symbolTable, const char* name );

SymbolTable* symtab = NULL;

int main( int argc, char** argv ) {
  unsigned result;

  result = DeclareNamespace(symtab, "myprog");
  if( result ) {
    printf( "DeclareNamespace error: %u\n", result );
    exit(1);
  }

  return 0;
}

#include "avl_tree.c"

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

  Symbol* newNode = NULL;

  newNode = calloc(1, sizeof(Symbol));
  if( newNode == NULL ) { return 3; }

  newNode->name = calloc(1, strlen(name) + 1);
  if( newNode->name ) {
    strcpy( newNode->name, name );

    if( avl_tree_insert(&symbolTable, &newNode->node,
        CompareSymbols) == NULL ) {

      return 0;
    }
  }

  if( newNode ) {
    if( newNode->name ) {
      free( newNode->name );
      newNode->name = NULL;
    }

    free( newNode );
    newNode = NULL;
  }

  return 3;
}

