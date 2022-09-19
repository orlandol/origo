#ifndef STACK_H
#define STACK_H

typedef unsigned StackSlot;

typedef struct Stack {
  StackSlot* slot;
  unsigned top;
  unsigned bottom;
} Stack;

typedef unsigned (*ReleaseSlotFunc)( StackSlot** slotPtr );

Stack* CreateStack();
void ReleaseStack( Stack** stackPtr );
void ReleasePointerStack( Stack** stackPtr,
  ReleaseSlotFunc releaseSlot );

unsigned GrowStack( Stack* stack );
unsigned ReserveStack( Stack* stack );
unsigned CompactStack( Stack* stack );

unsigned Push( Stack* stack, StackSlot fromItem );
unsigned Pop( Stack* stack, StackSlot* toItem );

unsigned PeekAhead( Stack* stack, unsigned byAmount, StackSlot* toItem );
unsigned Peek( Stack* stack, StackSlot* toItem );

#endif
