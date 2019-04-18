
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 *  Global variables
 */
  int argCount;
  char** argVar;

/*
 *  Token declarations
 */

/*
 *  String declarations
 */

/*
 *  Symbol table declarations
 */

  typedef struct _SymEntry
  {
    size_t entrySize;
  } SymEntry;

  typedef struct _SymTab
  {
    char*    tableName;
    size_t   entryCount;
    SymEntry* symItem;
  } SymTab;

  SymTab* NewSymTab( char* tableName );
  void FreeSymTab( SymTab** symTab );

  unsigned Lookup( SymTab* table, char* name, void* symItem );

/*
 *  Symbol declarations
 */

  typedef struct _TypeSpec
  {
    unsigned baseType;
  } TypeSpec;

  typedef struct _CallSpec
  {
    unsigned callType;
  } CallSpec;

  typedef struct _Param
  {
    TypeSpec* typeSpec;
  } Param;

  typedef struct _ParamList
  {
    size_t paramCount;
    Param* param;
  } ParamList;

  ParamList* NewParamList();
  void FreeParamList( ParamList** paramList );
  int DeclareParam( ParamList* paramList, TypeSpec* typeSpec );

  int DeclareVar( SymTab* symTab, char* name, TypeSpec* typeSpec,
    size_t varOffset, unsigned initType, void* initVal );

  int DeclareFunc( SymTab* symTab, char* name, CallSpec* callSpec,
    TypeSpec* typeSpec, ParamList* paramList, unsigned initType,
    void* initVal );

  int DeclareLabel( SymTab* symTab, char* name,
    unsigned targetType, size_t targetOfs );

  int DeclareObject( SymTab* symTab, char* name );
  int InheritObject( SymTab* symTab, char* objectName, char* baseObjectName );
  int DeclareMember( SymTab* symTab, char* objectName, char* memberName,
    TypeSpec* typeSpec, unsigned initType, void* initVal );

/*
 *  Lexer declarations
 */

  typedef union _IntVal
  {
    unsigned valUint;
    int valInt;
  } IntVal;

  int ReadIdent( FILE* sourceFile, char** ident );
  int ReadInt( FILE* sourceFile, unsigned* intType, IntVal* intVar );
  int ReadString( FILE* sourceFile, char** strVar );

/*
 *  Code generator declarations
 */

/*
 *  Windows PE declarations
 */

/*
 *  Backpatch declarations
 */

/*
 *  Expression parser declarations
 */

/*
 *  Parser declarations
 */

/*
 *  Main program
 */

int main( int argc, char* argv[] )
{
  argCount = argc;
  argVar = argv;

  return 0;
}

/*
 *  Token implementation
 */

/*
 *  String declarations
 */

/*
 *  Symbol table implementation
 */

/*
 *  Symbol implementation
 */

/*
 *  Lexer implementation
 */

/*
 *  Code generator implementation
 */

/*
 *  Windows PE implementation
 */

/*
 *  Backpatch implementation
 */

/*
 *  Expression parser implementation
 */

/*
 *  Parser implementation
 */
