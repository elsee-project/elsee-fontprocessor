#include  "Core.h"


__attribute__((always_inline)) static inline void ADD(F2Dot14 *StackPointer)
{
    F2Dot14 *dummy;
    *dummy = *StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  + *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}
__attribute__((always_inline)) static inline void SUB(F2Dot14 *StackPointer)
{
    F2Dot14 *dummy;
    *dummy = *StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  - *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}


__attribute__((always_inline)) static inline void DIV(F2Dot14 *StackPointer)
{
    F2Dot14 *dummy;
    *dummy = (F2Dot14)*StackPointer;
    StackPointer --;
    *StackPointer  = *StackPointer  / *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}

F2Dot14 MUL(F2Dot14 *StackPointer)
{
    F2Dot14 *dummy;
    *dummy = *StackPointer;
    StackPointer--;
    *StackPointer  = *StackPointer  * *dummy;
    free(dummy);
    dummy = 0;
    *(StackPointer + 1) = 0;
}

__attribute__((always_inline)) static inline uint8_t NEG(F2Dot14 *StackPointer)
{
    *StackPointer  = -*(StackPointer);

}

__attribute__((always_inline)) static inline void FunctionGroupSix(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    uint8_t LowEnd  = *instructionpointer & 15;
    F2Dot14 *StackPointer = (F2Dot14 *) MemoryLocations -> StackPointer;
    switch (LowEnd)

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
                            ADD(StackPointer);
                        }
                case 1 :
                        {
                            SUB(StackPointer);
                        }
                }
            }
            else
            {
                switch (LowEnd)
                {
                case 2 :
                        {
                            DIV(StackPointer);
                        }
                case 3 :
                        {
                            MUL(StackPointer);
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
                            ft_abs(StackPointer);
                        }
                case 5 :
                        {
                            NEG(StackPointer);
                        }
            }
        }
        else
        {
            switch (LowEnd)
            {
                case 6 :
                        {
                            ft_floor(*StackPointer);
                        }
                case 7 :
                        {
                            ft_ceil(*StackPointer);
                        }
            }
        }
    }

    else if (LowEnd < 8)
    {
     ROUND(LowEnd, RuntimeStates -> roundState, *StackPointer);   //Low End has the values for what to add
    }
}
