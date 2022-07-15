#ifndef SYMTAB_H
#define SYMTAB_H

#include "avl_tree.h"

enum SymType {
  symNone,
  symNamespace,
  symEnumField,
  symEnum
};

typedef struct NamespaceSymbol {
  char* name;
} NamespaceSymbol;

typedef struct EnumFieldSymbol {
  char* name;
  unsigned value;
} EnumFieldSymbol;

typedef struct EnumSymbol {
  char* name;
  struct Symbol* fieldRoot;
} EnumSymbol;

typedef struct Symbol {
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
} SymbolTable;

unsigned DeclareSymbol( SymbolTable* symTable, Symbol* symbol );
Symbol* LookupSymbol( SymbolTable* symTable, const char* name );

#endif
