
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "avl_tree.h"

#define ENUMREF(NODE) avl_tree_entry(NODE, EnumField, node)
#define STRUCTREF(NODE) avl_tree_entry(NODE, StructField, node)
#define UNIONREF(NODE) avl_tree_entry(NODE, UnionField, node)

#define SYMBOLREF(NODE) avl_tree_entry(NODE, Symbol, node)

enum PointerType {
  ptrNone,
  ptrData,
  ptrRef
};

enum BaseType {
  baseChar,
  baseInt,
  baseUint
};

enum SymbolType {
  symUndefined,
  symNamespace,
  symCopyright,
  symDescription,
  symVersion,
  symVersionString,
  symEnumOpened,
  symEnumClosed,
  symStructOpened,
  symStructClosed,
  symUnionOpened,
  symUnionClosed
};

typedef struct TypeSpec {
  unsigned pointerType; // Use with PointerType enum

  unsigned baseType; // Use with BaseType enum

  unsigned decBitSize;

  unsigned count;
  int minRange;
  int maxRange;
} TypeSpec;

typedef struct EnumField {
  char* fieldName;
  unsigned fieldValue;

  struct avl_tree_node node;
} EnumField;

typedef struct EnumTable {
  EnumField* root;
} EnumTable;

EnumField* CreateEnumField( const char* name, unsigned value );
void FreeEnumField( EnumField** enumfieldPtr );

EnumTable* CreateEnumTable();
void ReleaseEnumTable( EnumTable** enumTablePtr );

typedef struct StructField {
  char* fieldName;
  unsigned fieldType;

  struct avl_tree_node node;
} StructField;

typedef struct StructTable {
  StructField* root;
} StructTable;

typedef struct UnionField {
  char* fieldName;
  TypeSpec fieldType;

  struct avl_tree_node node;
} UnionField;

typedef struct UnionTable {
  UnionField* root;
} UnionTable;

typedef struct FuncParam {
  char* paramName;
  TypeSpec paramType;
} FuncParam;

typedef struct FuncParamTable {
  FuncParam* root;
} FuncParamTable;

typedef struct Symbol {
  char* name;

  unsigned symtype; // Use with SymbolType enum

  union {
    EnumTable* enumTable;
    StructTable* structTable;
    UnionTable* unionTable;
    FuncParamTable* paramTable;
  } symdata;

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

unsigned DeclareEnum( SymbolTable* symbolTable, const char* name, EnumTable** enumTable );
unsigned DeclareEnumField( EnumTable* enumTable, const char* fieldName, unsigned value );
unsigned CloseEnum( SymbolTable* symbolTable, const char* name );

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

EnumTable* enumTable = NULL;
DeclareEnum( symtab, "FileRead", &enumTable );

  DumpTree( symtab );

  ReleaseSymbolTable( &symtab );

  return 0;
}

#include "avl_tree.c"

EnumField* CreateEnumField( const char* name, unsigned value ) {
  return NULL;
}

void FreeEnumField( EnumField** enumfieldPtr ) {
}

EnumTable* CreateEnumTable() {
  return NULL;
}

void ReleaseEnumTable( EnumTable** enumTablePtr ) {
}

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
      switch( (*symbolPtr)->symtype ) {
      case symEnumOpened:
      case symEnumClosed:
        ReleaseEnumTable( &SYMBOLREF((*symbolPtr))->symdata.enumTable );
        break;
      }

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

  Symbol* newSymbol = CreateSymbol(name, symNamespace);
  if( newSymbol == NULL ) { return 3; }

  if( avl_tree_insert(&symbolTable->root, &newSymbol->node,
      CompareSymbols) == NULL ) {

    return 0;
  }

  FreeSymbol( &newSymbol );

  return 3;
}

unsigned DeclareEnum( SymbolTable* symbolTable, const char* name,
  EnumTable** enumTable ) {

  EnumTable* newEnumTable = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(name && (*name)) ) { return 2; }
  if( (enumTable == NULL) || (*enumTable) ) { return 3; }

  Symbol* newSymbol = CreateSymbol(name, symEnumOpened);
  if( newSymbol == NULL ) { return 3; }

if( newSymbol->symtype == symEnumOpened ) { printf( "sysEnumOpened\n" ); }


  if( avl_tree_insert(&symbolTable->root, &newSymbol->node,
      CompareSymbols) == NULL ) {
    newEnumTable = calloc(1, sizeof(EnumTable));
    if( newEnumTable ) {
      newSymbol->symdata.enumTable = newEnumTable;
      *enumTable = newEnumTable;
      return 0;
    }
  }

  FreeSymbol( &newSymbol );

  return 4;
}

unsigned DeclareEnumField( EnumTable* enumTable, const char* fieldName,
  unsigned value ) {

  return 4;
}

unsigned CloseEnum( SymbolTable* symbolTable, const char* name ) {
  return 2;
}
