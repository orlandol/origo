
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl.h"
#include "keyarray.h"

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
      rsvdStop,
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
    assignSymbol  = (4 << 9),
      assignTo,
      assignNot,
      assignAdd,
      assignSub,
      assignMul,
      assignDiv,
      assignMod,
      assignShl,
      assignShr,
      assignSShr,
      assignRol,
      assignSRol,
      assignRor,
      assignSRor,
      assignAnd,
      assignXor,
      assignOr,

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

  typedef struct Keyword {
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

  typedef struct rstring {
    size_t length;
    size_t rsvdLength;
  } rstring;

  size_t rstrlen( rstring* source );
  size_t rstrrsvd( rstring* source );
  char* rstrtext( rstring* source );

  rstring* rstralloc( size_t reserveLength );
  rstring* rstrzalloc( size_t reserveLength );

  rstring* rstrcpy( rstring* source );
  rstring* rstrcpyc( char* source, size_t sourceLength );

  rstring* rstrappendch( rstring* dest, char matchCh );

  rstring* rstrappend( rstring* dest, rstring* source );
  rstring* rstrappendc( rstring* dest, char* source, size_t sourceLength );

  size_t rrevscan( rstring* source, char matchCh );
  size_t rrevscanc( char* source, char matchCh );

  size_t rscan( rstring* source, char matchCh );
  size_t rscanc( char* source, char matchCh );

  rstring* rsubstr( rstring* source, size_t startPos, size_t endPos );
  rstring* rsubstrc( char* source, size_t sourceLength, size_t startPos, size_t endPos );

/*
 *  Symbol table declarations
 */

  typedef avl_tree_t SymTab;

  SymTab* NewSymTab( char* tableName );
  void FreeSymTab( SymTab** symTab );

  void* Lookup( SymTab* table, char* name );

/*
 *  Symbol declarations
 */

  typedef enum _SymType {
    funcDecl = 1,
    funcForward,
    funcImport
    // ...
  } SymType;

  // Function/Method parameter list
  typedef struct TypeSpec TypeSpec;

  typedef struct Param {
    // Parameter specifiers
    TypeSpec* typeSpec;
    size_t    paramOfs;
  } Param;

  // [<'@' | '#'>]baseType['[' CONSTEXPR[',' ...] ']'] ['(' [PARAMDECL[',' ...]] ')']
  typedef struct TypeSpec {
    // Optional name
    char*      name;
    // Optional Pointer specifiers: [Far] Pointer, [Far] Reference
    unsigned   pointerType;
    // Required simple/complex type
    unsigned   baseType;
    size_t     typeSize;
    unsigned   typeID;
    // Optional array specifier
    size_t     dimCount;
    unsigned*  dims;
    size_t     dimOfs;
    // Function parameter list
    size_t     paramCount;
    Param*     param;
    // Default value
    unsigned   initType;
    size_t     initSize;
    uint8_t*   initVal;
  } TypeSpec;

  //  enum [BASETYPE] IDENT
  //    [IDENT[, ...] ['=' CONSTEXPR]]
  //  end
  typedef struct EnumField {
    char*    name;
    size_t   valSize;
    uint8_t* fieldVal; 
  } EnumField;

  typedef struct EnumSym {
    // Common Symbol Entry fields
    char*      name;
    unsigned   symType;
    // Optional simple type
    unsigned   baseType;
    size_t     typeSize;
    // enum Fields
    size_t     fieldCount;
    EnumField* field;
  } EnumSym;

  //  union IDENT => UNIONNAME
  //    [TYPESPEC IDENT[, ...]]
  //  end
  typedef struct UnionField {
    char*     fieldName;
    TypeSpec* typeSpec;
    unsigned  fieldOfs;
  } UnionField;

  typedef struct UnionSym {
    // Common Symbol Entry fields
    char*       name;
    unsigned    symType;
    // Union fields
    size_t      fieldCount;
    UnionField* field;
  } UnionSym;

  //  struct IDENT => STRUCTNAME
  //    [TYPESPEC IDENT[, ...] ['=' CONSTEXPR]]
  //    [union '(' TYPESPEC[, ...] ')' IDENT[, ...]]
  //  end
  typedef struct StructField {
    char*     fieldName;
    TypeSpec* typeSpec;
    unsigned  fieldOfs;
    // Optional init value
    size_t    initSize;
    uint8_t*  initVal;
  } StructField;

  typedef struct StructSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Union fields
    size_t       fieldCount;
    StructField* field;
  } StructSym;

  //  type TYPESPEC IDENT ['=' CONSTEXPR][, ...]
  typedef struct TypeSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Type Specifier
    TypeSpec* typeSpec;
    // Optional init value
    size_t    initSize;
    uint8_t*  initVal;
  } TypeSym;

  //  const
  //    [TYPESPEC IDENT '=' CONSTEXPR[, ...]]
  //  end
  typedef struct ConstSym {
    // Common Symbol Entry fields
    char*     name;
    unsigned  symType;
    // Variable specifiers
    TypeSpec* typeSpec;
    size_t    constOfs;
    // Optional initialized value
    size_t    initSize;
    uint8_t*  initVal;
  } ConstSym;

  //  var
  //    [TYPESPEC IDENT['=' CONSTEXPR][, ...]]
  //  end
  typedef struct VarSym {
    // Common Symbol Entry fields
    char*     name;
    unsigned  symType;
    // Variable specifiers
    TypeSpec* typeSpec;
    size_t    varOfs;
    // Optional initialized value
    size_t    initSize;
    uint8_t*  initVal;
  } VarSym;

  // Function/Method cdecl, stdcall, fastcall, thiscall, etc specifier
  typedef struct CallSpec {
    // Call stack specifier
    unsigned  callType;
    // Left to right, right to left, etc
    unsigned  paramOrder;
    // Maximum parameters allowed by count and/or by size
    size_t    maxParams;
    size_t    stackLimit;
    // Destination register or stack parameter specifiers
    size_t    destCount;
    unsigned* paramDest;
    // Default frame type: esp or ebp
    unsigned  frameType;
  } CallSpec;

  // [declare] func [CALLSPEC] [TYPESPEC] IDENT '(' [PARAMDECL[, ...]] ')' ... end
  // import func [CALLSPEC] [TYPESPEC] IDENT '(' [PARAMDECL[, ...]] ')'
  //   from DLLNAMESTR [as ACTUALNAMESTR]
  typedef struct FuncSym {
    // Common Symbol Entry fields
    unsigned  symType;
    char*     name;
    // Function specifiers
    CallSpec* callSpec;
    TypeSpec* typeSpec;
    size_t    funcOfs;
    // Function parameter list
    size_t    paramCount;
    Param*    funcParam;
    // Optional import specifiers
    char*     dllName;
    char*     linkName;
    // Optional initialized value
    unsigned  initType;
    size_t    initSize;
    uint8_t*  initVal;
  } FuncSym;

  //  object IDENT
  //    [inherits OBJNAME[, ...]]
  //  [<public | mutable>]
  //    [TYPESPEC IDENT['=' CONSTEXPR][, ...]]
  //  end
  typedef struct ObjectDescendant {
    char* descendantName;
  } ObjectDescendant;

  typedef struct MemberSym {
    char*     memberName;
    size_t    memberOfs;
    unsigned  memberScope;
    // Descendant list
    size_t            descendantCount;
    ObjectDescendant* descendant;
    // Member variables
    TypeSpec* typeSpec;
    size_t    memberOfs;
    // Optional initialized value
    size_t    initSize;
    uint8_t*  initVal;
  } MemberSym;

  typedef struct ObjectSym {
    // Common Symbol Entry fields
    char*      name;
    unsigned   symType;
    // Object specifiers
    unsigned   objectID;
    // Member specifiers
    size_t     memberCount;
    MemberSym* member;
  } ObjectSym;

  //  ctor OBJNAME.IDENT '(' [MEMBERNAME[, ...]] ')'
  typedef struct MemberParam {
    char*     name;
    // Optional initialized value
    size_t    initSize;
    uint8_t*  initVal;
  } MemberParam;

  typedef struct CtorSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Offset value
    size_t       ctorOfs;
    // Name specifiers
    char*        objectName;
    char*        initName;
    // Member parameter list
    size_t       memberCount;
    MemberParam* memberInit;
  } CtorSym;

  //  dtor OBJNAME '(' ')'
  typedef struct DtorSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Offset value
    size_t       dtorOfs;
    // Name specifiers
    char*        objectName;
  } DtorSym;

  //  abstract <OBJNAME | IDENT [implements OBJNAME[, ...]]>
  //      [inherits <ABSNAME | IFACENAME>[, ...]]
  //    [method [CALLSPEC] [TYPESPEC] IDENT '(' [PARAMDECL[, ...]] ')']
  //  end
  typedef struct IFaceName {
    char* ifaceName;
  } IFaceName;

  typedef struct MethodSym MethodSym;

  typedef struct AbstractSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Inherited Abstract/Interfaces
    size_t       ifaceCount;
    IFaceName*   inheritedIFace;
    // Method declarations
    // ...
  } AbstractSym;

  typedef struct InterfaceSym {
    // Common Symbol Entry fields
    char*        name;
    unsigned     symType;
    // Inherited Abstract/Interfaces
    size_t       ifaceCount;
    IFaceName*   inheritedIFace;
    // Method declarations
    // ...
  } InterfaceSym;

  //  method [CALLSPEC] [TYPESPEC]
  //      <ABSNAME | IFACENAME>.METHODNAME '(' [PARAMDECL[, ...]] ')'
  typedef struct MethodSym {
    // Common Symbol Entry fields
    unsigned  symType;
    char*     name;
    // Function specifiers
    CallSpec* callSpec;
    TypeSpec* typeSpec;
    size_t    funcOfs;
    // Method parameter list
    size_t    paramCount;
    Param*    methodParam;
    // Optional initialized value
    unsigned  initType;
    size_t    initSize;
    uint8_t*  initVal;
  } MethodSym;

  bool DeclareParam( FuncSym* funcSym, TypeSpec* typeSpec );

  bool DeclareVar( SymTab* symTab, char* name, TypeSpec* typeSpec,
    size_t varOffset, unsigned initType, void* initVal );

  bool DeclareFunc( SymTab* symTab, char* name, CallSpec* callSpec,
    TypeSpec* typeSpec, Param* funcParam, size_t paramCount, unsigned initType,
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

  typedef struct SourceFile {
    FILE* handle;
    rstring* fileName;
    unsigned line;
    unsigned col;
    unsigned nextLine;
    unsigned nextCol;
    char curCh;
    char nextCh;
  } SourceFile;

  SourceFile* OpenSource( rstring* name );
  void CloseSource( SourceFile** sourceFile );

  bool ReadChar( SourceFile* sourceFile );

  rstring* ReadIdent( SourceFile* sourceFile, rstring* destIdent );
  bool ReadInt( FILE* sourceFile, void* destInt );
  rstring* ReadString( FILE* sourceFile, rstring* destString );

/*
 *  Code generator declarations
 */

  typedef struct x86InstructionBuffer {
    uint8_t fields;
    uint8_t prefix[4];
    uint8_t opcode[3];
    uint8_t modRM;
    uint8_t sib;
    uint32_t displacement;
    uint32_t immediate;
  } x86InstructionBuffer;

  typedef struct x86Addr {
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

  rstring* programStr = rstrcpyc( "program", 0 );
  rstring* programStrCopy = rstrcpy(programStr);
  rstring* programSubStr = NULL;

  printf( "[%p]->\"%s\"\n", programStr, rstrtext(programStr) );
  printf( "[%p]->\"%s\"\n", programStrCopy, rstrtext(programStrCopy) );

  programStr = rstrappendch( programStr, ' ' );
  programStr = rstrappendch( programStr, 't' );
  programStr = rstrappendch( programStr, 'e' );
  programStr = rstrappendch( programStr, 's' );
  programStr = rstrappendch( programStr, 't' );
  programStr = rstrappendch( programStr, 'i' );
  programStr = rstrappendch( programStr, 'n' );
  programStr = rstrappendch( programStr, 'g' );
  programStr = rstrappendch( programStr, '1' );
  programStr = rstrappendch( programStr, '2' );
  programStr = rstrappendch( programStr, '3' );

  programStrCopy = rstrappendc( programStrCopy, " testing", 0 );
  programStrCopy = rstrappendch( programStrCopy, '4' );
  programStrCopy = rstrappendch( programStrCopy, '5' );
  programStrCopy = rstrappendch( programStrCopy, '6' );

  printf( "[%p]->\"%s\"\n", programStr, rstrtext(programStr) );
  printf( "[%p]->\"%s\"\n", programStrCopy, rstrtext(programStrCopy) );

  programSubStr = rsubstr(programStr, rscan(programStr, 'p'), rscan(programStr, 'm'));
  programSubStr = rstrappendch( programSubStr, ' ' );
  programSubStr = rstrappendch( programSubStr, 't' );
  programSubStr = rstrappendch( programSubStr, 'e' );
  programSubStr = rstrappendch( programSubStr, 's' );
  programSubStr = rstrappendch( programSubStr, 't' );
  programSubStr = rstrappendch( programSubStr, 'i' );
  programSubStr = rstrappendch( programSubStr, 'n' );
  programSubStr = rstrappendch( programSubStr, 'g' );
  programSubStr = rstrappendch( programSubStr, '7' );
  programSubStr = rstrappendch( programSubStr, '8' );
  programSubStr = rstrappendch( programSubStr, '9' );
  printf( "[%p]->\"%s\"\n", programSubStr, rstrtext(programSubStr) );

  free( programStr );
  programStr = NULL;

  free( programStrCopy );
  programStrCopy = NULL;

  free( programSubStr );
  programSubStr = NULL;

  printf( "[%p]\n", programStr );
  printf( "[%p]\n", programStrCopy );

  printf( "Reverse scan 'b' in 'abcdcba': %u\n", rrevscanc("abcdcba", 'b') );
  printf( "Scan 'b' forward in 'abcdcba': %u\n", rscanc("abcdcba", 'b') );

  return 0;
}

/*
 *  Token implementation
 */

/*
 *  String declarations
 */

  inline size_t rstrlen( rstring* source ) {
    return (source ? source->length : 0);
  }

  inline size_t rstrrsvd( rstring* source ) {
    return (source ? source->rsvdLength : 0);
  }

  inline char* rstrtext( rstring* source ) {
    return (source ? (((char*)source) + sizeof(rstring)) : NULL);
  }

  rstring* rstralloc( size_t reserveLength ) {
    rstring* newString = NULL;
    size_t   textSize;

    if( reserveLength == 0 ) {
      reserveLength = 8;
    }

    textSize = (reserveLength + 7) & (~7);
    if( reserveLength > textSize ) {
      return NULL;
    }

    newString = malloc(sizeof(rstring) + textSize);
    if( newString ) {
      newString->length = 0;
      newString->rsvdLength = textSize;

      *(((char*)newString) + sizeof(rstring)) = '\0';
    }

    return newString;
  }

  rstring* rstrzalloc( size_t reserveLength ) {
    rstring* newString = NULL;
    size_t   textSize;

    if( reserveLength == 0 ) {
      reserveLength = 8;
    }

    textSize = (reserveLength + 7) & (~7);
    if( reserveLength > textSize ) {
      return NULL;
    }

    newString = calloc(1, sizeof(rstring) + textSize);
    if( newString ) {
      newString->rsvdLength = textSize;
    }

    return newString;
  }

  rstring* rstrcpy( rstring* source ) {
    rstring* newString = NULL;
    size_t sourceLength;
    size_t textSize;

    sourceLength = rstrlen(source);
    textSize = (sourceLength + 8) & (~7);
    if( sourceLength > textSize ) {
      return NULL;
    }

    newString = calloc(1, sizeof(rstring) + textSize);
    if( newString ) {
      newString->length = sourceLength;
      if( source ) {
        strcpy( ((char*)newString) + sizeof(rstring),
              ((char*)source) + sizeof(rstring) );
      }
    }

    if( newString ) {
      newString->rsvdLength = textSize;
    }

    return newString;
  }

  rstring* rstrcpyc( char* source, size_t sourceLength ) {
    rstring* newString = NULL;
    size_t textSize;

    if( source == NULL ) {
      source = "";
    }

    if( sourceLength == 0 ) {
      sourceLength = strlen(source);
    }

    textSize = (sourceLength + 8) & (~7);
    if( sourceLength > textSize ) {
      return NULL;
    }

    if( source ) {
      newString = calloc(1, sizeof(rstring) + textSize);
      if( newString ) {
        newString->length = sourceLength;
        strcpy( ((char*)newString) + sizeof(rstring), source );
      }
    }

    if( newString ) {
      newString->rsvdLength = textSize;
    }

    return newString;
  }

  rstring* rstrappendch( rstring* dest, char ch ) {
    rstring* destTemp;
    char* destText;
    size_t destLength;
    size_t destRsvd;
    size_t destSize;

    destLength = rstrlen(dest);
    destRsvd = rstrrsvd(dest);

    if( (destRsvd - destLength) < 2 ) {
      destSize = (destRsvd + 8) & (~7);

      destTemp = realloc(dest, sizeof(rstring) + destSize);
      if( destTemp == NULL ) {
        return NULL;
      }
      dest = destTemp;

      dest->rsvdLength = destSize;
    }

    destText = rstrtext(dest);
    if( destText ) {
      destText[destLength + 0] = ch;
      destText[destLength + 1] = '\0';
      dest->length++;
    }

    return dest;
  }

  rstring* rstrappend( rstring* dest, rstring* source ) {
    rstring* destTemp;
    size_t destLength;
    size_t destRsvd;
    size_t destSize;
    size_t sourceLength;
    size_t totalLength;

    if( source == NULL ) {
      return dest;
    }

    destLength = rstrlen(dest);
    destRsvd = rstrrsvd(dest);

    sourceLength = rstrlen(source);

    totalLength = destLength + sourceLength;
    destSize = (totalLength + 8) & (~7);
    if( totalLength > destSize ) {
      return NULL;
    }

    if( destSize > destRsvd ) {
      destTemp = realloc(dest, sizeof(rstring) + destSize);
      if( destTemp == NULL ) {
        return NULL;
      }
      dest = destTemp;
      dest->rsvdLength = destSize;
    }

    strcpy( ((char*)dest) + sizeof(rstring) + destLength,
        ((char*)source) + sizeof(rstring) );

    dest->length = totalLength;

    return dest;
  }

  rstring* rstrappendc( rstring* dest, char* source, size_t sourceLength ) {
    rstring* destTemp;
    size_t destLength;
    size_t destRsvd;
    size_t destSize;
    size_t totalLength;

    if( source == NULL ) {
      return dest;
    }

    destLength = rstrlen(dest);
    destRsvd = rstrrsvd(dest);

    if( source == NULL ) {
      source = "";
    }

    if( sourceLength == 0 ) {
      sourceLength = strlen(source);
    }

    totalLength = destLength + sourceLength;
    destSize = (totalLength + 8) & (~7);
    if( totalLength > destSize ) {
      return NULL;
    }

    if( destSize > destRsvd ) {
      destTemp = realloc(dest, sizeof(rstring) + destSize);
      if( destTemp == NULL ) {
        return NULL;
      }
      dest = destTemp;
      dest->rsvdLength = destSize;
    }

    strcpy( ((char*)dest) + sizeof(rstring) + destLength, source );

    dest->length = totalLength;

    return dest;
  }

  size_t rscan( rstring* source, char matchCh ) {
    char* sourceCh;
    size_t sourceLength;
    size_t matchIndex;

    sourceCh = rstrtext(source);
    sourceLength = rstrlen(source);

    if( !(sourceCh && sourceLength) ) {
      return (-1);
    }

    for( matchIndex = 0; matchIndex < sourceLength; matchIndex++ ) {
      if( *sourceCh == '\0' ) {
        break;
      }

      if( *sourceCh == matchCh ) {
        return matchIndex;
      }
      sourceCh++;
    }

    return (-1);
  }

  size_t rscanc( char* source, char matchCh ) {
    char* sourceCh;
    size_t matchIndex;

    sourceCh = source;

    if( sourceCh == NULL ) {
      return (-1);
    }

    matchIndex = 0;
    while( *sourceCh ) {
      if( *sourceCh == matchCh ) {
        return matchIndex;
      }
      sourceCh++;
      matchIndex++;
    }

    return (-1);
  }

  size_t rrevscan( rstring* source, char matchCh ) {
    char* sourceCh;
    size_t sourceLength;
    size_t matchIndex;

    sourceLength = rstrlen(source);
    sourceCh = rstrtext(source) + sourceLength;

    if( source == NULL ) {
      return (-1);
    }

    while( sourceLength && (*sourceCh) ) {
      if( (*sourceCh) == matchCh ) {
        return sourceLength;
      }

      sourceLength--;
    }

    return (-1);
  }

  size_t rrevscanc( char* source, char matchCh ) {
    char* sourceCh;
    size_t sourceIndex;
    size_t matchIndex = (-1);

    if( source == NULL ) {
      return (-1);
    }

    sourceCh = source;

    sourceIndex = 0;
    while( (*sourceCh) ) {
      if( (*sourceCh) == matchCh ) {
        matchIndex = sourceIndex;
      }

      sourceIndex++;
      sourceCh++;
    }

    return matchIndex;
  }

  rstring* rsubstr( rstring* source, size_t startPos, size_t endPos ) {
    rstring* newStr;
    char* destText;
    char* sourceText;
    size_t sourceLength;
    size_t subLength;

    if( source == NULL ) {
      return NULL;
    }

    sourceLength = rstrlen(source);
    sourceText = rstrtext(source);

    subLength = (endPos - startPos) + 1;
    if( subLength >= sourceLength ) {
      return NULL;
    }

    newStr = rstralloc(subLength);
    if( newStr == NULL ) {
      return NULL;
    }
    newStr->length = subLength;

    sourceText = rstrtext(source);
    destText = rstrtext(newStr);
    memcpy( destText, (sourceText + startPos), subLength );
    destText[subLength] = '\0';

    return newStr;
  }

  rstring* rsubstrc( char* source, size_t sourceLength,
      size_t startPos, size_t endPos ) {
    rstring* newStr;
    char* destText;
    size_t subLength;

    if( source == NULL ) {
      return NULL;
    }

    if( sourceLength == 0 ) {
      sourceLength = strlen(source);
    }

    subLength = (endPos - startPos) + 1;
    if( subLength >= sourceLength ) {
      return NULL;
    }

    newStr = rstralloc(subLength);
    if( newStr == NULL ) {
      return NULL;
    }
    newStr->length = subLength;

    destText = rstrtext(newStr);
    memcpy( destText, (source + startPos), subLength );
    destText[subLength] = '\0';

    return newStr;
  }

/*
 *  Symbol table implementation
 */

/*
 *  Symbol implementation
 */

/*
 *  Lexer implementation
 */

  SourceFile* OpenSource( rstring* name ) {
    return NULL;
  }

  void CloseSource( SourceFile** sourceFile ) {
  }

  bool ReadChar( SourceFile* sourceFile ) {
    return false;
  }

  rstring* ReadIdent( SourceFile* sourceFile, rstring* destIdent ) {
    return NULL;
  }

  bool ReadInt( FILE* sourceFile, void* destInt ) {
    return false;
  }

  rstring* ReadString( FILE* sourceFile, rstring* destString ) {
    return NULL;
  }

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
