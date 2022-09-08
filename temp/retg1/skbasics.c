
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack* stack = NULL;

void DumpStack( Stack* sourceStack, unsigned marker ) {
  unsigned slotIndex = 0;

  printf( "Dumping stack %u - stack(%p) slots(%p) top(%u) bottom(%u)\n",
      marker, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );

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

  DumpStack( stack, 0 );

  printf( "1:result = CompactStack(stack);\n" );
  result = CompactStack(stack);
  if( result ) {
    printf( "Error in CompactStack[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(1);
  }

  DumpStack( stack, 1 );

  printf( "2:result = Push(stack, 1234);\n" );
  result = Push(stack, 1234);
  if( result ) {
    printf( "Error in Push[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(2);
  }

  DumpStack( stack, 2 );

  printf( "3:result = CompactStack(stack);\n" );
  result = CompactStack(stack);
  if( result ) {
    printf( "Error in CompactStack[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(3);
  }

  DumpStack( stack, 3 );

  printf( "4:result = Push(stack, 2345);\n" );
  result = Push(stack, 2345);
  if( result ) {
    printf( "Error in Push[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(4);
  }

  DumpStack( stack, 4 );

  printf( "5:result = Peek(stack, &value);\n" );
  result = Peek(stack, &value);
  if( result ) {
    printf( "Error in Peek[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(5);
  }

  printf( "6:if( value != 2345 )\n" );
  if( value != 2345 ) {
    printf( "Expected 2345 from Peek: %u\n", value );
    exit(6);
  }

  printf( "7:result = PeekAhead(stack, 2, &value);\n" );
  result = PeekAhead(stack, 2, &value);
  if( result == 0 ) {
    printf( "Expected PeekAhead[%u] to fail: byAmount(2) stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(7);
  }

  printf( "8:result = PeekAhead(stack, 1, &value);\n" );
  result = PeekAhead(stack, 1, &value);
  if( result ) {
    printf( "Error in PeekAhead[%u]: byAmount(1) stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(8);
  }

  printf( "9:if( value != 1234 )\n" );
  if( value != 1234 ) {
    printf( "Expected 1234 from PeekAhead: %u\n", value );
    exit(9);
  }

  printf( "10:result = PeekAhead(stack, 0, &value);\n" );
  result = PeekAhead(stack, 0, &value);
  if( result ) {
    printf( "Error in PeekAhead[%u]: byAmount(0) stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(10);
  }

  printf( "11:if( value != 2345 )\n" );
  if( value != 2345 ) {
    printf( "Expected 2345 from PeekAhead: %u\n", value );
    exit(11);
  }

  printf( "12:result = Pop(stack, &value);\n" );
  result = Pop(stack, &value);
  if( result ) {
    printf( "Error in Pop[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(12);
  }

  printf( "13:if( value != 2345 )\n" );
  if( value != 2345 ) {
    printf( "Expected 2345 from Pop: %u\n", value );
    exit(13);
  }

  DumpStack( stack, 5 );

  printf( "14:result = Pop(stack, &value);\n" );
  result = Pop(stack, &value);
  if( result ) {
    printf( "Error in Pop[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(14);
  }

  DumpStack( stack, 6 );

  printf( "15:result = Peek(stack, &value);\n" );
  result = Peek(stack, &value);
  if( result == 0 ) {
    printf( "Expected Peek[%u] to fail: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(15);
  }

  printf( "16:result = PeekAhead(stack, 0, &value);\n" );
  result = PeekAhead(stack, 0, &value);
  if( result == 0 ) {
    printf( "Expected PeekAhead[%u] to fail: byAmount(0) stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(16);
  }

  printf( "17:result = Pop(stack, &value);\n" );
  result = Pop(stack, &value);
  if( result == 0 ) {
    printf( "Expected Pop[%u] to fail: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(17);
  }

  printf( "18:if( value != 1234 )\n" );
  if( value != 1234 ) {
    printf( "Expected 1234 from Pop: %u\n", value );
    exit(18);
  }

  DumpStack( stack, 7 );

/*
  printf( "19:result = CompactStack(stack);\n" );
  result = CompactStack(stack);
  if( result ) {
    printf( "Error in CompactStack[%u]: stack(%p) slots(%p) top(%u) bottom(%u)\n",
      result, stack, stack ? stack->slot : NULL, stack ? stack->top : 0,
      stack ? stack->bottom : 0 );
    exit(19);
  }

  DumpStack( stack, 8 );
*/

  Cleanup();

  return 0;
}

#include "stack.c"
