#include  "Core.h"

static inline uint32_t SCANCTRL(uint32_t *StackPointer, struct InstructionKit *MemoryLocations, struct  GraphicStates *RuntimeStates)
{
    if(*StackPointer & 8 << 1 == 1)
    {
        /*
        if (glyh =< threshold)
            {
                RuntimeStates -> scanControl = 1;
            }

        */
    }
    if(*StackPointer & 9 << 1== 1)
    {
        if (MemoryLocations -> Rotation =! 0)
            {
                RuntimeStates -> scanControl = 1;
            }
    }

    if(*StackPointer & 10 << 1== 1)
    {
        if (MemoryLocations -> Stretch =! 0)
            {
                RuntimeStates -> scanControl = 1;
            }
    }
    if (*StackPointer & 11 << 1 == 1)
    {
    //if threshold is not less than or equal
    /*
        if (glyh > threshold)
            {
                 RuntimeStates -> scanControl = 0;
            }

        */
    }
    if(*StackPointer & 12 << 1 == 1)
    {
        if (MemoryLocations -> Rotation == 0)
            {
                RuntimeStates -> scanControl = 0;
            }
    }
    if(*StackPointer & 13 << 1 == 1)
    {

        if (MemoryLocations -> Stretch =! 0)
            {
                RuntimeStates -> scanControl = 0;
            }

    }
}

static inline uint32_t FLIP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
        uint32_t *StackPointer = MemoryLocations -> StackPointer;
        if ( *instructionpointer == 80)
        {
            if ((MemoryLocations -> GlyphFlags[*StackPointer]) & 1 == 1)
            {
            MemoryLocations -> GlyphFlags[*StackPointer] = MemoryLocations -> GlyphFlags[*StackPointer] | 0;
            }
             else
            {
            MemoryLocations -> GlyphFlags[*StackPointer] = MemoryLocations -> GlyphFlags[*StackPointer] | 1;
            }
        }



        else if ( *instructionpointer == 81)
        {
            for (int i = *(StackPointer -1); *(StackPointer -2); i++)
            {
                MemoryLocations -> GlyphFlags[i] = MemoryLocations -> GlyphFlags[i] | 1; //Sets flat to Off
            }
        POP(StackPointer);
        POP(StackPointer);
        }
        else
        {
            for (int i = *(StackPointer - 1); *(StackPointer -2); i++)
            {
                MemoryLocations -> GlyphFlags[i] = MemoryLocations -> GlyphFlags[i] | 1; //Sets flat to Off
            }
        POP(StackPointer);
        POP(StackPointer);
        }

    }
F2Dot14 *SDPVTL(uint8_t *instructionpointer, F2Dot14 *DualProjectionVector)
{
       if (*instructionpointer == 134)
       {
        DualProjectionVector[0] = 1;
        DualProjectionVector[1] = 0;
       }
       else
       {
        DualProjectionVector[0] = 0;
        DualProjectionVector[1] = 1;
       }
return DualProjectionVector;
}


static inline uint32_t ROLL(uint32_t *StackPointer)
{

    uint32_t *dummy;
    *dummy = *(StackPointer - 2);
    *(StackPointer - 2) = *(StackPointer - 1);
    *(StackPointer - 1)= *(StackPointer - 0);
    *StackPointer  = *dummy;
    POP(StackPointer);
    dummy = 0;
    free(dummy);
}

 static inline uint8_t TF_MAX(uint32_t *StackPointer)
{

    uint32_t *dummy = StackPointer ;
    if (*(StackPointer - 1)  < *dummy)
    {
    *(StackPointer - 1)  =  *dummy;
    }
    *dummy = 0;
    POP(StackPointer);
    free(dummy);
    dummy = 0;
}

 static inline uint8_t TF_MIN(uint32_t *StackPointer)
{

    uint32_t *dummy = StackPointer;
    if ( *(StackPointer - 1)  > *dummy)
    {
    *(StackPointer - 1) =  *dummy;
    }
    *dummy = 0;
    POP(StackPointer);
    free(dummy);
    dummy = 0;
}

static inline uint8_t GETINFO(struct InstructionKit *MemoryLocations)
{
    if (*MemoryLocations -> StackPointer   == 1)
    {
        //This is not a standard Font Engine
    }
    else if (*MemoryLocations -> StackPointer & 128 == 128)
    {
        *MemoryLocations -> StackPointer = MemoryLocations -> Rotation;
    }
    else
    {
        *MemoryLocations -> StackPointer = MemoryLocations -> Stretch;
    }
    POP(MemoryLocations -> StackPointer);
}

static inline uint8_t IDEF(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{
int i = *MemoryLocations -> StackPointer;  //Stores address of Function defintion
MemoryLocations -> FunctionDefs[*MemoryLocations -> StackPointer] = (uint16_t)*instructionpointer;
    while (*instructionpointer != 44)
    {
        //load a function in maxp table and execute function the number of time
    }
}

static inline uint32_t INST_CONT(struct InstructionKit *MemoryLocations)
{
    // move instruction to end of instruction list
}

static inline uint8_t SCANTYPE(uint32_t *StackPointer, struct  GraphicStates *RuntimeStates)
{
    RuntimeStates -> scanControl = *StackPointer;
    POP(StackPointer);
}

static inline void FunctionGroupEight(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RunTimeStates )
{
    uint32_t *StackPointer = MemoryLocations -> StackPointer;
    uint8_t LowEnd  = *instructionpointer & 15;
    if(LowEnd < 8)
    {
        if (LowEnd <3)
        {
            FLIP(instructionpointer,  MemoryLocations, RunTimeStates);
        }
        else if (LowEnd < 6)
        {
            SCANCTRL(StackPointer, MemoryLocations, RunTimeStates);
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
                        SDPVTL(instructionpointer,  RunTimeStates -> DualProjectionVector);
                    }
                case 9 :
                    {
                        GETINFO(MemoryLocations);
                    }
                }
        }
        else
            {
                switch(LowEnd)
                {
                case 10 :
                    {
                        IDEF(instructionpointer, MemoryLocations);
                    }
                case 11 :
                    {
                        ROLL(StackPointer);
                    }
                }
            }
    }
        else if (LowEnd < 14)

    {
        switch (LowEnd)
            {
            case 12 :
                {
                    TF_MAX(StackPointer);
                }
            case 13 :
                {
                    TF_MIN(StackPointer);
                }
            }
    }
    else
    {
        switch (LowEnd)
        {
            case 14 :
                {
                    SCANTYPE(StackPointer, RunTimeStates);
                }
            case 15 :
                {
                    INST_CONT(MemoryLocations);
                }
        }
    }
}