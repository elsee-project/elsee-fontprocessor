#include  "Core.h"


static inline F2Dot14 WCVTF(struct InstructionKit *MemoryLocations)
{
    MemoryLocations -> StackPointer -= 2;
    MemoryLocations -> controlvaluetable[*MemoryLocations -> StackPointer] = MemoryLocations -> controlvaluetable[*(MemoryLocations -> StackPointer + 1)];
    POP(MemoryLocations -> StackPointer);
    POP(MemoryLocations -> StackPointer);
}


static inline uint8_t DELTAP (uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint32_t *dummy = MemoryLocations -> StackPointer ;
    uint8_t mostsig;
    uint8_t leastsig;
    F2Dot14 *P;
    MemoryLocations -> StackPointer ++;
    for (int i = (*instructionpointer - 73); i > 0 ; i ++)
    {
        RuntimeStates -> DeltaShift += 15;
    }
    for (int i = 0; i < *MemoryLocations -> StackPointer; i++)
    {
    for (int i = 0; i < (*dummy)*2; i += 2)
        {
            RuntimeStates -> DeltaShift += 15 & MemoryLocations -> Stack[i+1];
            P = Point(MemoryLocations, MemoryLocations -> Stack[i], RuntimeStates ->zp[2]);
            *P += (15 & MemoryLocations -> Stack[i+1] >> 4) * (MemoryLocations -> PixelSize) + RuntimeStates -> DeltaShift;
        }
    }
    free(dummy);
    dummy = 0;
}


static inline uint8_t DELTAC (uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint32_t *dummy = MemoryLocations -> StackPointer ;
    uint8_t mostsig;
    uint8_t leastsig;
    MemoryLocations -> StackPointer ++;
    for (int i = (*instructionpointer - 73); i > 0 ; i ++)
    {
        RuntimeStates -> DeltaShift += 15;
    }
    for (int i = 0; i < *MemoryLocations -> StackPointer; i++)
    {
    for (int i = 0; i < (*dummy)*2; i += 2)
        {
            RuntimeStates -> DeltaShift += 15 & MemoryLocations -> Stack[i+1];
            MemoryLocations -> controlvaluetable[MemoryLocations -> Stack[i]] += (15 & MemoryLocations -> Stack[i+1] >> 4) * (MemoryLocations -> PixelSize) + RuntimeStates -> DeltaShift;
        }
    }
    free(dummy);
    dummy = 0;
}
uint32_t SROUND (uint32_t *StackPointer, uint8_t *SuperBool)
{
    *SuperBool = 1;
    return *StackPointer;
}

uint32_t S45ROUND (uint32_t *StackPointer, uint8_t *SuperBool)
{
    *SuperBool = 2;
    return *StackPointer;
    //Rounded to a value added with (float)(value/period)
}

static inline uint8_t JumpFalse(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{
    if(*MemoryLocations -> StackPointer  == 0)
    {
        *MemoryLocations -> StackPointer  = *instructionpointer + *MemoryLocations -> StackPointer;
        MemoryLocations -> StackPointer--;
    }
}
static inline uint8_t JumpTrue(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{
    if(*MemoryLocations -> StackPointer  == 1)
    {
        *MemoryLocations -> StackPointer  = *instructionpointer + *MemoryLocations -> StackPointer ;
        MemoryLocations -> StackPointer--;
    }
}

static inline uint32_t ROFF(uint32_t roundState)
{
    roundState = 5;
    return roundState;
}

static inline uint32_t RUTG(uint32_t roundState)
{
    roundState = 4;
    return roundState;
}

static inline uint32_t RDTG(uint32_t roundState)
{
    roundState = 3;
    return roundState;
}

static inline void FunctionGroupSeven(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
uint8_t LowEnd  = *instructionpointer & 15;
uint32_t *StackPointer = MemoryLocations -> StackPointer;

    if(LowEnd < 8)
    {
        if (LowEnd <4)
        {
            if (LowEnd < 3)
            {
                if (LowEnd = 0)
                {
                    WCVTF(MemoryLocations);
                }
                else
                {
                    DELTAP (instructionpointer, MemoryLocations, RuntimeStates);
                }
            }
        }
        else if (LowEnd < 6)
        {
            DELTAC(instructionpointer, MemoryLocations, RuntimeStates);
        }

        else {
                switch (LowEnd)
                {
                    case 6 :
                        {
                            RuntimeStates -> roundState = SROUND (StackPointer, RuntimeStates -> SuperBool);
                        }
                    case 7 :
                        {
                            RuntimeStates -> roundState = S45ROUND (StackPointer, RuntimeStates -> SuperBool);
                        }
                }
            }
    }

    else if (LowEnd < 10)
    {
                switch (LowEnd)
                    {
                    case 8 :
                        {
                            JumpFalse(instructionpointer, MemoryLocations);
                        }
                    case 9 :
                        {
                            JumpTrue(instructionpointer, MemoryLocations);
                        }
                    }
    }
    else
        { switch(LowEnd)
            {
                case 10 :
                    {
                       RuntimeStates -> roundState = ROFF(RuntimeStates -> roundState);
                    }
                case 11 :
                    {
                        RuntimeStates -> roundState = RUTG(RuntimeStates -> roundState);
                    }
                case 12 :
                    {
                        RuntimeStates -> roundState = RDTG(RuntimeStates -> roundState);
                    }
            }
        }
instructionpointer++;
}