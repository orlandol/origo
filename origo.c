
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
      rsvdType,
      rsvdNewType,
      rsvdStruct,
      rsvdObject,
      rsvdImport,
      rsvdPublic,
      rsvdMutable,
      rsvdExtends,
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

    // Base type tokens
    baseType = (2 << 9),
      baseInt,
        firstBaseType = baseInt,
      baseUint,
      baseChar,
      baseBool,
        lastBaseType = baseBool,

    // Literal value tokens
    valImmediate = (3 << 9),
      valInt = (valImmediate + (0 << 5)),
          firstValInt = valInt,
        valInt8,
        valInt16,
        valInt32,
          lastValInt = valInt32,

      valUint = (valImmediate + (1 << 5)),
          firstValUint = valUint,
        valUint8,
        valUint16,
        valUint32,
          lastValUint = valUint32,

      valChar = (valImmediate + (2 << 5)),
          firstValChar = valChar,
          lastValChar = valChar,

      valBool = (valImmediate + (3 << 5)),
          firstValBool = valBool,
          lastValBool = valBool,

    // Operator tokens
    operSymbol = (4 << 9),
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
    assignSymbol  = (5 << 9),
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
    x86Operand = (6 << 9),
        x86OperandMask = (x86Operand | (15 << 4)),
        x86SubOperandMask = (~3),

      // x86 memory address
      x86Mem  = (x86Operand + (0 << 4) + 0),
          firstX86Mem = x86Mem,
        x86Mem8,
        x86Mem16,
        x86Mem32,
          lastX86Mem = x86Mem,

      // x86 segment/selector register tokens
      x86SReg = (x86Operand + (1 << 4) + 0),
        x86SRegES = x86SReg,
          firstX86SReg = x86SRegES,
        x86SRegCS,
        x86SRegSS,
        x86SRegDS,
        x86SRegFS,
        x86SRegGS,
          lastX86SReg = x86SRegGS,

      // x86 8-bit register tokens
      x86Reg8 = (x86Operand + (2 << 4) + 0),
        x86RegAL = x86Reg8,
          firstX86Reg8 = x86RegAL,
        x86RegCL,
        x86RegDL,
        x86RegBL,
        x86RegAH,
        x86RegCH,
        x86RegDH,
        x86RegBH,
          lastX86Reg8 = x86RegBH,

      // x86 16-bit register tokens
      x86Reg16 =(x86Operand + (3 << 4) + 0),
        x86RegAX = x86Reg16,
          firstX86Reg16 = x86RegAX,
        x86RegCX,
        x86RegDX,
        x86RegBX,
        x86RegSP,
        x86RegBP,
        x86RegSI,
        x86RegDI,
          lastX86Reg16 = x86RegDI,

      // x86 32-bit register tokens
      x86Reg32 = (x86Operand + (4 << 4) + 0),
        x86RegEAX = x86Reg32,
          firstX86Reg32 = x86RegEAX,
        x86RegECX,
        x86RegEDX,
        x86RegEBX,
        x86RegESP,
        x86RegEBP,
        x86RegESI,
        x86RegEDI,
          lastX86Reg32 = x86RegEDI,

    // x86 mnemonic tokens
    x86Mnemonic = (7 << 9),
      x86Call,
        firstX86Mnemonic = x86Call,
      x86Push,
      x86Popf,
      x86Pushf,
      x86Ret,
      x86Retf,
      x86Adc,
      x86Add,
      x86And,
      x86Cmp,
      x86Dec,
      x86Div,
      x86IDiv,
      x86Imul,
      x86In,
      x86Inc,
      x86Int,
      x86Jo,
        firstX86Jcc = x86Jo,
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
          lastX86Jcc = x86Jnle,
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
      x86Shld,
      x86Shr,
      x86Shrd,
      x86Sbb,
      x86Sub,
      x86Test,
      x86Xchg,
      x86Xor,
      x86Cbw,
      x86Cdq,
      x86Cwd,
      x86Clc,
      x86Cld,
      x86Cli,
      x86Cmc,
      x86Int3,
      x86Iret,
      x86Lahf,
      x86Nop,
      x86Rep,
      x86Repe,
      x86Repne,
      x86Repz,
      x86Repnz,
      x86Cmpsb,
      x86Cmpsw,
      x86Cmpsd,
      x86Scasb,
      x86Scasw,
      x86Scasd,
      x86Lodsb,
      x86Lodsw,
      x86Lodsd,
      x86Movsb,
      x86Movsw,
      x86Movsd,
      x86Stosb,
      x86Stosw,
      x86Stosd,
      x86Sahf,
      x86Stc,
      x86Std,
      x86Sti,
      x86Xlatb,
        lastX86Mnemonic = x86Xlatb,

    // x86 general keywords
    x86Keyword = (8 << 9),
        firstx86Keyword = x86Keyword,
      x86Short,
      x86Near,
      x86Far,
        lastx86Keyword = x86Far,
  } Token;

/*
 *  Keyword table declarations
 */

  typedef struct Keyword {
    char*    name;
    unsigned token;
  } Keyword;

  const Keyword keywordTable[] = {
    "end",     rsvdEnd,
    "import",  rsvdImport,
    "newtype", rsvdNewType,
    "program", rsvdProgram,
    "run",     rsvdRun,
    "type",    rsvdType
  };

  const size_t keywordCount = sizeof(keywordTable) / sizeof(keywordTable[0]);

  unsigned FindKeyword( char* identifier );

/*
 *  System helper declarations
 */

  bool RunProgram( const char* programName, char* const programArgs[], int* exitCode );

/*
 *  Error reporting declarations
 */

  // Logic warnings
  enum WarningCodes {
    noWarnings = 0
  };

  void Warning( unsigned line, unsigned column, unsigned warningCode );

  // Syntax errors
  enum ExpectedCodes {
    nothingExpected = 0,
    expectedProgram,
    expectedIdentifier,
    expectedTopLevel
  };

  void Expected( unsigned line, unsigned column, unsigned expectedCode );

  // Program errors
  enum ErrorCodes {
    noError = 0,
    setExitHandlerFailed,
    unableToOpen,
    unableToCreate,
    unableToRun,
    unableToBuild,
    unableToLink
  };

  void Error( unsigned errorCode, char* errorText );

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

  void rstrclear( rstring* source );

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
 *  Type specifier declarations
 */

  typedef struct ArrayDimension {
    int minRange;
    int maxRange;
    size_t count;
    size_t offset;
  } ArrayDimension;

  typedef struct TypeSpec {
    // <ptrRef '#' | ptrData '@'>
    unsigned pointerType;

    // <typeStruct, typeUint, etc>
    unsigned baseType;

    // '[' <[min..max[,...]] | [count[,...]]> ']'
    size_t dimensionCount;
    ArrayDimension* dimension;
  } TypeSpec;

/*
 *  Symbol table declarations
 */

  typedef struct BaseTypeSymbol {
    unsigned bitCount;
  } BaseTypeSymbol;

  typedef struct TypeSymbol {
    TypeSpec typeSpec;
    uint8_t* defaultVal;
    unsigned valType;
    size_t valSize;
  } TypeSymbol;

  enum SymType {
    symNone = 0,
    symBaseType,
    symType
  };

  typedef struct SymbolItem {
    unsigned token;

    union {
      BaseTypeSymbol baseTypeSym;
      TypeSymbol typeSym;
    };
  } SymbolItem;

  DECLARE_STRING_KEYARRAY_TYPES( SymbolTable, SymbolItem )

  void FreeSymItem( SymbolItem* symItem );

  DECLARE_STRING_KEYARRAY_CREATE( CreateSymTab, SymbolTable )
  DECLARE_STRING_KEYARRAY_FREE( ReleaseSymTab, SymbolTable, FreeSymItem )

  DECLARE_STRING_KEYARRAY_INSERT( InsertSymbol, SymbolTable, SymbolItem )
  DECLARE_STRING_KEYARRAY_REMOVE( RemoveSymbol, SymbolTable, FreeSymItem )

  DECLARE_STRING_KEYARRAY_RETRIEVE( RetrieveSymbol, SymbolTable, SymbolItem )

  bool DeclareBaseType( SymbolTable* symTab, char* name, unsigned baseToken );
  bool DeclareType( SymbolTable* symTab, char* name, TypeSpec* typeSpec );

/*
 *  Lexer declarations
 */

  typedef struct RetFile {
    FILE* handle;
    rstring* fileName;
    unsigned line;
    unsigned column;
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

  unsigned ReadTypeSpec( RetFile* source, TypeSpec* destSpec );

/*
 *  Expression parser declarations
 */

/*
 *  Assembler generation declarations
 */

  typedef struct AsmGen {
    FILE* asmHandle;
    FILE* importHandle;
    FILE* constHandle;
    FILE* dataHandle;
    FILE* bssHandle;
    rstring* fileName;
    bool runDeclared;
  } AsmGen;

  AsmGen* CreateAsm( char* fileName, size_t nameLength );
  void CloseAsm( AsmGen** source );

  bool AsmGenOp( AsmGen* asmGen, unsigned mnemonic );

/*
 *  Parser declarations
 */

  typedef struct _IfStack {
    unsigned block;
    unsigned currentIf;
    unsigned nextIf;
    unsigned endIf;
  } IfStack;

  typedef struct _LoopStack {
  } LoopStack;

  void ParseProgramHeader();
  void EndParse();

  void ParseType();
  void ParseNewType();
  void ParseImport();

  void ParseIf( SymbolTable* localSymTab, IfStack* ifStack );
  void ParseFor( SymbolTable* localSymTab, LoopStack* loopStack );

  void ParseStatement( SymbolTable* localSymTab );

  void ParseRun();

  void ParseTopLevel();

/*
 *  Main program
 */

  void PrintBanner() {
    printf( "\nOrigo Alpha\n" );
    printf( "Copyright 2019 Orlando Llanes\n" );
  }

  void PrintUsage() {
    printf( "\nusage: origo source[.ret] [binary[.ext]]\n" );
  }

  rstring* retFileName = NULL;
  rstring* asmFileName = NULL;
  rstring* objFileName = NULL;
  rstring* exeFileName = NULL;

  RetFile* retSource = NULL;
  AsmGen* asmGen = NULL;

  void Cleanup() {
    if( retFileName ) {
      free( retFileName );
      retFileName = NULL;
    }

    if( asmFileName ) {
      free( asmFileName );
      asmFileName = NULL;
    }

    if( objFileName ) {
      free( objFileName );
      objFileName = NULL;
    }

    if( exeFileName ) {
      free( exeFileName );
      exeFileName = NULL;
    }

    CloseRet( &retSource );
    CloseRet( &asmGen );
  }

int main( int argc, char* argv[] ) {
  argCount = argc;
  argVar = argv;

  if( atexit(Cleanup) ) {
    Error( setExitHandlerFailed, "" );
  }

  rstring* tmpFileName = NULL;
  size_t scanIndex = 0;

  PrintBanner();

  if( (argc < 2) || (argc > 3) ) {
    PrintUsage();
    return 1;
  }

  /* Open and create files */
  retFileName = rstrcopyc(argv[1], strlen(argv[1]));
  retSource = OpenRet(rstrtext(retFileName), rstrlen(retFileName));
  if( retSource == NULL ) {
    scanIndex = rrevscanc(argv[1], '.');
    if( scanIndex == -1 ) {
      tmpFileName = rstrappendc(retFileName, ".ret", 0);
      if( tmpFileName ) {
        retFileName = tmpFileName;
      }
    }
    retSource = OpenRet(rstrtext(retFileName), rstrlen(retFileName));
  }
  if( retSource == NULL ) {
    Error( unableToOpen, rstrtext(retFileName) );
  }

  /* Create assembler file and includes */
  scanIndex = rrevscan(retFileName, '.');
  if( scanIndex != -1 ) {
    asmFileName = rsubstr(retFileName, 0, scanIndex - 1);
    objFileName = rsubstr(retFileName, 0, scanIndex - 1);
  } else {
    asmFileName = rstrcopy(retFileName);
    objFileName = rstrcopy(retFileName);
  }
  tmpFileName = rstrappendc(asmFileName, ".rxa", 0);
  if( tmpFileName ) {
    asmFileName = tmpFileName;
  }
  tmpFileName = rstrappendc(objFileName, ".obj", 0);
  if( tmpFileName ) {
    objFileName = tmpFileName;
  }

  asmGen = CreateAsm(rstrtext(asmFileName), rstrlen(asmFileName));
  if( asmGen == NULL ) {
    Error( unableToCreate, rstrtext(asmFileName) );
  }

  /* Determine binary file name */
  if( argc >= 3 ) {
    exeFileName = rstrcopyc(argv[2], 0);
  } else {
    scanIndex = rrevscan(retFileName, '.');
    if( scanIndex != -1 ) {
      exeFileName = rsubstr(retFileName, 0, scanIndex - 1);
    }
  }
  tmpFileName = rstrappendc(exeFileName, ".exe", 0);
  if( tmpFileName ) {
    exeFileName = tmpFileName;
  }

  /* Parse Retineo source */
  printf( "\nParsing '%s'...\n", rstrtext(retFileName) );

  printf( "\n" );

  ParseProgramHeader();
  ParseTopLevel();
  EndParse();

  CloseRet( &retSource );
  CloseAsm( &asmGen );

  /* Build assembler source */
  printf( "\nBuilding '%s'...\n", rstrtext(asmFileName) );

  char* nasmOptions[] = {
    " -fobj",
    rstrtext(asmFileName),
    NULL
  };
  int nasmResult = 0;
  if( (RunProgram("nasm.exe", nasmOptions, &nasmResult) == false) ||
      nasmResult ) {
    Error( unableToRun, "nasm.exe" );
  }

  /* Link executable */
  printf( "\nLinking '%s'...\n", rstrtext(exeFileName) );

  char* alinkOptions[] = {
    " -c",
    "-oPE",
    "-subsys console",
    rstrtext(objFileName),
    NULL
  };
  int alinkResult = 0;
  if( (RunProgram("alink.exe", alinkOptions, &alinkResult) == false) ||
      alinkResult ) {
    Error( unableToRun, "alink.exe" );
  }

  /* Release resources */
  Cleanup();

  return 0;
}

/*
 *  Token implementation
 */

  unsigned FindKeyword( char* identifier ) {
    size_t leftIndex = 0;
    size_t rightIndex = keywordCount;
    size_t keywordIndex = keywordCount / 2;
    int    compareCode = 0;

    if( !(identifier && (*identifier)) ) {
      return 0;
    }

    while( leftIndex < rightIndex ) {
      compareCode = strcmp(keywordTable[keywordIndex].name, identifier);
      if( compareCode == 0 ) {
        return keywordTable[keywordIndex].token;
      }

      if( compareCode > 0 ) {
        rightIndex = keywordIndex;
      } else {
        leftIndex = keywordIndex + 1;
      }

      keywordIndex = (leftIndex + rightIndex) / 2;
    }

    return 0;
  }

/*
 *  System helper implementation
 */

  #include <process.h>

  bool RunProgram( const char* programName, char* const programArgs[], int* exitCode ) {
    int runResult = 0;

    runResult = spawnvp(_P_WAIT, programName, programArgs);

    if( runResult != -1 ) {
      *exitCode = runResult;
    }

    return (runResult != -1);
  }

/*
 *  Error reporting implementation
 */

  // Logic warning reporting
  const char* warningString[] = {
    "No warning"
  };

  const size_t warningCount = sizeof(warningString) / sizeof(warningString[0]);

  void Warning( unsigned line, unsigned column, unsigned warningCode ) {
    if( warningCode && (warningCode < warningCount) ) {
      printf( "  Warning(L%u, C%u): %s\n", line, column, warningString[warningCode] );
    } else if( warningCode ) {
      printf( "  Warning: code %u\n", warningCode );
    }
  }

  // Syntax error reporting
  const char* expectedString[] = {
    "Nothing",
    "program",
    "undeclared identifier",
    "type, newtype, or import"
  };

  const size_t expectedCount = sizeof(expectedString) / sizeof(expectedString[0]);

  void Expected( unsigned line, unsigned column, unsigned expectedCode ) {
    if( expectedCode && (expectedCode < expectedCount) ) {
      printf( "  Expected(L%u, C%u): %s\n", line, column, expectedString[expectedCode] );
    } else if( expectedCode ) {
      printf( "  Expected: code %u\n", expectedCode );
    }

    exit( expectedCode );
  }

  // Program error reporting
  const char* errorString[] = {
    "No error",
    "Set exit handler failed",
    "Unable to open",
    "Unable to create",
    "Unable to run",
    "Unable to build",
    "Unable to link"
  };

  const size_t errorCount = sizeof(errorString) / sizeof(errorString[0]);

  void Error( unsigned errorCode, char* errorText ) {
    if( errorCode && (errorCode < errorCount) && errorText ) {
      printf( "  ERROR(%u) %s: %s\n", errorCode, errorString[errorCode], errorText );
    } else if( errorCode ) {
      printf( "  ERROR: code %u\n", errorCode );
    }

    exit( errorCode );
  }

/*
 *  String implementation
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

  void rstrclear( rstring* dest ) {
    char* destText = NULL;
    if( dest ) {
      destText = rstrtext(dest);

      dest->length = 0;
      memset( destText, 0, rstrrsvdlen(dest) );
    }
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
    if( sourceLength == 0 ) {
      return (-1);
    }
    matchIndex = sourceLength - 1;

    sourceCh = rstrtext(source) + matchIndex;

    if( source == NULL ) {
      return (-1);
    }

    while( sourceLength && (*sourceCh) ) {
      if( (*sourceCh) == matchCh ) {
        return matchIndex;
      }

      sourceCh--;
      sourceLength--;
      matchIndex--;
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
 *  Type specifier implementation
 */

/*
 *  Symbol table implementation
 */

  void FreeSymItem( SymbolItem* symItem ) {
  }

  bool DeclareBaseType( SymbolTable* symTab, char* name, unsigned baseToken ) {
    return false;
  }

  bool DeclareType( SymbolTable* symTab, char* name, TypeSpec* typeSpec ) {
    return false;
  }

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
    newSource->column = 1;

    ReadChar( newSource );
    ReadChar( newSource );

    newSource->line = 1;
    newSource->column = 1;

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
    unsigned columnInc = 1;
    char tmpCh = 0;

    if( !(source && source->handle) ) {
      return false;
    }

    if( source->curCh == 10 ) {
      source->line++;
      source->column = 1;
      columnInc = 0;
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

    source->column += columnInc;

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
  const size_t operCount = sizeof(operTable) / sizeof(operTable[0]);

  unsigned ReadOperator( RetFile* source ) {
    char operator[8] = {};
    size_t leftIndex;
    size_t operIndex;
    size_t rightIndex;
    size_t operLength = 0;
    int    compareCode;
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
      rightIndex = operCount;
      operIndex = operCount / 2;

      while( leftIndex < rightIndex ) {
        compareCode = strcmp(operTable[operIndex].text, operator);
        if( compareCode == 0 ) {
          token = operTable[operIndex].token;
          break;
        }

        if( compareCode > 0 ) {
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

  unsigned ReadTypeSpec( RetFile* source, TypeSpec* destSpec ) {
    return 0;
  }

/*
 *  Expression parser implementation
 */

/*
 *  Assembler generation declarations
 */

  AsmGen* CreateAsm( char* fileName, size_t nameLength ) {
    AsmGen* newGen = NULL;

    if( !(fileName && (*fileName)) ) {
      goto ReturnError;
    }

    newGen = calloc(1, sizeof(AsmGen));
    if( newGen == NULL ) {
      goto ReturnError;
    }

    newGen->fileName = rstrcopyc(fileName, nameLength);
    if( newGen->fileName == NULL ) {
      goto ReturnError;
    }

    newGen->asmHandle = fopen(fileName, "wb");
    if( newGen->asmHandle == NULL ) {
      goto ReturnError;
    }

    newGen->importHandle = fopen("import.rxi", "wb");
    if( newGen->importHandle == NULL ) {
      goto ReturnError;
    }

    newGen->constHandle = fopen("const.rxi", "wb");
    if( newGen->constHandle == NULL ) {
      goto ReturnError;
    }

    newGen->dataHandle = fopen("data.rxi", "wb");
    if( newGen->dataHandle == NULL ) {
      goto ReturnError;
    }

    newGen->bssHandle = fopen("bss.rxi", "wb");
    if( newGen->bssHandle == NULL ) {
      goto ReturnError;
    }

    return newGen;

  ReturnError:
    CloseAsm( &newGen );

    return NULL;
  }

  void CloseAsm( AsmGen** asmGen ) {
    if( asmGen ) {
      if( (*asmGen) ) {
        if( (*asmGen)->asmHandle ) {
          fclose( (*asmGen)->asmHandle );
          (*asmGen)->asmHandle = NULL;
        }

        if( (*asmGen)->importHandle ) {
          fclose( (*asmGen)->importHandle );
          (*asmGen)->importHandle = NULL;
        }

        if( (*asmGen)->constHandle ) {
          fclose( (*asmGen)->constHandle );
          (*asmGen)->constHandle = NULL;
        }

        if( (*asmGen)->dataHandle ) {
          fclose( (*asmGen)->dataHandle );
          (*asmGen)->dataHandle = NULL;
        }

        if( (*asmGen)->bssHandle ) {
          fclose( (*asmGen)->bssHandle );
          (*asmGen)->bssHandle = NULL;
        }

        if( (*asmGen)->fileName ) {
          free( (*asmGen)->fileName );
          (*asmGen)->fileName = NULL;
        }

        free( (*asmGen) );
        (*asmGen) = NULL;
      }
    }
  }

  bool AsmGenOp( AsmGen* asmGen, unsigned mnemonic ) {
    return false;
  }

/*
 *  Parser implementation
 */

  void ParseProgramHeader() {
    rstring* identStr = NULL;
    unsigned hashCode = 0;
    unsigned line;
    unsigned column;

    if( !(retSource && asmGen) ) {
      return;
    }

    // Validate program header
    SkipComments( retSource );

    line = retSource->line;
    column = retSource->column;

    if( !ReadIdent(retSource, &identStr, &hashCode) ) {
      // Ignore read error
    }
    if( FindKeyword(rstrtext(identStr)) != rsvdProgram ) {
      Expected( line, column, expectedProgram );
    }
    rstrclear( identStr );

    // Validate namespace identifier
    SkipComments( retSource );

    line = retSource->line;
    column = retSource->column;

    if( !ReadIdent(retSource, &identStr, &hashCode) ) {
      // Ignore read error
    }
    if( FindKeyword(rstrtext(identStr)) != 0 ) {
      Expected( line, column, expectedIdentifier );
    }

    ///TODO: Allocate symbol table

    // Write start of assembler file
    fprintf( asmGen->asmHandle,
      "\n"
      "; program %s\n"
      "\n"
      "  CPU 386\n"
      "  BITS 32\n"
      "\n"
      "  %%include \"import.rxi\"\n"
      "\n"
      "section .text use32\n"
      "\n",
      rstrtext(identStr)
    );

    if( identStr ) {
      free( identStr );
      identStr = NULL;
    }
  }

  void EndParse() {
    fprintf( asmGen->asmHandle,
      "\n"
      "section .rdata use32\n"
      "\n"
      "  %%include \"const.rxi\"\n"
      "\n"
      "section .data use32\n"
      "\n"
      "  %%include \"data.rxi\"\n"
      "\n"
      "section .bss use32\n"
      "\n"
      "  %%include \"bss.rxi\"\n"
    );
  }

  void ParseType() {
  }

  void ParseNewType() {
  }

  void ParseImport() {
  }

  void ParseIf( SymbolTable* localSymTab, IfStack* ifStack ) {
    ///TODO: IfStack usage [draft]

    //  if COND
    //    currentIf = nextIf++
    //  then STATEMENT
    //  [".nextif", hex(block), hex(nextIf), ":"]
    //  [".endif", hex(block), hex(endIf), ":"]

    //  if COND
    //    currentIf = nextIf++
    //    ...
    //    jmp endIf
    //
    //  [".nextif", hex(block), hex(currentIf), ":"]
    //  elseif COND
    //    currentIf = nextIf++
    //    ...
    //    jmp endIf
    //
    //  [".nextif", hex(block), hex(currentIf), ":"]
    //  else
    //    ...
    //  [".endif", hex(block), hex(endIf), ":"]
    //  endif
  }

  void ParseFor( SymbolTable* localSymTab, LoopStack* loopStack ) {
  }

  void ParseStatement( SymbolTable* localSymTab ) {
  }

  void ParseRun() {
  }

  void ParseTopLevel() {
    rstring* identStr = NULL;
    unsigned token = 0;
    unsigned hashCode = 0;
    unsigned line = 0;
    unsigned column = 0;

    do {
      SkipComments( retSource );

      line = retSource->line;
      column = retSource->column;

      if( ReadIdent(retSource, &identStr, &hashCode) == false ) {
        ///TODO: Error.
      }

      token = FindKeyword(rstrtext(identStr));

      switch( token ) {
      case rsvdType:
        ParseType();
        break;

      case rsvdNewType:
        ParseNewType();
        break;

      case rsvdImport:
        ParseImport();
        break;

      case rsvdRun:
        ParseRun();
        break;

      default:
        Expected( line, column, expectedTopLevel );
      }
    } while( token );

    // Begin: Temporary code to be replaced by parser
    fprintf( asmGen->asmHandle,
      "run:\n"
      "..start:\n"
      "\n"
      "  push    ebp\n"
      "  mov     ebp, esp\n"
      "\n"
      "  mov     esp, ebp\n"
      "  pop     ebp\n"
      "\n"
      "  push   dword 123\n"
      "  call   [ExitProcess]\n"
    );
  
    fprintf( asmGen->importHandle,
      "\n"
      "  extern ExitProcess\n"
      "  import ExitProcess kernel32.dll\n"
    );
    // End: Temporary code to be replaced by parser
  }
