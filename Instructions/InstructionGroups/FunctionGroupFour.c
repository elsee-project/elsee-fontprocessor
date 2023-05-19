#include  "Core.h"


__attribute__((always_inline)) static inline F2Dot14 MeasureDistance( struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{   // TO-DO set to stack pointer
    F2Dot14 PointA[2];
    F2Dot14 PointB[2];
    PointA[0] = MemoryLocations -> OriginalPoints[*MemoryLocations -> StackPointer + 1][0];
    PointA[1] = MemoryLocations -> OriginalPoints[*MemoryLocations -> StackPointer + 1][1];
    PointB[0] = MemoryLocations -> OriginalPoints[*MemoryLocations -> StackPointer][0];
    PointB[1] = MemoryLocations -> OriginalPoints[*MemoryLocations -> StackPointer][1];
    POP(MemoryLocations -> StackPointer);
    *MemoryLocations -> StackPointer =   Distance(PointA, PointB);
}

__attribute__((always_inline)) static inline uint32_t WS(struct InstructionKit *MemoryLocations)
{
    uint32_t *dummy;
    MemoryLocations -> StackPointer--;
    MemoryLocations -> Storage[*MemoryLocations -> StackPointer+1] = *MemoryLocations -> StackPointer + 1;
    *(MemoryLocations -> StackPointer + 1) = 0;
    POP(MemoryLocations -> StackPointer);
}

__attribute__((always_inline)) static inline uint32_t RS(struct InstructionKit *MemoryLocations)
{
    MemoryLocations -> Storage[*MemoryLocations -> StackPointer] = *(MemoryLocations -> StackPointer + 1);
    MemoryLocations -> StackPointer++;
}

__attribute__((always_inline)) static inline F2Dot14 WCVTP(struct InstructionKit *MemoryLocations)
{
    uint32_t *StackPointer = MemoryLocations -> StackPointer;
    MemoryLocations -> controlvaluetable[*MemoryLocations -> StackPointer - 2] = MemoryLocations -> controlvaluetable[*(MemoryLocations -> StackPointer - 1 )] * (MemoryLocations -> PixelSize);
    POP(StackPointer);
    POP(StackPointer);
}

__attribute__((always_inline)) static inline uint32_t RCVT(struct InstructionKit *MemoryLocations)
{
    uint32_t *storedlocation;                         //Function adds the value the location of the variable to the location beginning of file
    *storedlocation = MemoryLocations -> controlvaluetable[*MemoryLocations -> StackPointer];   //then loads value at that memory; it then replaces the previous location value
    *MemoryLocations -> StackPointer = *storedlocation;
}

__attribute__((always_inline)) static inline F2Dot14 GC(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    F2Dot14 *Coordinates;
    F2Dot14 *ProjectionVector = RuntimeStates -> ProjectionVector;
    MemoryLocations -> StackPointer -= 2;
    if(*instructionpointer ==  46)
    {
    Coordinates = Point(MemoryLocations, *MemoryLocations -> StackPointer ,RuntimeStates -> zp[2]);
    Coordinates = ProjectionDistance(Coordinates, ProjectionVector);
    }
    else
    {
    Coordinates[0] = MemoryLocations -> OriginalPoints[0][*MemoryLocations -> StackPointer];
    Coordinates[1] = MemoryLocations -> OriginalPoints[1][*MemoryLocations -> StackPointer];// This uses Outpline value
    Coordinates = ProjectionDistance(Coordinates, ProjectionVector);
    }
    *MemoryLocations -> StackPointer  = 0;
    *(MemoryLocations -> StackPointer  + 1) = 0;
}

__attribute__((always_inline)) static inline F2Dot14 *SCFS(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    F2Dot14  *Coordinates;
    Coordinates = Point(MemoryLocations, (*MemoryLocations -> StackPointer - 1), RuntimeStates -> zp[2]);
    F2Dot14 *Magnitude = MoveRelProjFree(Point(MemoryLocations, *(MemoryLocations -> StackPointer - 2), RuntimeStates -> zp[0]), RuntimeStates,  MemoryLocations -> controlvaluetable[*(MemoryLocations -> StackPointer - 1)]);    //Find Vectors of the same magnitude towards this direction
    Coordinates[0] = Magnitude[0];
    Coordinates[1] = Magnitude[1];
    free(Magnitude);
    POP(MemoryLocations ->StackPointer);
    return Coordinates;
}

__attribute__((always_inline)) static inline F2Dot14 MeasurePixelsPerEM(struct InstructionKit *MemoryLocations)
{   // TO-DO set to stack pointer
    MemoryLocations -> StackPointer++;
    *MemoryLocations -> StackPointer  = MemoryLocations -> PixelSize;

}
__attribute__((always_inline)) static inline F2Dot14 MeasurePointSize(struct InstructionKit *MemoryLocations)
{   // TO-DO set to stack pointer
    MemoryLocations -> StackPointer++;
    *MemoryLocations -> StackPointer  = MemoryLocations -> PixelSize;
}

__attribute__((always_inline)) static inline uint8_t FlipOff(uint8_t autoFlip)
{
        autoFlip = 0;
}
__attribute__((always_inline)) static inline uint8_t FlipOn(uint8_t autoFlip)
{
    autoFlip = 1;
}

__attribute__((always_inline)) static inline void FunctionGroupFour(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{

    uint8_t LowEnd  = *instructionpointer & 15;
    uint32_t *StackPointer = MemoryLocations -> StackPointer;
    if(LowEnd < 8)
    {
        if (LowEnd <4)
        {
                switch (LowEnd)
                {
                case 2 :
                        {
                            RS(MemoryLocations);
                        }
                case 3 :
                        {
                            WS(MemoryLocations);
                        }
                }

        }
        else if (LowEnd < 6)
        {
            switch (LowEnd)
            {
                case 4 :
                        {
                            WCVTP(MemoryLocations);
                        }
                case 5 :
                        {
                            RCVT(MemoryLocations);
                        }
            }
        }
        else {
                 GC(instructionpointer, MemoryLocations, RuntimeStates);
            }
    }


    else if (LowEnd < 12)
    {
        if (LowEnd < 10)
        {
            SCFS(MemoryLocations, RuntimeStates);
        }
    }
    else
        {
            switch (LowEnd)
                {
                default :
                    {
                        MeasureDistance(MemoryLocations, RuntimeStates);
                    }
                case 15 :
                        {
                        // DEBUG FUNCTION
                        }
                }
        }
}

