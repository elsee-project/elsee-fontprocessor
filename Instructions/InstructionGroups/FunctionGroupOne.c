#include  "Core.h"


static inline uint8_t SRP(uint8_t *instructionpointer, uint32_t *StackPointer, struct GraphicStates *RuntimeStates)
{
    RuntimeStates -> rp[*instructionpointer%16] = *StackPointer;
}

static inline uint8_t SZPS(uint32_t *StackPointer, struct GraphicStates *RuntimeStates)
{
    RuntimeStates -> zp[0] = RuntimeStates -> zp[*StackPointer];
    RuntimeStates -> zp[1] = RuntimeStates -> zp[*StackPointer];
    RuntimeStates -> zp[2] = RuntimeStates -> zp[*StackPointer];
    POP(StackPointer);
}

static inline uint8_t SZP(uint8_t *instructionpointer, uint32_t *StackPointer, struct GraphicStates *RuntimeStates)
{
    RuntimeStates -> zp[*StackPointer] = RuntimeStates -> zp[(*instructionpointer) - 13];
    POP(StackPointer);
}

static inline uint8_t JumpRel(uint8_t *instructionpointer,  uint32_t *StackPointer)
{
    *StackPointer  = *instructionpointer + *StackPointer;
    StackPointer--;
}

static inline uint32_t sloop(uint32_t loop, uint32_t *StackPointer)
{
    loop = *StackPointer ;
    POP(StackPointer);
}

static inline uint32_t RTG (uint32_t roundState)
{
    roundState = 3;
    return roundState;
}

static inline uint32_t RTHG (uint32_t roundState)
{
    roundState = 2;
    return roundState;
}

static inline uint8_t SMD(F2Dot14 minDistance, uint32_t *StackPointer)
{
    minDistance = *StackPointer;
    POP(StackPointer);
}

static inline F2Dot14 SCVTCI(F2Dot14 controlvalue_cut_in , uint32_t *StackPointer)
{
    controlvalue_cut_in = *StackPointer;
    POP(StackPointer);
}

static inline F2Dot14 SSWCI(F2Dot14 single_width_cut_in , uint32_t *StackPointer)
{
    single_width_cut_in = *StackPointer;
    POP(StackPointer);
}

static inline F2Dot14 SSW(F2Dot14 single_width_value  , uint32_t *StackPointer)
{
    single_width_value = *StackPointer;
    POP(StackPointer);
}

    //uint16_t ELSE   = 27;

static inline void FunctionGroupOne(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint8_t LowEnd  = *instructionpointer & 15;
    uint32_t *StackPointer = MemoryLocations -> StackPointer;
    if(LowEnd < 8)
    {
        if (LowEnd < 3)
        {
            SRP(instructionpointer, StackPointer, RuntimeStates);
        }
        else if (LowEnd > 7)
        {
            if(LowEnd == 6)
            {
                SZP(instructionpointer, StackPointer, RuntimeStates);
            }
            else
            {
                SZPS(StackPointer, RuntimeStates);
            }
        }
        else
        {
            sloop(RuntimeStates -> loop, StackPointer);
        }
    }
    else if (LowEnd < 12)
    {
        if (LowEnd < 10)
        {
            switch (LowEnd)
            {
                case 8 :
                    {
                        RuntimeStates -> roundState = RTG(RuntimeStates -> roundState);
                    }
                case 9 :
                    {
                        RuntimeStates -> roundState = RTHG(RuntimeStates -> roundState);
                    }
            }
        }
        else
        {
        SMD(RuntimeStates -> minDistance, StackPointer);
        }
    }
    else if (LowEnd < 14)
    {
        switch (LowEnd)
        {
            case 12 :
                {
                    JumpRel(instructionpointer, StackPointer);
                }

            case 13 :
                {
                    SCVTCI(RuntimeStates -> controlvalue_cut_in , StackPointer);
                }
        }
    }
    else
    {
        switch (LowEnd)
        {
            case 14 :
                {
                    SSWCI(RuntimeStates -> single_width_cut_in ,  StackPointer);
                }
            case 15 :
                {
                    SSW(RuntimeStates -> single_width_cut_in ,  StackPointer);
                }
        }
    }
}