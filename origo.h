#ifndef ORIGO_H
#define ORIGO_H

#include <stdio.h>
#include <stdlib.h>

/*
 *  System declarations
 */

#ifndef _MAX_PATH
// https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation
#define _MAX_PATH 260
#endif

#ifndef _MAX_DRIVE
// https://learn.microsoft.com/en-us/previous-versions/8e46eyt7(v=vs.140)
#define _MAX_DRIVE 3
#endif

#ifndef _MAX_DIR
// https://learn.microsoft.com/en-us/previous-versions/8e46eyt7(v=vs.140)
#define _MAX_DIR 256
#endif

#ifndef _MAX_FNAME
// https://learn.microsoft.com/en-us/previous-versions/8e46eyt7(v=vs.140)
#define _MAX_FNAME 256
#endif

#ifndef _MAX_EXT
// https://learn.microsoft.com/en-us/previous-versions/8e46eyt7(v=vs.140)
#define _MAX_EXT 256
#endif

/*
 *  Error reporting declarations
 */

void SyntaxError( unsigned line, unsigned column, const char* message );

/*
 *  Symbol Table declarations
 */

typedef struct SymbolTable {
  const char* name;
  unsigned symType;
} SymbolTable;

/*
 *  Parser declarations
 */

typedef struct Grammar {
  FILE* handle;
  unsigned line;
  unsigned column;
  unsigned nextline;
  unsigned nextcolumn;
  char ch;
  char nextch;
} Grammar;

Grammar* OpenGrammar( const char* fileName );
void CloseGrammar( Grammar** grammarPtr );

int ReadChar( Grammar* source );

void SkipSpaceAndComments( Grammar* source );

/*
 *  Compiler declarations
 */

typedef struct Compiler {
  FILE* handle;
  FILE* cHandle;
  FILE* hHandle;
  FILE* defHandle;
} Compiler;

Compiler* CreateCompiler( const char* fileName );
void CloseCompiler( Compiler** compilerPtr );

unsigned BuildCompiler( Grammar* source,
  SymbolTable* symtab, Compiler* binary );

#endif
