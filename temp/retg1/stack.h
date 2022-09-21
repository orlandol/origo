#ifndef STACK_H
#define STACK_H

#define DECLARE_STACK_TYPES \
  typedef STACKSLOT_TYPE STACKSLOT_TYPENAME;\
  \
  typedef struct STACK_TYPENAME {\
    STACKSLOT_TYPENAME* slot;\
    unsigned top;\
    unsigned bottom;\
  } STACK_TYPENAME;\
  \
  typedef unsigned (*ReleaseSlotFunc)( STACKSLOT_TYPENAME* slotPtr );

#define DECLARE_CREATESTACK( FUNCNAME )\
STACK_TYPENAME* FUNCNAME();

#define DECLARE_RELEASESTACK( FUNCNAME )\
void FUNCNAME( STACK_TYPENAME** stackPtr );

#define DECLARE_RELEASESTACKSLOTS( FUNCNAME )\
void FUNCNAME( STACK_TYPENAME** stackPtr,\
  ReleaseSlotFunc releaseSlot );

#define DECLARE_GROWSTACK( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack );

#define DECLARE_COMPACTSTACK( FUNCNAME )\
unsigned CompactStack( STACK_TYPENAME* stack );

#define DECLARE_PUSHSLOT( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME fromItem );

#define DECLARE_POPSLOT( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem );

#define DECLARE_PEEKSLOT( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem );

#define DECLARE_PEEKSLOTAHEAD( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, unsigned byAmount,\
  STACKSLOT_TYPENAME* toItem );


#define DECLARE_STACK_DEFAULT_FUNCTIONS \
  DECLARE_CREATESTACK( CreateStack )\
  DECLARE_RELEASESTACK( ReleaseStack )\
  DECLARE_GROWSTACK( GrowStack )\
  DECLARE_COMPACTSTACK( CompactStack )\
  DECLARE_PUSHSLOT( Push )\
  DECLARE_POPSLOT( Pop )\
  DECLARE_PEEKSLOT( Peek )\
  DECLARE_PEEKAHEADSLOT( PeekAheadSlot )

#endif // STACK_H

#undef STACKSLOT_TYPE
#undef STACKSLOT_TYPENAME
#undef STACK_TYPENAME

#ifdef STACK_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>

#ifndef STACK_GROWTH_NUMBITS
  #define STACK_GROWTH_NUMBITS (4)
#endif

#define STACK_GROWTH_INCREMENT (1 << STACK_GROWTH_NUMBITS)
#define STACK_GROWTH_MASK (STACK_GROWTH_INCREMENT - 1)

#define IMPLEMENT_CREATESTACK( FUNCNAME )\
STACK_TYPENAME* FUNCNAME() {\
  return calloc(1, sizeof(STACK_TYPENAME));\
}

#define IMPLEMENT_RELEASESTACK( FUNCNAME )\
void ReleaseStack( STACK_TYPENAME** stackPtr ) {\
  if( stackPtr ) {\
    if( (*stackPtr) ) {\
      if( (*stackPtr)->slot ) {\
        free( (*stackPtr)->slot );\
        (*stackPtr)->slot = NULL;\
      }\
      \
      free( (*stackPtr) );\
        (*stackPtr) = NULL;\
    }\
  }\
}

#define IMPLEMENT_RELEASESTACKSLOTS( FUNCNAME )\
void FUNCNAME( STACK_TYPENAME** stackPtr,\
  ReleaseSlotFunc releaseSlot )\
  \
  STACKSLOT_TYPENAME* slotPtr;\
  unsigned index;\
  \
  if( stackPtr ) {\
    if( (*stackPtr) ) {\
      if( (*stackPtr)->slot ) {\
        for( index = (*stackPtr)->bottom; index < (*stackPtr)->top; index++ ) {\
          slotPtr = (*stackPtr)->slot[index];\
          \
          if( releaseSlot(&slotPtr) ) {\
            break;\
          }\
        }\
        \
        free( (*stackPtr)->slot );\
        (*stackPtr)->slot = NULL;\
      }\
      \
      free( (*stackPtr) );\
      (*stackPtr) = NULL;\
    }\
  }\
}

#define IMPLEMENT_GROWSTACK( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack ) {\
  STACKSLOT_TYPENAME* newSlot = NULL;\
  unsigned newTop;\
  unsigned newBottom;\
  \
  if( stack == NULL ) { return 1; }\
  \
  if( (((unsigned)-1) - stack->top) < STACK_GROWTH_INCREMENT ) {\
    return 2;\
  }\
  \
  newTop = (stack->top & (~STACK_GROWTH_MASK)) + STACK_GROWTH_INCREMENT;\
  newBottom = newTop - (stack->top - stack->bottom);\
  \
  if( stack->bottom == 0 ) {\
    newSlot = realloc(stack->slot, newTop * sizeof(STACKSLOT_TYPENAME));\
    if( newSlot == NULL ) {\
      return 3;\
    }\
    \
    if( stack->slot ) {\
      memmove( &newSlot[newBottom], &newSlot[stack->bottom],\
        stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
      memset( newSlot, 0, stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
    }\
    \
    stack->slot = newSlot;\
    stack->top = newTop;\
    stack->bottom = newBottom;\
  }\
  \
  return 0;\
}

#define IMPLEMENT_COMPACTSTACK( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack ) {\
  STACKSLOT_TYPENAME* newSlots = NULL;\
  unsigned newTop;\
  unsigned newBottom;\
  \
  if( (stack == NULL) || (stack->top < stack->bottom) ) { return 1; }\
  \
  newTop = stack->top - stack->bottom;\
  newBottom = 0;\
  \
  if( newTop > newBottom ) {\
    if( stack->slot ) {\
      memmove( &stack->slot[0], &stack->slot[stack->bottom],\
        stack->bottom * sizeof(STACKSLOT_TYPENAME) );\
    }\
    \
    newSlots = realloc(stack->slot, newTop * sizeof(STACKSLOT_TYPENAME));\
    if( newSlots == NULL ) {\
      return 2;\
    }\
    \
    stack->slot = newSlots;\
    stack->top = newTop;\
    stack->bottom = newBottom;\
    \
    return 0;\
  }\
  \
  if( stack->slot ) {\
    free( stack->slot );\
    stack->slot = NULL;\
  }\
  \
  stack->top = 0;\
  stack->bottom = 0;\
  \
  return 0;\
}

#define IMPLEMENT_STACKPUSH( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, STACKSLOT_TYPENAME fromItem ) {\
  if( stack == NULL ) { return 1; }\
  \
  if( GrowStack(stack) ) {\
    return 3;\
  }\
  \
  if( stack->slot == NULL ) { return 4; }\
  \
  stack->bottom--;\
  stack->slot[stack->bottom] = fromItem;\
  \
  return 0;\
}

#define IMPLEMENT_STACKPOP( FUNCNAME )\
unsigned Pop( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem ) {\
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }\
  if( toItem == NULL ) { return 2; }\
  \
  (*toItem) = stack->slot[stack->bottom];\
  stack->bottom++;\
  \
  return 0;\
}

#define IMPLEMENT_STACKPEEK( FUNCNAME )\
unsigned Peek( STACK_TYPENAME* stack, STACKSLOT_TYPENAME* toItem ) {\
  if( (stack == NULL) || (stack->top == stack->bottom) ) { return 1; }\
  if( toItem == NULL ) { return 2; }\
  \
  (*toItem) = stack->slot[stack->bottom];\
  \
  return 0;\
}

#define IMPLEMENT_STACKPEEKAHEAD( FUNCNAME )\
unsigned FUNCNAME( STACK_TYPENAME* stack, unsigned byAmount, STACKSLOT_TYPENAME* toItem ) {\
  if( stack == NULL ) { return 1; }\
  \
  if( byAmount >= (stack->top - stack->bottom) ) { return 2; }\
  \
  if( toItem == NULL ) { return 3; }\
  \
  (*toItem) = stack->slot[stack->bottom + byAmount];\
  \
  return 0;\
}

#define IMPLEMENT_STACK_DEFAULT_FUNCTIONS \
  IMPLEMENT_CREATESTACK( CreateStack )\
  IMPLEMENT_RELEASESTACK( ReleaseStack )\
  IMPLEMENT_GROWSTACK( GrowStack )\
  IMPLEMENT_COMPACTSTACK( CompactStack )\
  IMPLEMENT_PUSHSLOT( Push )\
  IMPLEMENT_POPSLOT( Pop )\
  IMPLEMENT_PEEKSLOT( Peek )\
  IMPLEMENT_PEEKAHEADSLOT( PeekAheadSlot )

#undef STACKSLOT_TYPE
#undef STACKSLOT_TYPENAME
#undef STACK_TYPENAME

#endif // STACK_IMPLEMENTATION

#undef STACK_IMPLEMENTATION
