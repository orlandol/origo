
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack* stack = NULL;

void DumpStack( Stack* sourceStack ) {
  unsigned slotIndex = 0;

  printf( "Dumping stack...\n" );
  if( (sourceStack == NULL) ||
    (sourceStack->top == sourceStack->bottom) ) {

    printf( "  Stack empty.\n" );
    goto Done;
  }

  slotIndex = stack->top;
  do {
    slotIndex--;
    printf( "  Stack->slot[%u] == %u\n",
      slotIndex, stack->slot[slotIndex] );
  } while( slotIndex > stack->bottom );

Done:
  printf(
    "...Done.\n"
    "\n"
  );
}

void Cleanup() {
  ReleaseStack( &stack );
}

int main( int argc, char** argv ) {
  unsigned result = 0;
  StackSlot value = 0;

  stack = CreateStack();

  DumpStack( stack );

  result = Push(stack, 1234);
  if( result ) {
    printf( "Error in Push[%u]: stack(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(1);
  }

  DumpStack( stack );

  result = Push(stack, 2345);
  if( result ) {
    printf( "Error in Push[%u]: stack(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(2);
  }

  DumpStack( stack );

  result = Pop(stack, &value);
  if( result ) {
    printf( "Error in Pop[%u]: stack(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(3);
  }

  if( value != 2345 ) {
    printf( "Expected 2345 from Push: %u\n", value );
    exit(4);
  }

  result = Pop(stack, &value);
  if( result ) {
    printf( "Error in Pop[%u]: stack(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(5);
  }

  result = Pop(stack, &value);
  if( result == 0 ) {
    printf( "Expected Pop[%u] to fail: stack(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(6);
  }

  if( value != 1234 ) {
    printf( "Expected 1234 from Push: %u\n", value );
    exit(7);
  }

  DumpStack( stack );

  Cleanup();

  return 0;
}

#include "stack.c"
