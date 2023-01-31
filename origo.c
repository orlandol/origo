
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "origo.h"

/*
 *  Main program
 */

char grammarFileName[_MAX_PATH] = {0};

Grammar* grammar = NULL;
SymbolTable* symbolTable = NULL;
Compiler* compiler = NULL;

unsigned ParseArgs( int argc, char** argv ) {
  ///TODO: Infer file names
  ///TODO: Generate template file with -new parameter
  return 3;
}

void Cleanup( void ) {
  CloseGrammar( &grammar );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  grammar = OpenGrammar("test.ori");
  if( grammar == NULL ) {
    printf( "Error opening file '%s'\n", "test.ori" );
    exit(1);
  }

  BuildCompiler( grammar, symbolTable, compiler );

  Cleanup();

  return 0;
}

/*
 *  Error reporting implementation
 */

void SyntaxError( unsigned line, unsigned column, const char* message ) {
  if( message ) {
    printf( "Grammar Error [L%u,C%u]: %s\n", line, column, message );
  }
  exit(1);
}

/*
 *  Symbol Table implementation
 */

/*
 *  Parser implementation
 */

Grammar* OpenGrammar( const char* fileName ) {
  Grammar* newGrammar = NULL;

  if( !(fileName && (*fileName)) ) { return NULL; }

  newGrammar = calloc(1, sizeof(Grammar));
  if( newGrammar == NULL ) { goto ReturnError; }

  newGrammar->handle = fopen(fileName, "rb");
  if( newGrammar->handle == NULL ) { goto ReturnError; }

  newGrammar->ch = EOF;
  newGrammar->nextch = EOF;

  ReadChar( newGrammar );

  newGrammar->line = 1;
  newGrammar->column = 1;
  newGrammar->nextline = 1;
  newGrammar->nextcolumn = 1;

  ReadChar( newGrammar );

  return newGrammar;

ReturnError:
  CloseGrammar( &newGrammar );

  return NULL;
}

void CloseGrammar( Grammar** grammarPtr ) {
  if( grammarPtr ) {
    if( (*grammarPtr) ) {
      if( (*grammarPtr)->handle ) {
        fclose( (*grammarPtr)->handle );
        (*grammarPtr)->handle = NULL;

        free( (*grammarPtr) );
        (*grammarPtr) = NULL;
      }
    }

    free( grammarPtr );
    grammarPtr = NULL;
  }
}

Grammar grammarState[1024];
size_t grammarIndex = 0;

int ReadChar( Grammar* source ) {
  if( source && source->handle ) {
    source->line = source->nextline;
    source->column = source->nextcolumn;

    source->ch = source->nextch;
    source->nextch = fgetc(source->handle);

    if( source->ch == '\r' ) {
      if( source->nextch == '\n' ) {
        source->nextch = fgetc(source->handle);
      }
      source->ch = '\n';
    }

    if( source->ch == '\n' ) {
      source->nextline++;
      source->nextcolumn = 0;
    }

    source->nextcolumn++;
  }

  return source->ch;
}

void SkipSpaceAndComments( Grammar* source ) {
  unsigned nestLevel = 0;
  unsigned line = 0;
  unsigned column = 0;

  if( source && source->handle ) {
    while( source->ch != EOF ) {
      if( isspace(source->ch) ) {
        do {
          ReadChar( source );
        } while( isspace(source->ch) );
        continue;
      }

      if( (source->ch == '/') && (source->nextch == '/') ) {
        ReadChar( source );

        do {
          ReadChar( source );
          if( source->ch == '\n' ) {
            break;
          }
        } while( source->ch != EOF );
        continue;
      }

      if( (source->ch == '/') && (source->nextch == '*') ) {
        ReadChar( source );
        ReadChar( source );
        nestLevel = 1;

        while( source->ch != EOF ) {
          if( (source->ch == '/') && (source->nextch == '*') ) {
            if( nestLevel == ((unsigned)-1) ) {
              SyntaxError( source->line, source->column,
                "Nesting limit reached" );
            }
            nestLevel++;

            ReadChar( source );
            ReadChar( source );
            continue;
          }

          if( (source->ch == '*') && (source->nextch == '/') ) {
            if( nestLevel == 0 ) {
              SyntaxError( source->line, source->column,
                "Attempt to close a comment that's not open" );
            }
            nestLevel--;

            ReadChar( source );
            ReadChar( source );
            continue;
          }

          ReadChar( source );
        }

        if( nestLevel ) {
          SyntaxError( source->line, source->column,
            "Unexpected EOF in comment" );
        }
        continue;
      }
      break;
    }
  }
}

/*
 *  Compiler implementation
 */

Compiler* CreateCompiler( const char* fileName ) {
  Compiler* newCompiler = NULL;

  if( !(fileName && (*fileName)) ) { return NULL; }

  ///TODO: Infer c/h/def filenames

  newCompiler = calloc(1, sizeof(Compiler));
  if( newCompiler == NULL ) { goto ReturnError; }

  ///TODO: Create c/h/def files

  return newCompiler;

ReturnError:
  CloseCompiler( &newCompiler );

  return NULL;
}

void CloseCompiler( Compiler** compilerPtr ) {
  if( compilerPtr ) {
    if( (*compilerPtr) ) {
      if( (*compilerPtr)->handle ) {
        fclose( (*compilerPtr)->handle );
        (*compilerPtr)->handle = NULL;
      }

      if( (*compilerPtr)->cHandle ) {
        fclose( (*compilerPtr)->cHandle );
        (*compilerPtr)->cHandle = NULL;
      }

      if( (*compilerPtr)->hHandle ) {
        fclose( (*compilerPtr)->hHandle );
        (*compilerPtr)->hHandle = NULL;
      }

      if( (*compilerPtr)->defHandle ) {
        fclose( (*compilerPtr)->defHandle );
        (*compilerPtr)->defHandle = NULL;
      }
    }
  }
}

unsigned BuildCompiler( Grammar* source,
  SymbolTable* symtab, Compiler* binary ) {

  SkipSpaceAndComments( source );

  ///TODO: Flag error if parse isn't declared on EOF

  return 4;
}
