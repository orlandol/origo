
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

/*
 *  Global variables
 */
  int argCount;
  char** argVar;

/*
 *  Token declarations
 */

  typedef enum _Token {
    // General tokens
    tkGeneral = (0 << 9),
      tkEOF,
      tkEOL,
      tkIdent,
      tkDot,
      tkDotDot,
      tkLParen,
      tkRParen,
      tkLBrace,
      tkRBrace,
      tkLDoubleBrace,
      tkRDoubleBrace,
      tkComma,
      tkColon,
      tkSemicolon,
      tkAt,

    // Reserved high level keyword tokens
    rsvdIdent = (1 << 9),
      rsvdProgram,
      rsvdEnum,
      rsvdStruct,
      rsvdObject,
      rsvdPublic,
      rsvdVisible,
      rsvdInternal,
      rsvdExtends,
      rsvdPrivate,
      rsvdInterface,
      rsvdImplements,
      rsvdMethod,
      rsvdSelf,
      rsvdFunc,
      rsvdAsm,
      rsvdNoRet,
      rsvdNoFrame,
      rsvdReturn,
      rsvdVar,
      rsvdRun,
      rsvdEnd,
      rsvdIf,
      rsvdThen,
      rsvdThenIf,
      rsvdElseIf,
      rsvdElse,
      rsvdEndIf,
      rsvdWhile,
      rsvdNext,
      rsvdBreak,
      rsvdEndWhile,
      rsvdIn,
      rsvdExit,

    // Literal value tokens
    valLiteral = (2 << 9),
      valFalse,
      valTrue,
      valInt,
      valInt8,
      valUint,
      valUint8,
      valString,
      valChar,

    // Operator tokens
    operSymbol = (3 << 9),
      operPrec00 = (operSymbol + (0 << 5)),
      operPrec01 = (operSymbol + (1 << 5)),
        opPostInc,
        opPostDec,
      operPrec02 = (operSymbol + (2 << 5)),
        opPreInc,
        opPreDec,
        unaryNeg,
        unaryIsNot,
        unaryNot,
      operPrec03 = (operSymbol + (3 << 5)),
      operPrec04 = (operSymbol + (4 << 5)),
        opMul,
        opDiv,
        opMod,
      operPrec05 = (operSymbol + (5 << 5)),
        opAdd,
        opSub,
      operPrec06 = (operSymbol + (6 << 5)),
        opShl,
        opShr,
        opSShr,
        opRol,
        opSRol,
        opRor,
        opSRor,
      operPrec07 = (operSymbol + (7 << 5)),
      operPrec08 = (operSymbol + (8 << 5)),
        opLT,
        opLTEq,
        opGT,
        opGTEq,
      operPrec09 = (operSymbol + (9 << 5)),
        opEq,
        opNotEq,
      operPrec10 = (operSymbol + (10 << 5)),
        opAnd,
      operPrec11 = (operSymbol + (11 << 5)),
        opXor,
      operPrec12 = (operSymbol + (12 << 5)),
        opOr,
      operPrec13 = (operSymbol + (13 << 5)),
        opAndIs,
      operPrec14 = (operSymbol + (14 << 5)),
        opOrIs,
      operPrec15 = (operSymbol + (15 << 5)),

    // Assignment operators
    setSymbol  = (4 << 9),
      setTo,
      setNot,
      setAdd,
      setSub,
      setMul,
      setDiv,
      setMod,
      setShl,
      setShr,
      setSShr,
      setRol,
      setSRol,
      setRor,
      setSRor,
      setAnd,
      setXor,
      setOr,

    // x86 general tokens
    x86General = (5 << 9),
      // x86 memory address
      x86Mem  = (x86General + (0 << 4) + 0),

      // x86 segment/selector register tokens
      x86SReg = (x86General + (1 << 4) + 0),
        x86SRegES,
        x86SRegCS,
        x86SRegSS,
        x86SRegDS,
        x86SRegFS,
        x86SRegGS,
        lastX86SReg = x86SRegGS,

      // x86 8-bit register tokens
      x86Reg8 = (x86General + (2 << 4) + 0),
        x86RegAL,
        x86RegCL,
        x86RegDL,
        x86RegBL,
        x86RegAH,
        x86RegCH,
        x86RegDH,
        x86RegBH,

      // x86 16-bit register tokens
      x86Reg16 =(x86General + (3 << 4) + 0),
        x86RegAX,
        x86RegCX,
        x86RegDX,
        x86RegBX,
        x86RegSP,
        x86RegBP,
        x86RegSI,
        x86RegDI,

      // x86 32-bit register tokens
      x86Reg32 = (x86General + (4 << 4) + 0),
        x86RegEAX,
        x86RegECX,
        x86RegEDX,
        x86RegEBX,
        x86RegESP,
        x86RegEBP,
        x86RegESI,
        x86RegEDI,

      // x86 mnemonic tokens
      x86Ident = (6 << 9),
  } Token;

/*
 *  Keyword table declarations
 */

  typedef struct _Keyword {
    char*    name;
    unsigned token;
  } Keyword;

  const Keyword keyword[] = {
    "end",     rsvdEnd,
    "program", rsvdProgram,
    "run",     rsvdRun
  };

  const size_t keywordCount = sizeof(keyword) / sizeof(keyword);

/*
 *  String declarations
 */

  typedef struct _string {
    size_t length;
    char   text[];
  } string;

  string* NewString( size_t length );
  string* CopyString( char* strVal );
  void FreeString( string** strVar );

  string* AppendString( string* toString, char* fromVal );

  int Compare( string* left, string* right );
  int CaseCompare( string* left, string* right );

/*
 *  Symbol table declarations
 */

  typedef avl_tree_t SymTab;

  SymTab* NewSymTab( char* tableName );
  void FreeSymTab( SymTab** symTab );

  void* Lookup( SymTab* table, char* name, void* symItem );

/*
 *  Symbol declarations
 */

  typedef struct _TypeSpec {
    // Optional name
    char*      name;
    // Optional Pointer or reference specifier
    unsigned   pointerType;
    // Required simple/complex type
    unsigned   baseType;
    size_t     typeSize;
    unsigned   typeCode;
    // Optional array specifier
    size_t     dimCount;
    unsigned*  dims;
    size_t     dimOfs;
    // Default value
    unsigned   initType;
    size_t     initSize;
    uint8_t*   initVal;
  } TypeSpec;

  typedef struct _CallSpec {
    unsigned callType;
  } CallSpec;

  typedef struct _Param {
    TypeSpec* typeSpec;
  } Param;

  typedef struct _ParamList {
    size_t paramCount;
    Param param[];
  } ParamList;

  typedef struct _VarSym {
    // Common Symbol Entry fields
    unsigned  symType;
    char*     name;
    // Global variable specifiers
    TypeSpec* typeSpec;
    size_t    dataOfs;
    // Optional initialized value
    unsigned  initType;
    size_t    initSize;
    uint8_t*  initVal;
  } VarSym;

  ParamList* NewParamList();
  void FreeParamList( ParamList** paramList );

  bool DeclareParam( ParamList* paramList, TypeSpec* typeSpec );

  bool DeclareVar( SymTab* symTab, char* name, TypeSpec* typeSpec,
    size_t varOffset, unsigned initType, void* initVal );

  bool DeclareFunc( SymTab* symTab, char* name, CallSpec* callSpec,
    TypeSpec* typeSpec, ParamList* paramList, unsigned initType,
    void* initVal );

  bool DeclareLabel( SymTab* symTab, char* name,
    unsigned targetType, size_t targetOfs );

  bool DeclareObject( SymTab* symTab, char* name );
  bool InheritObject( SymTab* symTab, char* objectName, char* baseObjectName );
  bool DeclareMember( SymTab* symTab, char* objectName, char* memberName,
    TypeSpec* typeSpec, unsigned initType, void* initVal );

/*
 *  Lexer declarations
 */

  typedef union _IntVal {
    unsigned valUint;
    int valInt;
  } IntVal;

  bool ReadIdent( FILE* sourceFile, char** ident );
  bool ReadInt( FILE* sourceFile, unsigned* intType, IntVal* intVar );
  bool ReadString( FILE* sourceFile, char** strVar );

/*
 *  Code generator declarations
 */

  typedef struct _x86InstructionBuffer {
    uint8_t fields;
    uint8_t prefix[4];
    uint8_t opcode[3];
    uint8_t modRM;
    uint8_t sib;
    uint32_t displacement;
    uint32_t immediate;
  } x86InstructionBuffer;

  typedef struct _x86Addr {
    unsigned fields;
    unsigned baseReg;
    unsigned indexReg;
    unsigned scale;
    int32_t  displacement;
  } x86Addr;

  bool x86Emit( FILE* binFile, x86InstructionBuffer* instruction );

  bool x86EncodeAddr16( x86InstructionBuffer* destInstructionk, x86Addr* addr16 );
  bool x86EncodeAddr32( x86InstructionBuffer* destInstructionk, x86Addr* addr32 );

  bool x86GenOpMem( x86InstructionBuffer* destInstruction,
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

int main( int argc, char* argv[] ) {
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
