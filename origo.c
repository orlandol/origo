
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
  Declaraytions
*/

/// String declarations

char* CreateString( const char* value );
void FreeString( char** stringPtr );

/// Symbol Table declarations

/// Parser deckarations

typedef struct Parser {
  FILE* handle;

  int ch;

  unsigned line;
  unsigned column;

  unsigned nextLine;
  unsigned nextColumn;

  unsigned entryPoint;
} Parser;

Parser* OpenGrammar( const char* fileName );
void CloseGrammar( Parser** sourcePtr );

int ReadChar( Parser* source );

/// Compiler declarations

typedef struct Compiler {
  FILE* handle;
  char* outCode;
} Compiler;

Compiler* CreateCompiler( const char* fileName );
void CloseCompiler( Compiler** compilerPtr );

/*
  Main program
*/

Parser* parser = NULL;
Compiler* compiler = NULL;

void ShowBanner() {
  printf( "Origo compiler generator v%u.%u %s\nCopyright 2022 Orlando Llanes\n", 0, 1, "Alpha" );
}

void ShowUsage() {
  printf( "\nUsage: origo infile.og outfile.c\n" );
}

void Cleanup() {
  CloseGrammar( &parser );
  CloseCompiler( &compiler );
}

int main( int argc, char** argv ) {
  atexit( Cleanup );

  ShowBanner();

  if( argc < 3 ) {
    ShowUsage();
    exit(1);
  }

  parser = OpenGrammar(argv[1]);
  if( parser == NULL ) {
    printf( "Error opening source grammar '%s'\n", argv[1] );
    exit(1);
  }

  compiler = CreateCompiler(argv[2]);
  if( compiler == NULL ) {
    printf( "Error creating compiler '%s'\n", argv[2] );
    exit(1);
  }

  Cleanup();

  return 0;
}

/*
  Implementations
*/

/// String implementation

char* CreateString( const char* value ) {
  char* newString = NULL;
  size_t valueLen = 0;

  if( value ) { 
    valueLen = strlen(value);
  }

  newString = calloc(1, (valueLen + 1) * sizeof(char) );

  if( newString ) {
    strcat( newString, value );
  }

  return newString;
}

void FreeString( char** stringPtr ) {
  if( stringPtr ) {
    if( (*stringPtr) ) {
      free( (*stringPtr) );
      (*stringPtr) = NULL;
    }
  }
}

/// Symbol Table implementation

/// Parser implementation

Parser* OpenGrammar( const char* fileName ) {
  Parser* newParser = NULL;

  if( !(fileName || (*fileName)) ) { return NULL; }

  newParser = calloc(1, sizeof(Parser));
  if( newParser == NULL ) { return NULL; }

  newParser->handle = fopen(fileName, "r");
  if( newParser->handle ) {
    newParser->line = 1;
    newParser->column = 1;
    newParser->nextLine = 1;
    newParser->nextColumn = 1;
 
    ReadChar( newParser );

    return newParser;
  }

  CloseGrammar( &newParser );
  return NULL;
}

void CloseGrammar( Parser** sourcePtr ) {
  if( sourcePtr ) {
    if( (*sourcePtr) ) {
      if( (*sourcePtr)->handle ) {
        fclose( (*sourcePtr)->handle );
        (*sourcePtr)->handle = NULL;
      }

      free( (*sourcePtr) );
      (*sourcePtr) = NULL;
    }
  }
}

int ReadChar( Parser* source ) {
  if( !(source && source->handle) ) { return 0; }

  source->ch = fgetc(source->handle);
  if( source->ch != EOF ) {
    if( source->ch == '\r' ) {
      source->ch = fgetc(source->handle);
    }

    source->nextColumn++;
    if( source->ch == '\n' ) {
      source->nextLine++;
      source->nextColumn = 1;
    }

    return source->ch;
  }

  return EOF;
}

/// Compiler implementation

Compiler* CreateCompiler( const char* fileName ) {
  Compiler* newCompiler = NULL;

  newCompiler = calloc(1, sizeof(Compiler));
  if( newCompiler == NULL ) { return NULL; }

  newCompiler->handle = fopen(fileName, "w");
  if( newCompiler->handle ) {
    fprintf( newCompiler->handle,
      "// Generated by Origo\n\n"
      "#include <stdio.h>\n\n"
      "/// DECLARATIONS\n\n"
    );

    newCompiler->outCode = CreateString(
      "/// IMPLEMENTATION\n\n"
      "int main( int argc, char** argv ) {\n"
      "  return 0;\n"
      "}\n"
    );
    return newCompiler;
  }

  CloseCompiler( &newCompiler );
}

void CloseCompiler( Compiler** compilerPtr ) {
  if( compilerPtr ) {
    if( (*compilerPtr) ) {
      // Write the generated code following previously written declarations
      if( (*compilerPtr)->handle && (*compilerPtr)->outCode ) {
        fprintf( (*compilerPtr)->handle, "%s", (*compilerPtr)->outCode );

        // Release the compiler implementation string
        free( (*compilerPtr)->outCode );
        (*compilerPtr)->outCode = NULL;
      }

      // Close the file
      if( (*compilerPtr)->handle ) {
        fclose( (*compilerPtr)->handle );
        (*compilerPtr)->handle = NULL;
      }

      free( *compilerPtr );
      (*compilerPtr) = NULL;
    }
  }
}
