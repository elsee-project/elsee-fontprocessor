#include  "Core.h"


static inline F2Dot14 MDRP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    F2Dot14 *FstPnt = Point(MemoryLocations, *(MemoryLocations -> StackPointer - 1), RuntimeStates-> zp[1]);
    F2Dot14 *SndPnt = Point(MemoryLocations, RuntimeStates-> rp[1], RuntimeStates-> zp[0]);;
    F2Dot14 distance = Distance(FstPnt, SndPnt);
    if (*instructionpointer & 1 == 1)
        {
            RuntimeStates -> rp[0] = *(MemoryLocations -> StackPointer - 1);
        }
    if (*instructionpointer & 2 == 2)
        {
            if(Distance(MemoryLocations->OriginalPoints[*(MemoryLocations -> StackPointer - 1)],MemoryLocations-> OriginalPoints[*(MemoryLocations -> StackPointer - 1)]) < RuntimeStates -> minDistance)
            {
                //Move to minimum stance of same direction
            }
        }

    if (*instructionpointer & 3 == 3)
    {
        ROUND(0, RuntimeStates -> roundState, distance);
    }
    if (*instructionpointer == 0)
     {
         //Grey, White and Black distances
     }
    RuntimeStates -> rp[1] = RuntimeStates -> rp[0];
    RuntimeStates -> rp[2] =  (*MemoryLocations -> StackPointer);
    POP(MemoryLocations -> StackPointer);
}
