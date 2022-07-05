
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "avl_tree.h"

#define ENUMFIELDREF(NODE) avl_tree_entry(NODE, EnumField, node)
#define STRUCTFIELDREF(NODE) avl_tree_entry(NODE, StructField, node)
#define UNIONFIELDREF(NODE) avl_tree_entry(NODE, UnionField, node)

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
  symEnum,
  symStruct,
  symUnion,
  symTypeDecl,
  symConst,
  symImport,
  symFuncDecl,
  symObject,
  symCtorDecl,
  symDtorDecl,
  symInterface,
  symMethodDecl,
  symOperatorDecl,
  symMain
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
  char* name;
  unsigned value;

  struct avl_tree_node node;
} EnumField;

typedef struct EnumTable {
  EnumField* root;
  unsigned tableOpen;
} EnumTable;

EnumField* CreateEnumField( const char* name, unsigned value );
void FreeEnumField( EnumField** enumFieldPtr );

EnumTable* CreateEnumTable();
void ReleaseEnumTable( EnumTable** enumTablePtr );

typedef struct StructField {
  char* name;
  TypeSpec typeSpec;

  struct avl_tree_node node;
} StructField;

typedef struct StructTable {
  StructField* root;
  unsigned tableOpen;
} StructTable;

typedef struct UnionField {
  char* name;
  TypeSpec typeSpec;

  struct avl_tree_node node;
} UnionField;

typedef struct UnionTable {
  UnionField* root;
  unsigned tableOpen;
} UnionTable;

typedef struct FuncParam {
  char* name;
  TypeSpec typeSpec;
} FuncParam;

typedef struct FuncParamTable {
  FuncParam* root;
  unsigned tableOpen;
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

void DumpEnumTable( EnumTable* enumTable ) {
  EnumField* field = NULL;

  if( !(enumTable && enumTable->root) ) {
    printf( "DumpEnumTable: Tree empty\n" );
    exit(5);
  }

  avl_tree_for_each_in_postorder(field, enumTable->root,
      EnumField, node) {
    printf( "  EnumField[%s] == %u\n", field->name, field->value );
  }
}

void DumpSymbolTable( SymbolTable* symbolTable ) {
  Symbol* symbol = NULL;

  if( !(symbolTable && symbolTable->root) ) {
    printf( "DumpSymbolTable: Tree empty\n" );
    exit(3);
  }

  avl_tree_for_each_in_postorder(symbol, symbolTable->root,
      Symbol, node) {

    switch( symbol->symtype ) {
    case symNamespace:
      printf( "Namespace[%s]\n", symbol->name );
      break;

    case symCopyright:
      printf( "Copyright[%s]\n", symbol->name );
      break;

    case symDescription:
      printf( "Description[%s]\n", symbol->name );
      break;

    case symVersion:
      printf( "Version[%s]\n", symbol->name );
      break;

    case symVersionString:
      printf( "VersionString[%s]\n", symbol->name );
      break;

    case symEnum:
      printf( "Enum[%s]\n", symbol->name );
      DumpEnumTable( symbol->symdata.enumTable );
      break;

    case symStruct:
      printf( "Struct[%s]\n", symbol->name );
      break;

    case symUnion:
      printf( "Union[%s]\n", symbol->name );
      break;

    default:
      printf( "Node[%s].symtype == %u\n",
        symbol->name, symbol->symtype );
    }
  }
}

int main( int argc, char** argv ) {
  unsigned result;

  symtab = CreateSymbolTable();

  DeclareNamespace( symtab, "myprog" );

EnumTable* enumtab = NULL;
DeclareEnum( symtab, "FileRead", &enumtab );
DeclareEnumField( enumtab, "Read", 1 );
DeclareEnumField( enumtab, "Write", 2 );

  DumpSymbolTable( symtab );

  ReleaseSymbolTable( &symtab );

  return 0;
}

#include "avl_tree.c"

EnumField* CreateEnumField( const char* name, unsigned value ) {
  EnumField* newEnumField = NULL;

  if( name && (*name) ) {
    newEnumField = calloc(1, sizeof(EnumField));

    if( newEnumField ) {
      newEnumField->name = calloc(1, strlen(name) + 1);

      if( newEnumField->name ) {
        strcpy( newEnumField->name, name );
        newEnumField->value = value;

        return newEnumField;
      }

      FreeEnumField( &newEnumField );
    }
  }

  return NULL;
}

void FreeEnumField( EnumField** enumFieldPtr ) {
  if( enumFieldPtr ) {
    if( (*enumFieldPtr) ) {
      if( (*enumFieldPtr)->name ) {
        free( (*enumFieldPtr)->name );
        (*enumFieldPtr)->name = NULL;
      }

      free( (*enumFieldPtr) );
      (*enumFieldPtr) = NULL;
    }
  }
}

EnumTable* CreateEnumTable() {
  return calloc(1, sizeof(EnumTable));
}

void ReleaseEnumTable( EnumTable** enumTablePtr ) {
  EnumField* enumField = NULL;

  if( enumTablePtr ) {
    if( (*enumTablePtr) ) {
      avl_tree_for_each_in_postorder(enumField, (*enumTablePtr)->root,
          EnumField, node) {

        avl_tree_remove( enumTablePtr, &enumField->node );

        FreeEnumField( &enumField );
      }

      free( (*enumTablePtr) );
      (*enumTablePtr) = NULL;
    }
  }
}

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType ) {
  Symbol* newSymbol = NULL;

  if( symbolName && (*symbolName) ) {
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
      case symEnum:
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
  return calloc(1, sizeof(SymbolTable));
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

  Symbol* newSymbol = CreateSymbol(name, symEnum);
  if( newSymbol == NULL ) { return 3; }

  if( avl_tree_insert(&symbolTable->root, &newSymbol->node,
      CompareSymbols) == NULL ) {
    newEnumTable = calloc(1, sizeof(EnumTable));
    if( newEnumTable ) {
      newSymbol->symdata.enumTable = newEnumTable;
      newSymbol->symdata.enumTable->tableOpen = 1;
      *enumTable = newEnumTable;
      return 0;
    }
  }

  FreeSymbol( &newSymbol );

  return 4;
}

static int CompareEnumFieldNodes( const struct avl_tree_node* left,
  const struct avl_tree_node* right ) {

  if( (left && ENUMFIELDREF(left)->name) &&
    (right && ENUMFIELDREF(right)->name) ) {

    return strcmp(ENUMFIELDREF(left)->name, ENUMFIELDREF(right)->name);
  }

  return -1;
}

static int CompareEnumFieldNameToNode( const void* name,
  const struct avl_tree_node* right ) {

  if( name && (right && ENUMFIELDREF(right)->name) ) {
    return strcmp(name, ENUMFIELDREF(right)->name);
  }

  return -1;
}

unsigned DeclareEnumField( EnumTable* enumTable, const char* fieldName,
  unsigned value ) {

  if( enumTable == NULL ) { return 1; }
  if( !(fieldName && (*fieldName)) ) { return 2; }

  if( enumTable->tableOpen == 0 ) { return 3; }

  EnumField* newEnumField = CreateEnumField(fieldName, value);
  if( newEnumField == NULL ) { return 4; }

  if( avl_tree_insert(&enumTable->root, &newEnumField->node,
      CompareEnumFieldNodes) == NULL ) {

    return 0;
  }

  FreeEnumField( &newEnumField );

  return 5;
}

unsigned CloseEnum( SymbolTable* symbolTable, const char* name ) {
  Symbol* symbol = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(name && (*name)) ) { return 2; }

  symbol = SYMBOLREF(avl_tree_lookup(symbolTable->root, name,
    CompareEnumFieldNameToNode));

  if( !(symbol && (symbol->symtype == symEnum)) ) { return 3; }

  if( !(symbol->symdata.enumTable &&
      (symbol->symdata.enumTable->tableOpen == 0)) ) { return 5; }

  symbol->symdata.enumTable->tableOpen = 1;

  return 0;
}
