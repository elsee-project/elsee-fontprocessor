#include  "Core.h"

static inline uint8_t AND(uint32_t *StackPointer)
{
    uint32_t *dummy;
    *dummy = *StackPointer;
    *(StackPointer - 1)  = (*(StackPointer - 1) & *dummy);
    free(dummy);
    dummy = 0;
    POP(StackPointer);
}

 static inline uint8_t LogOR(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer ;
    *(StackPointer - 1)  = (*(StackPointer - 1) | *dummy);
    free(dummy);
    dummy = 0;
    POP(StackPointer);
}

static inline uint8_t LogNOT(uint32_t *StackPointer)
{
    *StackPointer  = ~(*StackPointer);
}

static inline uint8_t DELTAP (char *instructionpointer, struct InstructionKit *Memorylocations)
{

    uint32_t *dummy = Memorylocations -> StackPointer ;
    uint8_t mostsig;
    uint8_t leastsig;
    Memorylocations -> StackPointer ++;
    for (int i = 0; i < (*dummy)*2; i += 2)
        {
            Memorylocations -> controlvaluetable[Memorylocations -> Stack[i]];
            mostsig = 15 & (Memorylocations -> controlvaluetable[Memorylocations -> Stack[i+1]] >> 4);
            leastsig = 15 & Memorylocations -> controlvaluetable[Memorylocations -> Stack[i+1]];

        }
        for (int i = (*instructionpointer - 73); i > 0 ; i ++)
        {
            mostsig += 15;
            leastsig += 15;

        }

    free(dummy);
    dummy = 0;
}
uint32_t SDB(uint32_t *StackPointer, struct GraphicStates *RuntimeStates)
    {
        RuntimeStates -> DeltaBase = *StackPointer ;
        POP(StackPointer);
    }

uint32_t SDS(uint32_t *StackPointer, struct GraphicStates *RuntimeStates)
    {

        RuntimeStates -> DeltaShift = *StackPointer;
        POP(StackPointer);
    }

static inline uint8_t LTEQ(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer;
    *StackPointer  = (*StackPointer  <= *dummy);
    free(dummy);
    dummy = 0;
}

static inline uint8_t LT(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer;
    *StackPointer  = (*StackPointer  < *dummy);
    free(dummy);
    dummy = 0;
}

static inline uint8_t GTEQ(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer ;
    *StackPointer  = (*StackPointer  >=*dummy);
    free(dummy);
    dummy = 0;
}
static inline uint8_t GT(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer ;
    *StackPointer  = (*StackPointer  > *dummy);
    free(dummy);
    dummy = 0;
}

static inline uint8_t NOTEQ(uint32_t *StackPointer)
{
    uint32_t *dummy;
    *dummy = *StackPointer;
    StackPointer--;
    *StackPointer  = (*StackPointer  != *dummy);
    free(dummy);
    dummy = 0;

}
static inline uint8_t EQ(uint32_t *StackPointer )
{
    uint32_t *dummy;
    *dummy = *StackPointer;
    *StackPointer  = (*StackPointer  == *dummy);
    free(dummy);
    dummy = 0;
}

static inline uint8_t EVENORODD(uint8_t *instructionpointer, uint32_t *StackPointer, uint32_t roundState)
{

    ROUND(-127, roundState, (F2Dot14) *StackPointer);
    if (*instructionpointer == 56)
    {
        if (*StackPointer & 1 == 1)
        {
            *StackPointer = 1;
        }
        else
        {
            *StackPointer = 0;
        }
    }
    else
    {
        if (*StackPointer & 0 == 0)
        {
        *StackPointer = 0;
        }
        else
        {
        *StackPointer = 1;
        }
    }

}


static inline uint8_t IF(uint8_t *instructionpointer, uint32_t *StackPointer)
{


    POP(StackPointer);
    if(*StackPointer  == 1)
    {
         while(*instructionpointer != 89 || 27)
        {
                       // Put execute function here
        }
    }
    else
    {
        while(*instructionpointer != 89)
        {
                     //figure out later
        }
    }
             POP(StackPointer);
}
    //uint16_t EIF  = 89;
    uint16_t DELTAP1   = 93;
static inline void FunctionGroupFive(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint32_t *StackPointer = MemoryLocations -> StackPointer;
    uint8_t LowEnd  = *instructionpointer & 15;
    if(LowEnd < 8)
    {
        if (LowEnd < 4)
        {
            if (LowEnd < 2)
            {
                switch (LowEnd)
                {
                    case 0 :
                        {
                            LTEQ(StackPointer);
                        }
                    case 1 :
                        {
                            LT(StackPointer);
                        }
                }
            }
            else
            {
                switch (LowEnd)
                 {
                    case 2 :
                        {
                            GTEQ(StackPointer);
                        }
                    case 3 :
                        {
                            GT(StackPointer);
                        }
                }
            }
        }
        else if (LowEnd < 6)
        {
            switch (LowEnd)
                {
                case 4 :
                    {
                        EQ(StackPointer);
                    }
                case 5 :
                    {
                        NOTEQ(StackPointer);
                    }
                }
        }
        else
            {
                EVENORODD(instructionpointer, StackPointer, RuntimeStates -> roundState);
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
                        IF(instructionpointer, StackPointer);
                    }
                case 9 :
                    {

                    }
            }
        }
        else
        {
            switch(LowEnd)
                {
                case 10 :
                    {
                        AND(StackPointer);
                    }
                case 11 :
                    {
                        LogOR(StackPointer);
                    }
                }
        }
    }
    else if (LowEnd < 14)
        {   switch (LowEnd)
            {
                case 12 :
                    {
                        LogNOT(StackPointer);
                    }

                case 13 :
                    {

                    }
             }
        }
    else
        {
            switch (LowEnd)
            {
                case 14 :
                    {
                        SDB(StackPointer, RuntimeStates);
                    }
                case 15 :
                    {
                        SDS(StackPointer, RuntimeStates);
                    }
            }
        }
 }




