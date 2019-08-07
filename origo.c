
#include <ctype.h>
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
      tkAt,
      tkHash,

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
    valImmediate = (2 << 9),
      valInt = (valImmediate + (0 << 5)),
        valInt8,
        valInt16,
        valInt32,

      valUint = (valImmediate + (1 << 5)),
        valUint8,
        valUint16,
        valUint32,

      valChar = (valImmediate + (2 << 5)),

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

    // x86 operand tokens
    x86Operand = (5 << 9),
        x86OperandMask = (x86Operand | (15 << 4)),
        x86SubOperandMask = (~3),

      // x86 memory address
      x86Mem  = (x86Operand + (0 << 4) + 0),

      // x86 segment/selector register tokens
      x86SReg = (x86Operand + (1 << 4) + 0),
        x86SRegES = x86SReg,
        x86SRegCS,
        x86SRegSS,
        x86SRegDS,
        x86SRegFS,
        x86SRegGS,
        lastX86SReg = x86SRegGS,

      // x86 8-bit register tokens
      x86Reg8 = (x86Operand + (2 << 4) + 0),
        x86RegAL = x86Reg8,
        x86RegCL,
        x86RegDL,
        x86RegBL,
        x86RegAH,
        x86RegCH,
        x86RegDH,
        x86RegBH,

      // x86 16-bit register tokens
      x86Reg16 =(x86Operand + (3 << 4) + 0),
        x86RegAX = x86Reg16,
        x86RegCX,
        x86RegDX,
        x86RegBX,
        x86RegSP,
        x86RegBP,
        x86RegSI,
        x86RegDI,

      // x86 32-bit register tokens
      x86Reg32 = (x86Operand + (4 << 4) + 0),
        x86RegEAX = x86Reg32,
        x86RegECX,
        x86RegEDX,
        x86RegEBX,
        x86RegESP,
        x86RegEBP,
        x86RegESI,
        x86RegEDI,

      // x86 mnemonic tokens
    x86Ident = (6 << 9),
      x86Call,
        firstX86Ident = x86Call,
      x86Push,
      x86Cbw,
      x86Cwd,
      x86Clc,
      x86Cld,
      x86Cli,
      x86Cmc,
      x86Int3,
      x86Iret,
      x86Lahf,
      x86Nop,
      x86Popf,
      x86Pushf,
      x86Ret,
      x86Retf,
      x86Sahf,
      x86Stc,
      x86Std,
      x86Sti,
      x86Xlat,
      x86Xlatb,
      x86Adc,
      x86Add,
      x86And,
      x86Cmp,
      x86Dec,
      x86Div,
      x86IDiv,
      x86IMul,
      x86In,
      x86Inc,
      x86Int,
      x86Jo,
        x86JCC = x86Jo,
      x86Jno,
      x86Jb,
        x86Jc = x86Jb,
        x86Jnae = x86Jb,
      x86Jnb,
        x86Jnc = x86Jnb,
        x86Jae = x86Jnb,
      x86Je,
        x86Jz = x86Je,
      x86Jne,
        x86Jnz = x86Jne,
      x86Jbe,
        x86Jna = x86Jbe,
      x86Jnbe,
        x86Ja = x86Jnbe,
      x86Js,
      x86Jns,
      x86Jp,
        x86Jpe = x86Jp,
      x86Jnp,
        x86Jpo = x86Jnp,
      x86Jl,
        x86Jnge = x86Jl,
      x86Jnl,
        x86Jge = x86Jnl,
      x86Jle,
        x86Jng = x86Jle,
      x86Jnle,
        x86Jg = x86Jnle,
      x86Rep,
      x86Repe,
      x86Repne,
      x86Repz,
      x86Repnz,
      x86Lodsb,
      x86Lodsw,
      x86Movsb,
      x86Movsw,
      x86Stosb,
      x86Stosw,
      x86Cmpsb,
      x86Cmpsw,
      x86Scasb,
      x86Scasw,
      x86Jcxz,
      x86Jmp,
      x86Lds,
      x86Les,
      x86Lea,
      x86Loop,
      x86Loope,
      x86Loopne,
      x86Loopz,
      x86Loopnz,
      x86Mov,
      x86Mul,
      x86Neg,
      x86Not,
      x86Or,
      x86Out,
      x86Pop,
      x86Rcl,
      x86Rcr,
      x86Rol,
      x86Ror,
      x86Sal,
      x86Sar,
      x86Shl,
      x86Shr,
      x86Sbb,
      x86Sub,
      x86Test,
      x86Xchg,
      x86Xor,
        lastX86Ident = x86Xor
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
  size_t rstrrsvdlen( rstring* source );
  char* rstrtext( rstring* source );

  rstring* rstralloc( size_t reserveLength );
  rstring* rstrzalloc( size_t reserveLength );

  rstring* rstrcopy( rstring* source );
  rstring* rstrcopyc( char* source, size_t sourceLength );

  rstring* rstrappendch( rstring* dest, char ch );

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

  typedef struct RetFile {
    FILE* handle;
    rstring* fileName;
    unsigned line;
    unsigned col;
    char curCh;
    char nextCh;
  } RetFile;

  RetFile* OpenRet( char* fileName, size_t nameLength );
  void CloseRet( RetFile** source );

  bool ReadChar( RetFile* source );

  bool SkipSpace( RetFile* source );
  bool SkipComments( RetFile* source );

  bool ReadIdent( RetFile* source, rstring** ident, unsigned* hashCode );

  bool ReadBinNum( RetFile* source, unsigned* num );
  bool ReadOctalNum( RetFile* source, unsigned* num );
  bool ReadHexNum( RetFile* source, unsigned* num );
  bool ReadNum( RetFile* source, unsigned* num );

  bool ReadString( RetFile* source, rstring** string, unsigned* hashCode );

  unsigned ReadOperator( RetFile* source );

/*
 *  Windows PE declarations
 */

  typedef struct WinPE {
    FILE* handle;
    size_t entryPoint;
    unsigned sizeFlag;
    uint8_t sizePrefix;
  } WinPE;

  typedef struct WinPEHeader {
    ///TODO: Define barebones PE header
  } WinPEHeader;

  WinPE* CreatePE( char* fileName );
  void ClosePE( WinPE** winpe );

/*
 *  Code generator declarations
 */

  typedef enum x86Field {
    hasPrefix1 = 1 << 16,
    hasPrefix2 = 1 << 15,
    hasPrefix3 = 1 << 14,
    hasPrefix4 = 1 << 13,

    hasOpcode1 = 1 << 12,
    hasOpcode2 = 1 << 11,
    hasOpcode3 = 1 << 10,

    hasModRM   = 1 << 9,

    hasSIB     = 1 << 8,

    hasDisp1   = 1 << 7,
    hasDisp2   = 1 << 6,
    hasDisp3   = 1 << 5,
    hasDisp4   = 1 << 4,

    hasImm1    = 1 << 3,
    hasImm2    = 1 << 2,
    hasImm3    = 1 << 1,
    hasImm4    = 1 << 0,

    hasDisp8 = hasDisp1,
    hasDisp16 = hasDisp1 | hasDisp2,
    hasDisp32 = hasDisp1 | hasDisp2 | hasDisp3 | hasDisp4,

    hasImm8 = hasImm1,
    hasImm16 = hasImm1 | hasImm2,
    hasImm32 = hasImm1 | hasImm2 | hasImm3 | hasImm4,
  } x86Field;

  typedef struct x86Instruction {
    unsigned fields;
    uint8_t prefix[4];
    uint8_t opcode[3];
    uint8_t modRM;
    uint8_t sib;
    uint32_t displacement;
    uint32_t immediate;
  } x86Instruction;

  typedef struct x86Addr {
    unsigned baseReg;
    unsigned indexReg;
    unsigned scale;
    int32_t  displacement;
  } x86Addr;

  typedef struct x86FormatParam {
    unsigned first;
    unsigned last;
  } x86FormatParam;

  typedef struct x86Format {
    unsigned mnemonic;
    x86FormatParam param[3];
    unsigned index;
  } x86Format;

  enum PrefixFlags {
    hasLock = hasPrefix1,
    hasRepe = hasPrefix1,
    hasRepne = hasPrefix1,
    hasRepz = hasPrefix1,
    hasRepnz = hasPrefix1,

    hasCS = hasPrefix2,
    hasSS = hasPrefix2,
    hasDS = hasPrefix2,
    hasES = hasPrefix2,
    hasFS = hasPrefix2,
    hasGS = hasPrefix2,
    hasBranch = hasPrefix2,
    hasNoBranch = hasPrefix2,

    hasOperandSize = hasPrefix3,

    hasAddressSize = hasPrefix4
  };

  enum PrefixIndices {
    indexLock = 0,
    indexRepe = 0,
    indexRepne = 0,
    indexRepz = 0,
    indexRepnz = 0,

    indexCS = 1,
    indexSS = 1,
    indexDS = 1,
    indexES = 1,
    indexFS = 1,
    indexGS = 1,
    indexBranch = 1,
    indexNoBranch = 1,

    indexOperandSize = 2,

    indexAddressSize = 3
  };

  typedef struct x86Encoding {
    unsigned fields;
    uint8_t prefix[4];
    uint8_t opcode[3];
    uint8_t modRM;
    uint8_t xformOp;
    uint8_t xform[3];
  } x86Encoding;

  // Global variable
  typedef struct TargetEnv {
    uint8_t bits;
    uint8_t operandWBits;
    uint8_t operandWPrefix;
    uint8_t addressWBits;
    uint8_t addressWPrefix;
  } TargetEnv;

  TargetEnv target = {
    32,
    16,
    0x66,
    16,
    0x67
  };

  bool x86Emit( FILE* binFile, x86Instruction* instruction );

  bool x86EncodeAddr16( x86Instruction* instruction, x86Addr* addr16 );
  bool x86EncodeAddr32( x86Instruction* instruction, x86Addr* addr32 );

  bool x86GenOpRegMem( FILE* binFile, unsigned mnemonic, x86Addr* addr );

  bool x86GenOpRegImm( FILE* binFile,
    unsigned mnemonic, unsigned reg, unsigned imm );

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
 *  Temp test functions
 */

;;;
  bool TestAddr32( FILE* binFile, unsigned destReg, unsigned baseReg, unsigned indexReg, uint8_t scale, int displacement ) {
    x86Instruction instruction = {};
    x86Addr addr32 = {};

    if( binFile == NULL ) {
      return false;
    }

    instruction.fields |= hasOpcode1 | hasModRM;
    instruction.opcode[0] = 0x12;
    instruction.modRM = (destReg - x86Reg8) << 3;

    addr32.baseReg = baseReg;
    addr32.indexReg = indexReg;
    addr32.scale = scale;
    addr32.displacement = displacement;

    if( x86EncodeAddr32(&instruction, &addr32) ) {
      return x86Emit(binFile, &instruction);
    }

    return false;
  }

/*
 *  Main program
 */

  void PrintBanner() {
    printf( "\nOrigo Alpha\n" );
    printf( "Copyright 2019 Orlando Llanes\n\n" );
  }

  void PrintUsage() {
    printf( "usage: origo source[.ret] [binary[.ext]]\n" );
  }

int main( int argc, char* argv[] ) {
  argCount = argc;
  argVar = argv;

  PrintBanner();

  if( argc < 2 ) {
    PrintUsage();
    return 1;
  }

  FILE* binFile = fopen("out", "wb");
  if( binFile ) {
;;;
/*
    // [base]
    TestAddr32( binFile, x86RegCL, x86RegEAX,  0, 0, 0 );
    TestAddr32( binFile, x86RegBH, x86RegEDI,  0, 0, 0 );
    TestAddr32( binFile, x86RegCL, x86RegESP,  0, 0, 0 );
    TestAddr32( binFile, x86RegCL, x86RegEBP,  0, 0, 0 );
*/

    // [disp]
    TestAddr32( binFile, x86RegCL, 0,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegBH, 0,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegCL, 0,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0,  0, 0, 0x11223344 );

/*
    // [base + disp]
    TestAddr32( binFile, x86RegCL, x86RegEAX,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegBH, x86RegEDI,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegCL, x86RegEAX,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegBH, x86RegEDI,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegCL, x86RegESP,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegBH, x86RegESP,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegCL, x86RegESP,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegBH, x86RegESP,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegCL, x86RegEBP,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegBH, x86RegEBP,  0, 0, 0x11 );
    TestAddr32( binFile, x86RegCL, x86RegEBP,  0, 0, 0x11223344 );
    TestAddr32( binFile, x86RegBH, x86RegEBP,  0, 0, 0x11223344 );

    // [index * scale]
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  1, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  1, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  1, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  1, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  2, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  2, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  2, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  2, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  4, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  4, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  4, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  4, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  8, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  8, 0 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  8, 0 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  8, 0 );

    // [index * scale + disp]
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  1, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  1, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  1, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  1, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  2, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  2, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  2, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  2, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  4, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  4, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  4, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  4, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEAX,  8, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEDI,  8, 0x11223344 );
    TestAddr32( binFile, x86RegCL, 0, x86RegEBP,  8, 0x11223344 );
    TestAddr32( binFile, x86RegBH, 0, x86RegEBP,  8, 0x11223344 );
*/

    fclose( binFile );
    binFile = NULL;
  }

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

  inline size_t rstrrsvdlen( rstring* source ) {
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

  rstring* rstrcopy( rstring* source ) {
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

  rstring* rstrcopyc( char* source, size_t sourceLength ) {
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
    destRsvd = rstrrsvdlen(dest);

    if( (destRsvd - destLength) < 2 ) {
      destSize = (destRsvd + 8) & (~7);

      destTemp = realloc(dest, sizeof(rstring) + destSize);
      if( destTemp == NULL ) {
        return NULL;
      }
      dest = destTemp;

      dest->length = destLength;
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
    destRsvd = rstrrsvdlen(dest);

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
    destRsvd = rstrrsvdlen(dest);

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

  RetFile* OpenRet( char* fileName, size_t nameLength ) {
    RetFile* newSource = NULL;

    if( !(fileName && (*fileName)) ) {
      goto ReturnError;
    }

    newSource = calloc(1, sizeof(RetFile));
    if( newSource == NULL ) {
      goto ReturnError;
    }

    newSource->fileName = rstrcopyc(fileName, nameLength);
    if( newSource->fileName == NULL ) {
      goto ReturnError;
    }

    newSource->handle = fopen(fileName, "rb");
    if( newSource->handle == NULL ) {
      goto ReturnError;
    }

    newSource->line = 1;
    newSource->col = 1;

    ReadChar( newSource );
    ReadChar( newSource );

    newSource->line = 1;
    newSource->col = 1;

    return newSource;

  ReturnError:
    CloseRet( &newSource );

    return NULL;
  }

  void CloseRet( RetFile** source ) {
    if( source ) {
      if( (*source) ) {
        if( (*source)->handle ) {
          fclose( (*source)->handle );
          (*source)->handle = NULL;
        }

        if( (*source)->fileName ) {
          free( (*source)->fileName );
          (*source)->fileName = NULL;
        }

        free( (*source) );
        (*source) = NULL;
      }
    }
  }

  bool ReadChar( RetFile* source ) {
    unsigned colInc = 1;
    char tmpCh = 0;

    if( !(source && source->handle) ) {
      return false;
    }

    if( source->curCh == 10 ) {
      source->line++;
      source->col = 1;
      colInc = 0;
    }

    source->curCh = source->nextCh;
    source->nextCh = 0;

    if( fread(&(source->nextCh), 1, sizeof(char),
        source->handle) != sizeof(char) ) {
      return false;
    }

    if( source->nextCh == 13 ) {
      if( fread(&tmpCh, 1, sizeof(char),
          source->handle) != sizeof(char) ) {
        return false;
      }

      if( tmpCh != 10 ) {
        if( fseek(source->handle, -1, SEEK_CUR) == 0 ) {
          return false;
        }
      }

      source->nextCh = 10;
    }

    source->col += colInc;

    return true;
  }

  bool SkipSpace( RetFile* source ) {
    if( source == NULL ) {
      return false;
    }

    if( isspace(source->curCh) ) {
      while( isspace(source->curCh) ) {
        if( !ReadChar(source) ) {
          return false;
        }
      }
      return true;
    }

    return false;
  }

  bool SkipComments( RetFile* source ) {
    unsigned level;
    unsigned loopFlags;

    if( source == NULL ) {
      return false;
    }

    do {
      loopFlags = 0;

      // Skip space characters
      if( isspace(source->curCh) ) {
        while( isspace(source->curCh) ) {
          if( !ReadChar(source) ) {
            return false;
          }
        }
        loopFlags |= 1;
      }

      // Skip single-line comments
      if( (source->curCh == '/') && (source->nextCh == '/') ) {
        do {
          if( !ReadChar(source) ) {
            return false;
          }
        } while( source->curCh != 10 );
        loopFlags |= (1 << 1);
      }

      // Skip multi-line comments
      if( (source->curCh == '/') && (source->nextCh == '*') ) {
        level = 0;
        do {
          if( (source->curCh == '/') && (source->nextCh == '*') ) {
            if( level == (-1) ) {
              return false;
            }
            level++;
            if( !(ReadChar(source) && ReadChar(source)) ) {
              return false;
            }
            continue;
          }

          if( (source->curCh == '*') && (source->nextCh == '/') ) {
            if( level == 0 ) {
              return false;
            }
            level--;
            if( !(ReadChar(source) && ReadChar(source)) ) {
              return false;
            }
            continue;
          }

          if( !ReadChar(source) ) {
            return false;
          }
        } while( level );

        loopFlags |= (1 << 2);
      }
    } while( loopFlags );

    return true;
  }

  bool ReadIdent( RetFile* source, rstring** ident, unsigned* hashCode ) {
    rstring* dest = NULL;
    rstring* tmpDest = NULL;
    unsigned runningHash = 0;

    if( !(source && ident && hashCode) ) {
      return false;
    }

    dest = (*ident);

    if( !((source->curCh == '_') || isalnum(source->curCh)) ) {
      return false;
    }

    while( (source->curCh == '_') || isalnum(source->curCh) ) {
      tmpDest = rstrappendch(dest, source->curCh);
      if( tmpDest == NULL ) {
        (*ident) = dest;
        return false;
      }
      dest = tmpDest;

      if( !ReadChar(source) ) {
        (*ident) = dest;
        return false;
      }
    }

    (*ident) = dest;
    (*hashCode) = runningHash;
    return true;
  }

  bool ReadBinNum( RetFile* source, unsigned* num ) {
    unsigned result = 0;

    if( !(source && num) ) {
      return false;
    }

    if( (source->curCh != '0') && (source->nextCh != 'b') ) {
      return false;
    }

    if( !(ReadChar(source) && ReadChar(source)) ) {
      return false;
    }

    while( source->curCh ) {
      if( (source->curCh >= '0') && (source->curCh <= '1') ) {
        result = (result << 1) + (source->curCh - '0');
      } else if( source->curCh == '_' ) {
      } else {
        break;
      }

      if( !ReadChar(source) ) {
        return false;
      }
    }

    *num = result;
    return true;
  }

  bool ReadOctalNum( RetFile* source, unsigned* num ) {
    unsigned result = 0;

    if( !(source && num) ) {
      return false;
    }

    if( (source->curCh != '0') && (source->nextCh != 'o') ) {
      return false;
    }

    if( !(ReadChar(source) && ReadChar(source)) ) {
      return false;
    }

    while( source->curCh ) {
      if( (source->curCh >= '0') && (source->curCh <= '7') ) {
        result = (result << 3) + (source->curCh - '0');
      } else if( source->curCh == '_' ) {
      } else {
        break;
      }

      if( !ReadChar(source) ) {
        return false;
      }
    }

    *num = result;
    return true;
  }

  bool ReadHexNum( RetFile* source, unsigned* num ) {
    unsigned result = 0;

    if( !(source && num) ) {
      return false;
    }

    if( (source->curCh != '0') && (source->nextCh != 'x') ) {
      return false;
    }

    if( !(ReadChar(source) && ReadChar(source)) ) {
      return false;
    }

    while( source->curCh ) {
      if( (source->curCh >= '0') && (source->curCh <= '9') ) {
        result = (result << 4) + (source->curCh - '0');
      } else if( (source->curCh >= 'a') && (source->curCh <= 'f') ) {
        result = (result << 4) + (source->curCh - 'a') + 10;
      } else if( (source->curCh >= 'A') && (source->curCh <= 'F') ) {
        result = (result << 4) + (source->curCh - 'A') + 10;
      } else if( source->curCh == '_' ) {
      } else {
        break;
      }

      if( !ReadChar(source) ) {
        return false;
      }
    }

    *num = result;
    return true;
  }

  bool ReadNum( RetFile* source, unsigned* num ) {
    unsigned result = 0;

    if( !(source && num) ) {
      return false;
    }

    // Check for other types of integers
    if( source->curCh == '0' ) {
      switch( source->nextCh ) {
      case 'b':
        return ReadBinNum(source, num);

      case 'o':
        return ReadOctalNum(source, num);

      case 'x':
        return ReadHexNum(source, num);
      }
    }

    // Skip leading zero's
    while( source->curCh == '0' ) {
      if( !ReadChar(source) ) {
        return false;
      }
    }

    // Tokenize decimal
    while( (source->curCh == '_') || isdigit(source->curCh) ) {
      if( source->curCh != '_' ) {
        result = (result * 10) + ((source->curCh) - '0');
      }
      if( !ReadChar(source) ) {
        return false;
      }
    }

    *num = result;
    return true;
  }

  bool ReadString( RetFile* source, rstring** string, unsigned* hashCode ) {
    rstring* dest = NULL;
    rstring* tmpDest = NULL;
    unsigned runningHash = 0;
    char quoteCh;

    if( !(source && string && hashCode) ) {
      return false;
    }

    dest = (*string);

    quoteCh = source->curCh;
    if( !((quoteCh == '\"') || (quoteCh == '\'')) ) {
      return false;
    }

    if( !ReadChar(source) ) {
      return false;
    }

    while( source->curCh != quoteCh ) {
      tmpDest = rstrappendch(dest, source->curCh);
      if( tmpDest == NULL ) {
        (*string) = dest;
        return false;
      }
      dest = tmpDest;

      if( !ReadChar(source) ) {
        (*string) = dest;
        return false;
      }
    }

    // Skip closing quote character
    if( !ReadChar(source) ) {
      (*string) = dest;
      return false;
    }

    (*string) = dest;
    (*hashCode) = runningHash;
    return true;
  }

  typedef struct Operator {
    char* text;
    unsigned token;
  } Operator;

  const Operator operTable[] = {
    "!",    unaryIsNot,
    "!=",   opNotEq,
    "#",    tkHash,
    "%",    opMod,
    "%=",   assignMod,
    "&",    opAnd,
    "&&",   opAndIs,
    "&=",   assignAnd,
    "(",    tkLParen,
    ")",    tkRParen,
    "*",    opMul,
    "*=",   assignMul,
    "+",    opAdd,
    "++",   opPostInc,
    "+=",   assignAdd,
    ",",    tkComma,
    "-",    opSub,
    "--",   opPostDec,
    "-=",   assignSub,
    ".",    tkDot,
    "..",   tkDotDot,
    "/",    opDiv,
    "/=",   assignDiv,
    ":",    tkColon,
    "<",    opLT,
    "<-<",  opSRol,
    "<-<=", assignSRol,
    "<<",   opShl,
    "<<<",  opRol,
    "<<<=", assignRol,
    "<<=",  assignShl,
    "<=",   opLTEq,
    "=",    assignTo,
    "==",   opEq,
    ">",    opGT,
    ">->",  opSRor,
    ">->=", assignSRor,
    ">=",   opGTEq,
    ">>",   opShr,
    ">>=",  assignShr,
    ">>>",  opRor,
    ">>>=", assignRor,
    "@",    tkAt,
    "[",    tkLBrace,
    "[[",   tkLDoubleBrace,
    "]",    tkRBrace,
    "]]",   tkRDoubleBrace,
    "^",    opXor,
    "^=",   assignXor,
    "|",    opOr,
    "|=",   assignOr,
    "|>>",  opSShr,
    "|>>=", assignSShr,
    "||",   opOrIs,
    "~",    unaryNot,
    "~=",   assignNot
  };
  const size_t numOpers = sizeof(operTable) / sizeof(operTable[0]);

  unsigned ReadOperator( RetFile* source ) {
    char operator[8] = {};
    size_t leftIndex;
    size_t operIndex;
    size_t rightIndex;
    size_t operLength = 0;
    int result;
    unsigned token = 0;

    if( !(source && ispunct(source->curCh)) ) {
      return 0;
    }

    while( ispunct(source->curCh) ) {
      if( operLength > 7 ) {
        break;
      }

      operator[operLength++] = source->curCh;

      leftIndex = 0;
      rightIndex = numOpers;
      operIndex = numOpers / 2;

      while( leftIndex < rightIndex ) {
        result = strcmp(operTable[operIndex].text, operator);
        if( result == 0 ) {
          token = operTable[operIndex].token;
          break;
        }

        if( result > 0 ) {
          rightIndex = operIndex;
        } else {
          leftIndex = operIndex + 1;
        }

        operIndex = (leftIndex + rightIndex) / 2;
      }

      if( leftIndex > rightIndex ) {
        return token;
      }

      if( !ReadChar(source) ) {
        return token;
      }
    }

    return token;
  }

/*
 *  Windows PE implementation
 */

  WinPE* CreatePE( char* fileName ) {
    WinPE* newPE = NULL;
    FILE* dosStub = NULL;

    if( !(fileName && (*fileName)) ) {
      return NULL;
    }

    newPE = calloc(1, sizeof(WinPE));
    if( newPE == NULL ) {
      goto ReturnError;
    }

    newPE->handle = fopen(fileName, "wb");
    if( newPE->handle == NULL ) {
      goto ReturnError;
    }

    ///TODO: Write default header

    ///TODO: Write MS-DOS stub

    return newPE;

  ReturnError:
    if( dosStub ) {
      fclose( dosStub );
      dosStub = NULL;
    }
    ClosePE( &newPE );
    return NULL;
  }

  void ClosePE( WinPE** winpe ) {
    if( winpe ) {
      if( (*winpe) ) {
        if( (*winpe)->handle ) {
          ///TODO: Finalize header/etc.

          fclose( (*winpe)->handle );
          (*winpe)->handle = NULL;
        }

        free( (*winpe) );
        (*winpe) = NULL;
      }
    }
  }

/*
 *  Code generator implementation
 */

  const size_t formatStart[] = {
    /* x86Call   */ -1,
    /* x86Push   */ -1,
    /* x86Cbw    */ -1,
    /* x86Cwd    */ -1,
    /* x86Clc    */ -1,
    /* x86Cld    */ -1,
    /* x86Cli    */ -1,
    /* x86Cmc    */ -1,
    /* x86Int3   */ -1,
    /* x86Iret   */ -1,
    /* x86Lahf   */ -1,
    /* x86Nop    */ -1,
    /* x86Popf   */ -1,
    /* x86Pushf  */ -1,
    /* x86Ret    */ -1,
    /* x86Retf   */ -1,
    /* x86Sahf   */ -1,
    /* x86Stc    */ -1,
    /* x86Std    */ -1,
    /* x86Sti    */ -1,
    /* x86Xlat   */ -1,
    /* x86Xlatb  */ -1,
    /* x86Adc    */ 0,
    /* x86Add    */ -1,
    /* x86And    */ -1,
    /* x86Cmp    */ -1,
    /* x86Dec    */ -1,
    /* x86Div    */ -1,
    /* x86IDiv   */ -1,
    /* x86IMul   */ -1,
    /* x86In     */ -1,
    /* x86Inc    */ -1,
    /* x86Int    */ -1,
    /* x86Jo     */ -1,
    /* x86Jno    */ -1,
    /* x86Jb     */ -1,
    /* x86Jnb    */ -1,
    /* x86Je     */ -1,
    /* x86Jne    */ -1,
    /* x86Jbe    */ -1,
    /* x86Jnbe   */ -1,
    /* x86Js     */ -1,
    /* x86Jns    */ -1,
    /* x86Jp     */ -1,
    /* x86Jnp    */ -1,
    /* x86Jl     */ -1,
    /* x86Jnl    */ -1,
    /* x86Jle    */ -1,
    /* x86Jnle   */ -1,
    /* x86Rep    */ -1,
    /* x86Repe   */ -1,
    /* x86Repne  */ -1,
    /* x86Repz   */ -1,
    /* x86Repnz  */ -1,
    /* x86Lodsb  */ -1,
    /* x86Lodsw  */ -1,
    /* x86Movsb  */ -1,
    /* x86Movsw  */ -1,
    /* x86Stosb  */ -1,
    /* x86Stosw  */ -1,
    /* x86Cmpsb  */ -1,
    /* x86Cmpsw  */ -1,
    /* x86Scasb  */ -1,
    /* x86Scasw  */ -1,
    /* x86Jcxz   */ -1,
    /* x86Jmp    */ -1,
    /* x86Lds    */ -1,
    /* x86Les    */ -1,
    /* x86Lea    */ -1,
    /* x86Loop   */ -1,
    /* x86Loope  */ -1,
    /* x86Loopne */ -1,
    /* x86Loopz  */ -1,
    /* x86Loopnz */ -1,
    /* x86Mov    */ -1,
    /* x86Mul    */ -1,
    /* x86Neg    */ -1,
    /* x86Not    */ -1,
    /* x86Or     */ -1,
    /* x86Out    */ -1,
    /* x86Pop    */ -1,
    /* x86Rcl    */ -1,
    /* x86Rcr    */ -1,
    /* x86Rol    */ -1,
    /* x86Ror    */ -1,
    /* x86Sal    */ -1,
    /* x86Sar    */ -1,
    /* x86Shl    */ -1,
    /* x86Shr    */ -1,
    /* x86Sbb    */ -1,
    /* x86Sub    */ -1,
    /* x86Test   */ -1,
    /* x86Xchg   */ -1,
    /* x86Xor    */ -1
  };
  const formatStartCount = sizeof(formatStart) / sizeof(formatStart[0]);

  // mnemonic,
  // param1.first, param1.last, param2.first, param2.last, param3.first, param3.last,
  // index
  const x86Format formatTable[] = {
    /* 0 */ x86Adc, x86RegAL, x86RegAL, valUint, valUint32, 0, 0, 0,
    /* 1 */ x86Adc, x86RegAX, x86RegAX, valUint, valUint32, 0, 0, 1,
    /* 2 */ x86Adc, x86RegEAX, x86RegEAX, valUint, valUint32, 0, 0, 1,
    /* 3 */ x86Adc, x86RegAL, x86RegBH, valUint, valUint32, 0, 0, 2,
    /* 4 */ x86Adc, x86RegAX, x86RegDI, valUint, valUint32, 0, 0, 3,
    /* 5 */ x86Adc, x86RegEAX, x86RegEDI, valUint, valUint32, 0, 0, 3,
  };
  const formatCount = sizeof(formatTable) / sizeof(formatTable[0]);

  enum x86Transform {
    xformOpW = 1, // Add operand size prefix
    xformModReg, // Add register to ModRM bits 0..2
    xformImm8  // S bit in last opcode if imm in [-128..127]
  };

  // fields,
  // prefix1, prefix2, prefix3, prefix4,
  // opcode1, opcode2, opcode3, modRM,
  // xformop, xform1, xform2, xform3
  const x86Encoding encodeTable[] = {
    /* 0 */ hasOpcode1, 0, 0, 0, 0, 0x14, 0, 0, 0, 0, 0, 0, 0,
    /* 1 */ hasOpcode1, 0, 0, 0, 0, 0x15, 0, 0, 0, xformOpW, 0, 0, 0,
    /* 2 */ hasOpcode1 | hasModRM, 0, 0, 0, 0, 0x80, 0, 0, 0xD0, 0, xformModReg, 0, 0,
    /* 3 */ hasOpcode1 | hasModRM, 0, 0, 0, 0, 0x81, 0, 0, 0xD0, xformOpW, xformModReg, xformImm8, 0
  };
  const encodeCount = sizeof(encodeTable) / sizeof(encodeTable[0]);

  bool x86Emit( FILE* binFile, x86Instruction* instruction ) {
    uint8_t machineCode[sizeof(x86Instruction)];
    unsigned codeLength = 0;
    unsigned fields;
    unsigned fieldBit;
    unsigned index;

    if( !(binFile && instruction) ) {
      return false;
    }

    fields = instruction->fields;

    if( fields & hasPrefix1 ) {
      machineCode[codeLength++] = instruction->prefix[0];
    }

    if( fields & hasPrefix2 ) {
      machineCode[codeLength++] = instruction->prefix[1];
    }

    if( fields & hasPrefix3 ) {
      machineCode[codeLength++] = instruction->prefix[2];
    }

    if( fields & hasPrefix4 ) {
      machineCode[codeLength++] = instruction->prefix[3];
    }

    if( fields & hasOpcode1 ) {
      machineCode[codeLength++] = instruction->opcode[0];
    }

    if( fields & hasOpcode2 ) {
      machineCode[codeLength++] = instruction->opcode[1];
    }

    if( fields & hasOpcode3 ) {
      machineCode[codeLength++] = instruction->opcode[2];
    }

    if( fields & hasModRM ) {
      machineCode[codeLength++] = instruction->modRM;
    }

    if( fields & hasSIB ) {
      machineCode[codeLength++] = instruction->sib;
    }

    switch( fields & hasDisp32 ) {
    case hasDisp8:
      machineCode[codeLength++] = instruction->displacement;
      break;

    case hasDisp16:
      machineCode[codeLength++] = instruction->displacement;
      machineCode[codeLength++] = instruction->displacement >> 8;
      break;

    case hasDisp32:
      machineCode[codeLength++] = instruction->displacement;
      machineCode[codeLength++] = instruction->displacement >> 8;
      machineCode[codeLength++] = instruction->displacement >> 16;
      machineCode[codeLength++] = instruction->displacement >> 24;
      break;
    }

    switch( fields & hasImm32 ) {
    case hasImm8:
      machineCode[codeLength++] = instruction->immediate;
      break;

    case hasImm16:
      machineCode[codeLength++] = instruction->immediate;
      machineCode[codeLength++] = instruction->immediate >> 8;
      break;

    case hasImm32:
      machineCode[codeLength++] = instruction->immediate;
      machineCode[codeLength++] = instruction->immediate >> 8;
      machineCode[codeLength++] = instruction->immediate >> 16;
      machineCode[codeLength++] = instruction->immediate >> 24;
      break;
    }

    return (codeLength &&
        (fwrite(machineCode, 1, codeLength, binFile) == codeLength));
  }

  enum AddrReg16 {
    indexSI = 1,
    indexDI = 2,
    baseBX = 4,
    baseBP = 8
  };

  const uint8_t x86Mem16Table[16] = {
    /*  0 */ 0x38, // Invalid
    /*  1 */ 0x04, // [SI]
    /*  2 */ 0x05, // [DI]
    /*  3 */ 0x38, // Invalid
    /*  4 */ 0x07, // [BX]
    /*  5 */ 0x00, // [BX + SI]
    /*  6 */ 0x01, // [BX + DI]
    /*  7 */ 0x38, // Invalid
    /*  8 */ 0x06, // [BP + <DISP8 | DISP16>] if not [DISP16]
    /*  9 */ 0x02, // [BP + SI]
    /* 10 */ 0x03, // [BP + DI]
    /* 11 */ 0x38, // Invalid
    /* 12 */ 0x38, // Invalid
    /* 13 */ 0x38, // Invalid
    /* 14 */ 0x38, // Invalid
    /* 15 */ 0x38  // Invalid
  };

  bool x86EncodeAddr16( x86Instruction* instruction, x86Addr* addr16 ) {
    unsigned baseReg = 0;
    unsigned indexReg = 0;
    unsigned iteration;
    int displacement;
    uint8_t modRM;
    uint8_t xlateRM;

    if( !(instruction && addr16) ) {
      return false;
    }
    modRM = instruction->modRM;
    displacement = addr16->displacement;

    switch( addr16->baseReg ) {
    case 0:
      break;

    case x86RegBX:
      baseReg = baseBX;
      break;

    case x86RegBP:
      baseReg = baseBP;
      break;

    default:
      return false;
    }

    switch( addr16->indexReg ) {
    case 0:
      break;

    case x86RegSI:
      indexReg = indexSI;
      break;

    case x86RegDI:
      indexReg = indexDI;
      break;

    default:
      return false;
    }

    if( (baseReg | indexReg) > 15 ) {
      return false;
    }

    xlateRM = x86Mem16Table[baseReg | indexReg];
    if( xlateRM == 0x38 ) {
      return false;
    }
    modRM |= xlateRM;

    instruction->fields |= hasModRM;
    instruction->modRM |= modRM;

    if( (baseReg != baseBP) && (displacement == 0) ) {
      // [base + index]
    } else if( (displacement >= -128) && (displacement <= 127) ) {
      // [base + index + disp8]
      instruction->fields |= hasDisp8;
      instruction->modRM |= 0x40;
      instruction->displacement = displacement;
    } else {
      // Assume [base + index + disp16]
      instruction->fields |= hasDisp16;
      instruction->modRM |= 0x80;
      instruction->displacement = displacement;
    }

    if( target.bits != 16 ) {
      instruction->fields |= hasAddressSize;
      instruction->prefix[indexAddressSize] = target.addressWPrefix;
    }

    return true;
  }

;;;
  bool x86EncodeAddr32( x86Instruction* instruction, x86Addr* addr32 ) {
    x86Instruction tempInstruction = {};
    x86Addr tempAddr = {};

    if( !(instruction && addr32 && (addr32->indexReg != x86RegESP)) ) {
      return false;
    }

    tempInstruction = *instruction;
    tempAddr = *addr32;

    if( (tempAddr.baseReg | tempAddr.indexReg) == 0 ) {
      tempInstruction.fields |= hasModRM | hasDisp32;
      tempInstruction.modRM |= 0x05;
      tempInstruction.displacement = tempAddr.displacement;

      *instruction = tempInstruction;
      return true;
    }

    return false;
  }

  bool x86GenOpRegMem( FILE* binFile, unsigned mnemonic, x86Addr* addr ) {
    return false;
  }

  bool x86GenOpRegImm( FILE* binFile, unsigned mnemonic, unsigned reg, unsigned imm ) {
    x86Instruction instruction = {};
    size_t mnemonicIndex;
    size_t formatIndex;
    size_t encodeIndex;
    x86FormatParam param1;
    x86FormatParam param2;
    x86FormatParam param3;
    unsigned opIndex;
    unsigned regBits;

    if( !(binFile && mnemonic && (mnemonic <= lastX86Ident)) ) {
      return false;
    }

    mnemonicIndex = mnemonic - firstX86Ident;
    formatIndex = formatStart[mnemonicIndex];
    if( formatIndex > formatCount ) {
      return false;
    }

    do {
      if( formatTable[formatIndex].mnemonic != mnemonic ) {
        return false;
      }

      param1 = formatTable[formatIndex].param[0];
      param2 = formatTable[formatIndex].param[1];
      param3 = formatTable[formatIndex].param[2];

      if( (param3.first | param3.last) == 0 ) {
        if( (param2.first <= valUint) && (valUint32 <= param2.last) ) {
          if( (param1.first <= reg) && (reg <= param1.last) ) {
            break;
          }
        }
      }

      formatIndex++;
    } while( formatIndex < formatCount );

    encodeIndex = formatTable[formatIndex].index;
    if( encodeIndex >= encodeCount ) {
      return false;
    }

    // Initialize instruction structure
    instruction.fields = encodeTable[encodeIndex].fields;

    switch( reg & x86OperandMask ) {
    case 0:
      break;

    case x86Reg8:
      instruction.fields |= hasImm8;
      break;

    case x86Reg16:
      instruction.fields |= hasImm16;
      break;

    case x86Reg32:
      instruction.fields |= hasImm32;
      break;

    default:
      return false;
    }

    instruction.prefix[0] = encodeTable[encodeIndex].prefix[0];
    instruction.prefix[1] = encodeTable[encodeIndex].prefix[1];
    instruction.prefix[2] = encodeTable[encodeIndex].prefix[2];
    instruction.prefix[3] = encodeTable[encodeIndex].prefix[3];

    instruction.opcode[0] = encodeTable[encodeIndex].opcode[0];
    instruction.opcode[1] = encodeTable[encodeIndex].opcode[1];
    instruction.opcode[2] = encodeTable[encodeIndex].opcode[2];

    instruction.modRM = encodeTable[encodeIndex].modRM;

    instruction.immediate = imm;

    // Mnemonic/Opcode transforms
    opIndex = 0;
    switch( encodeTable[encodeIndex].xformOp ) {
    case 0:
      break;

    case xformOpW:
      regBits = 0;
      switch( reg & x86OperandMask ) {
      case x86Reg16:
        regBits = 16;
        break;
      case x86Reg32:
        regBits = 32;
        break;
      default:
        return false;
      }
      if( target.bits != regBits ) {
        instruction.fields |= hasOperandSize;
        instruction.prefix[indexOperandSize] = target.operandWPrefix;
      }
      break;

    default:
      return false;
    }

    // Register transforms
    switch( encodeTable[encodeIndex].xform[0] ) {
    case 0:
      break;

    case xformModReg:
      instruction.modRM |= (reg - (reg & x86OperandMask));
      break;

    default:
      return false;
    }

    // Immediate transforms
    opIndex = 0;
    switch( encodeTable[encodeIndex].xform[1] ) {
    case 0:
      break;

    case xformImm8:
      if( instruction.fields & hasOpcode3 ) {
        opIndex++;
      }
      if( instruction.fields & hasOpcode2 ) {
        opIndex++;
      }
      if( (imm & 0xFFFFFF00) == 0xFFFFFF00 ) {
        instruction.fields &= (~hasImm32);
        instruction.fields |= hasImm8;
        instruction.opcode[opIndex] |= (1 << 1);
      }
      break;

    default:
      return false;
    }

    // Unused parameter validation
    if( encodeTable[encodeIndex].xform[2] != 0 ) {
      return false;
    }

    return x86Emit(binFile, &instruction);
  }

/*
 *  Backpatch implementation
 */

/*
 *  Expression parser implementation
 */

/*
 *  Parser implementation
 */
