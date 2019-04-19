
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

  typedef struct _string
  {
    size_t length;
    char text[];
  } string;

  string* ReserveString( size_t length );
  string* CopyString( char* strVal );
  void FreeString( string** strVar );

  string* AppendString( string* toString, char* fromVal );

  int CaseCompare( string* left, string* right );
  int Compare( string* left, string* right );

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

  typedef struct _x86InstructionBuffer
  {
    uint8_t fields;
    uint8_t prefix[4];
    uint8_t opcode[3];
    uint8_t modRM;
    uint8_t sib;
    uint32_t displacement;
    uint32_t immediate;
  } x86InstructionBuffer;

  typedef struct _x86Addr
  {
    unsigned fields;
    unsigned baseReg;
    unsigned indexReg;
    unsigned scale;
    int32_t  displacement;
  } x86Addr;

  int x86Emit( FILE* binFile, x86InstructionBuffer* instruction );

  int x86EncodeAddr16( x86InstructionBuffer* destInstructionk, x86Addr* addr16 );
  int x86EncodeAddr32( x86InstructionBuffer* destInstructionk, x86Addr* addr32 );

  int x86GenOpMem( x86InstructionBuffer* destInstruction,
    unsigned mnemonic, x86Addr* addr );

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
