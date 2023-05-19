#include  "Core.h"

extern void InstructionExecute(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);

__attribute__((always_inline)) static inline uint8_t  DUP(uint32_t *StackPointer)
{
*(StackPointer  + 1) = *StackPointer;
}


__attribute__((always_inline)) static inline uint32_t CLEAR(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer;

    for (int i = sizeof(*dummy); i > 0; i--)
     {
         *StackPointer = 0;                  // A values turn to 0
     }
    free(dummy);
    dummy = 0;
}

__attribute__((always_inline)) static inline uint8_t SWAP(uint32_t *StackPointer)
{
    uint32_t *dummy = StackPointer;
    StackPointer--;
    *StackPointer  = *StackPointer  * *dummy;
    *dummy = *StackPointer  / *dummy;
    *StackPointer  = *StackPointer  / *dummy;
    free(dummy);
    dummy = 0;
}

__attribute__((always_inline)) static inline uint8_t DEPTH(uint32_t *StackPointer, uint32_t *Stack)
{
    StackPointer++;
    *StackPointer = StackPointer - Stack;
}

__attribute__((always_inline)) static inline F2Dot14 ALIGNPTS(uint32_t *StackPointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {

        F2Dot14 *Coord1 = Point(MemoryLocations, *StackPointer, RuntimeStates -> zp[2]);
        F2Dot14 *Coord2 = Point(MemoryLocations, *(StackPointer - 1), RuntimeStates -> zp[2]);
        ProjectionDistance(RuntimeStates -> ProjectionVector, Coord1);
        ProjectionDistance(RuntimeStates -> ProjectionVector, Coord2);
        F2Dot14 distance = Distance(Coord1, Coord2);
        if (*StackPointer > *(StackPointer - 1))
        {
            MoveRelProjFree(Point(MemoryLocations, *StackPointer, RuntimeStates -> zp[2]), RuntimeStates , distance/2);
            MoveRelProjFree(Point(MemoryLocations, *(StackPointer - 1), RuntimeStates -> zp[2]), RuntimeStates ,  -distance/2);
        }
        else
        {
           MoveRelProjFree(Point(MemoryLocations, *StackPointer, RuntimeStates -> zp[2]), RuntimeStates , -distance/2);
           MoveRelProjFree(Point(MemoryLocations, *(StackPointer - 1), RuntimeStates -> zp[2]), RuntimeStates ,  distance/2);
        }
         POP(StackPointer);
    }

__attribute__((always_inline)) static inline uint32_t NPUSH(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{   uint8_t *dummy;
    *dummy = *(instructionpointer + 1);
    if (*instructionpointer = 41)
        {
        instructionpointer += 2;
         for(int i = 0; i < *dummy; i++)
         {
             *instructionpointer = (uint8_t)*(MemoryLocations -> StackPointer + i);
         }
        }
    else
        {
         instructionpointer += 2;
         for(int i = 0; i < *dummy*2; i++ )
         {
             *instructionpointer = (uint16_t)*(MemoryLocations -> StackPointer + i);
         }
        }
}
__attribute__((always_inline)) static inline uint32_t CINDEX(uint32_t *StackPointer)
{   uint32_t dummy;
    *StackPointer = *(StackPointer - *StackPointer);  // Loads value at location of the top of stack minus the value stored and replaces the l-value with the stored value
}

__attribute__((always_inline)) static inline uint32_t MINDEX(uint32_t *StackPointer)
{
    *StackPointer  = *(StackPointer  - *StackPointer );
    *(StackPointer  - *StackPointer) = 0;
}

__attribute__((always_inline)) static inline uint8_t FDEF(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{
  //Stores address of Function defintion
    MemoryLocations -> FunctionDefs[*MemoryLocations -> StackPointer] = (uint16_t)*instructionpointer;
    while (*instructionpointer != 44)
    {



    }
}
__attribute__((always_inline)) static inline uint32_t UntouchPoint(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)

{   F2Dot14 *FreedomVector = RuntimeStates -> FreedomVector;

    if (FreedomVector[0] == 1)
        {
            MemoryLocations -> TouchedPoints[(*MemoryLocations -> StackPointer)][0];
        }
    else if (FreedomVector[1] == 1)
        {
            MemoryLocations -> TouchedPoints[(*MemoryLocations -> StackPointer)][1];
        }
    else
    {
        MemoryLocations -> TouchedPoints[(*MemoryLocations -> StackPointer)][0];
        MemoryLocations -> TouchedPoints[(*MemoryLocations -> StackPointer)][1];
    }
    POP(MemoryLocations ->StackPointer);
}



uint8_t CALL(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint8_t *dummy = instructionpointer;
    instructionpointer = (uint8_t*)MemoryLocations -> StackPointer;
    POP(MemoryLocations ->StackPointer);
    while (*instructionpointer != 44)
        {
            InstructionExecute(instructionpointer, MemoryLocations, RuntimeStates);
        }
    instructionpointer = dummy;
    free(dummy);
    dummy = 0;
}

uint8_t LOOPCALL(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint32_t *dummy;
    *dummy = *MemoryLocations -> StackPointer;
    for (int i = 0; i <= *dummy; i++)
        {
        CALL(instructionpointer, MemoryLocations, RuntimeStates);
        }
}

__attribute__((always_inline)) static inline F2Dot14 MDAP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    if (*instructionpointer == 45)
    {
      ROUND(-127, RuntimeStates -> roundState, *Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[0]));
      RuntimeStates -> rp[0]  = *MemoryLocations -> StackPointer;
      RuntimeStates -> rp[1]  = *MemoryLocations -> StackPointer;
      POP(MemoryLocations ->StackPointer);
    }
    else
    {
      Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[0]);
      RuntimeStates -> rp[0]  = *MemoryLocations -> StackPointer;
      RuntimeStates -> rp[1]  = *MemoryLocations -> StackPointer;
      POP(MemoryLocations ->StackPointer);
    }
}

__attribute__((always_inline)) static inline void FunctionGroupTwo(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
uint32_t *StackPointer = MemoryLocations -> StackPointer;
uint8_t LowEnd  = *instructionpointer & 15;
if(LowEnd < 8)
{
    if (LowEnd <4)
    {
        if (LowEnd < 2)
        {
            switch (LowEnd)
            {
                case 0 :
                        {
                            DUP(StackPointer);
                        }
                case 1 :
                        {
                            POP(StackPointer);
                        }
            }
        }
        else
        {
             switch (LowEnd)
            {
            case 2 :
                    {
                        CLEAR(StackPointer);
                    }
            case 3 :
                    {
                        SWAP(StackPointer);
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
                        DEPTH(StackPointer , MemoryLocations -> Stack);
                    }
            case 5 :
                    {
                        CINDEX(StackPointer);
                    }
            }
       }
        else
        {
        switch (LowEnd)
            {
            case 6 :
                    {
                        MINDEX(StackPointer);
                    }
            case 7 :
                    {
                        ALIGNPTS(StackPointer, MemoryLocations, RuntimeStates);
                    }
            }
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
                                NPUSH(instructionpointer,  MemoryLocations);
                            }
                    case 9 :
                            {
                                UntouchPoint(MemoryLocations, RuntimeStates);
                            }
                }
        }
        else
            {
                switch(LowEnd)
                {
                    case 10 :
                            {
                                LOOPCALL(instructionpointer, MemoryLocations, RuntimeStates);
                            }
                    case 11 :
                            {
                                CALL(instructionpointer,  MemoryLocations, RuntimeStates);
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
                        FDEF(instructionpointer,  MemoryLocations);
                        }
            }
    }
    else
        {
        MDAP(instructionpointer, MemoryLocations, RuntimeStates);
        }
}
