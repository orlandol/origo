#ifndef SYMTAB_H
#define SYMTAB_H

#include "avl_tree.h"

enum SymType {
  symNone,
  symNamespace,
  symEnum,
  symEnumField
};

typedef struct EnumFieldSymbol {
  char* name;
  unsigned value;
} EnumFieldSymbol;

typedef struct EnumSymbol {
  struct SymbolTable* fieldTable;
} EnumSymbol;

typedef struct Symbol {
  char* name;
  unsigned symType;

  union {
    EnumSymbol Enum;
    EnumFieldSymbol EnumField;
  } sym;

  struct avl_tree_node node;
} Symbol;

#define SYMBOLREF(nodeVar) avl_tree_entry(nodeVar, Symbol, node)

typedef struct SymbolTable {
  Symbol* root;
  unsigned isOpen;
} SymbolTable;

Symbol* CreateSymbol( const char* symbolName, unsigned symbolType );
void ReleaseSymbol( Symbol** symbolPtr );

SymbolTable* CreateSymbolTable();
void ReleaseSymbolTable( SymbolTable** symbolTablePtr );

unsigned DeclareSymbol( SymbolTable* symbolTable, Symbol* symbol );
Symbol* LookupSymbol( SymbolTable* symbolTable, const char* name );

unsigned DeclareEnum( SymbolTable* symbolTable,
  const char* enumName, SymbolTable** fieldTablePtr );
unsigned CloseEnum( SymbolTable* symbolTable, const char* enumName );

EnumFieldSymbol* DeclareEnumField( SymbolTable* fieldTable,
  const char* fieldName, unsigned fieldValue );

#endif
