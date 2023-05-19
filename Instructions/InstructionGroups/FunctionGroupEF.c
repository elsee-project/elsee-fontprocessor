#include  "Core.h"
#include <math.h>


__attribute__((always_inline)) static inline void MIRP(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    F2Dot14 *FstPnt = Point(MemoryLocations, *(MemoryLocations -> StackPointer - 1), RuntimeStates-> zp[1]);
    F2Dot14 *SndPnt = Point(MemoryLocations, RuntimeStates-> rp[1], RuntimeStates-> zp[0]);;
    F2Dot14 distance = Distance(FstPnt, SndPnt);
    if (*instructionpointer & 1 == 1)
    {
        RuntimeStates -> rp[0] = *(MemoryLocations -> StackPointer);
    }
    if (*instructionpointer & 2 == 2)
    {
        if(distance < RuntimeStates -> minDistance)
        {
                    //Move to minimum stance of same direction
        }
    }

    if (*instructionpointer & 3 == 3)
    {
        ROUND(-127, RuntimeStates -> roundState, (F2Dot14)*MemoryLocations -> StackPointer);
    }
    RuntimeStates -> rp[0] =  RuntimeStates -> rp[1];
    RuntimeStates -> rp[2] =  *(MemoryLocations -> StackPointer);
    POP(MemoryLocations -> StackPointer);
}
