
#include <stdlib.h>

#include "avl_tree.h"
#include "symtab.h"

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType ) {
  Symbol* newSymbol = NULL;

  if( symbolName && (*symbolName) ) {
    newSymbol = calloc(1, sizeof(Symbol));

    if( newSymbol ) {
      newSymbol->name = calloc(1, strlen(symbolName) + 1);

      if( newSymbol->name ) {
        strcpy( newSymbol->name, symbolName );
        newSymbol->symType = symbolType;
        return newSymbol;
      }

      ReleaseSymbol( &newSymbol );
    }
  }

  return NULL;
}

void ReleaseSymbol( Symbol** symbolPtr ) {
  if( symbolPtr ) {
    if( (*symbolPtr) ) {
      switch( (*symbolPtr)->symType ) {
      case symEnum:
        ReleaseSymbolTable( &SYMBOLREF((*symbolPtr))->sym.Enum.fieldTable );
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

static int CompareSymbols( const struct avl_tree_node* left,
  const struct avl_tree_node* right ) {

  if( (left && SYMBOLREF(left)->name) &&
    (right && SYMBOLREF(right)->name) ) {

    return strcmp(SYMBOLREF(left)->name, SYMBOLREF(right)->name);
  }

  return -1;
}

static int CompareNameToSymbolName( const void* leftName,
  const struct avl_tree_node* right ) {

  if( leftName &&
    (right && SYMBOLREF(right)->name) ) {

    return strcmp(leftName, SYMBOLREF(right)->name);
  }

  return -1;
}

SymbolTable* CreateSymbolTable() {
  SymbolTable* newSymbolTable = NULL;

  newSymbolTable = calloc(1, sizeof(SymbolTable));
  if( newSymbolTable ) {
    newSymbolTable->isOpen = 1;
  }

  return newSymbolTable;
}

void ReleaseSymbolTable( SymbolTable** symbolTablePtr ) {
  Symbol* symbol = NULL;

  if( symbolTablePtr ) {
    avl_tree_for_each_in_postorder(symbol, (*symbolTablePtr)->root,
        Symbol, node) {

      avl_tree_remove( symbolTablePtr, &symbol->node );

      ReleaseSymbol( &symbol );
    }
  }
}

unsigned DeclareSymbol( SymbolTable* symbolTable, Symbol* symbol ) {
  if( !(symbolTable && symbolTable->isOpen) ) { return 1; }
  if( !(symbol && (symbol->name) && (*(symbol->name))) ) { return 2; }

  if( avl_tree_insert(&symbolTable->root, &symbol->node,
      CompareSymbols) == NULL ) {
    return 0;
  }

  return 3;
}

Symbol* LookupSymbol( SymbolTable* symbolTable, const char* name ) {
  Symbol* symbol = NULL;

  if( symbolTable == NULL ) { return NULL; }
  if( !(name && (*name)) ) { return NULL; }

  if( symbolTable && name && (*name) ) {
    symbol = SYMBOLREF(avl_tree_lookup(symbolTable->root, name,
      CompareNameToSymbolName));
  }

  return symbol;
}

unsigned DeclareEnum( SymbolTable* symbolTable,
  const char* enumName, SymbolTable** fieldTablePtr ) {

  Symbol* newEnum = NULL;
  SymbolTable* newFieldTable = NULL;

  if( !(symbolTable && symbolTable->isOpen) ) { return 1; }
  if( !(enumName && (*enumName)) ) { return 2; }
  if( !(fieldTablePtr && ((*fieldTablePtr) == NULL)) ) { return 3; }

  newEnum = CreateSymbol(enumName, symEnum);

  if( newEnum ) {
    newFieldTable = CreateSymbolTable();

    if( newFieldTable ) {
      newEnum->sym.Enum.fieldTable = newFieldTable;
      *fieldTablePtr = newFieldTable;

      if( DeclareSymbol(symbolTable, newEnum) == 0 ) {
        return 0;
      }
    }
  }

  ReleaseSymbol( &newEnum );
  ReleaseSymbolTable( &newFieldTable );

  return 4;
}

unsigned CloseEnum( SymbolTable* symbolTable, const char* enumName ) {
  Symbol* enumSymbol = NULL;

  if( symbolTable == NULL ) { return 1; }
  if( !(enumName && (*enumName)) ) { return 2; }

  enumSymbol = LookupSymbol(symbolTable, enumName);
  if( enumSymbol && enumSymbol->sym.Enum.fieldTable ) {
    enumSymbol->sym.Enum.fieldTable->isOpen = 0;

    return 0;
  }

  return 3;
}

EnumFieldSymbol* DeclareEnumField( SymbolTable* fieldTable,
  const char* fieldName, unsigned fieldValue ) {

  Symbol* newEnumField = NULL;

  if( !(fieldTable && fieldTable->isOpen) ) { return NULL; }
  if( !(fieldName && (*fieldName)) ) { return NULL; }

  newEnumField = CreateSymbol(fieldName, symEnumField);
  if( newEnumField ) {
    if( DeclareSymbol(fieldTable, newEnumField) == 0 ) {
      newEnumField->sym.EnumField.value = fieldValue;
      return newEnumField;
    }
  }

  ReleaseSymbol( &newEnumField );

  return NULL;
}