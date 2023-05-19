#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <FontEngineInit.h>


F2Dot14 Coordinates[2];

__attribute__((always_inline)) static inline void POP(uint32_t *StackPointer)
{
    *StackPointer = 0;   //Value is discarded
    StackPointer--;
}

__attribute__((always_inline)) static inline F2Dot14 *Point(struct InstructionKit *MemoryLocations, uint32_t point, uint32_t zp)
{   

    if (zp == 1)
    {
        Coordinates[0] = MemoryLocations -> TwilightZone[point][0];
        Coordinates[1] = MemoryLocations -> TwilightZone[point][1];
        return Coordinates;
    }
    else
    {
        Coordinates[0] = MemoryLocations -> GlyphZone[point][0];
        Coordinates[1] = MemoryLocations -> GlyphZone[point][1];
        return Coordinates;
    }
}


__attribute__((always_inline)) static inline F2Dot14 Distance(F2Dot14 CoordinatesA[2], F2Dot14 CoordinatesB[2])
{
    F2Dot14 DisX = CoordinatesA[0] - CoordinatesB[0];
    F2Dot14 DisY = CoordinatesA[1] - CoordinatesB[1];
    F2Dot14 distance = DisX * DisX + DisY * DisY; //x2 *x1 + y2 +y1
    distance = sqrt(distance);
    return distance;
}

__attribute__((always_inline)) static inline F2Dot14 FreeTransform(F2Dot14 *FreedomVector, F2Dot14 Mag, int index)
{
    return FreedomVector[index] * Mag;
}

// Totaly distance is Proj Vector times Coordinate divided by mag of coordinate, M
//Mutliply  Magnitude of Displacement by x and y


__attribute__((always_inline)) static inline F2Dot14 *ProjectionDistance(F2Dot14 ProjectionVector[2], F2Dot14 Coordinates[2])
{
    F2Dot14 Mag = Distance(Coordinates, 0);
    Coordinates[0] = ProjectionVector[0] * (Coordinates[0])/Mag;
    Coordinates[1] = ProjectionVector[1] * (Coordinates[1])/Mag;
    return Coordinates;
}

__attribute__((always_inline)) static inline F2Dot14 *DualProjectionTransform(F2Dot14 DualProjectionVector[2], F2Dot14 Coordinates[2])
{
    F2Dot14 Mag = Distance(Coordinates, 0);
    Coordinates[0] = DualProjectionVector[0] * Coordinates[0]/Mag;
    Coordinates[1] = DualProjectionVector[1] * Coordinates[1]/Mag;
    return Coordinates;
}

__attribute__((always_inline)) static inline F2Dot14 *MoveRelProjFree(F2Dot14 Coordinates[2], struct GraphicStates *RuntimeStates, F2Dot14 Arg)
{
    F2Dot14 *FreedomVector = RuntimeStates -> FreedomVector;
    F2Dot14 Magnitude[2];
    Magnitude[0] = FreeTransform(FreedomVector, Arg, 0);    //Find Vectors of the same magnitude towards this direction
    Magnitude[1] = FreeTransform(FreedomVector, Arg, 1);
    ProjectionDistance(RuntimeStates -> ProjectionVector, Magnitude);  // Find Projection of this magnitude and Add it to Coordinaates
    Coordinates[0] += Magnitude[0];
    Coordinates[1] += Magnitude[1];
    return Coordinates;
}

__attribute__((always_inline)) static inline uint8_t ft_abs(F2Dot14 *StackPointer)
{
    if (*StackPointer  > 0)
        {
            *StackPointer  == -(*StackPointer);
        }
}
__attribute__((always_inline)) static inline uint8_t ft_floor(F2Dot14 StackPointer)
{
    floor(StackPointer);
}
__attribute__((always_inline)) static inline uint8_t ft_ceil(F2Dot14 StackPointer)
{
    ceil(StackPointer);
}

float PointFiveRound(float Arg)
{
    int deci = (int)Arg & ((255) << 24); //TODO Figure out how to prevent shift overflow complaints and replace 24 with ((sizeof(Arg)) - 9))
    if ((int)Arg & (deci << 1) == 1)
    {
        if ((int)Arg & (deci - 1 << 1) == 1)
        {
            Arg = (int)Arg & (255 << 23);
            Arg = Arg + 1;
        }
        else
        {
        Arg = (int)Arg & (255 << 23);
        Arg = (int)Arg | 23 << 1;
        }
    }
    else if ((int)Arg & (deci - 1 << 1) == 1)
    {
        Arg = (int)Arg & (255 << 23);
        Arg = (int)Arg | 23 << 1;

    }
    else
    {
        Arg = (int)Arg & (255 << 23);
    }
}

int StandardRound(float Arg)
{
    int deci = (int)Arg & ((255) << (24));
    if ((int)Arg & deci == 1)
    {
        Arg = (int)Arg & (255 << 23);
        return 1;
    }
    else
    {
        return 0;
    }
}

F2Dot14 ROUND(char boolean, uint32_t roundState, F2Dot14 Arg)
{

    if(boolean == -127) //Exception condition in case boolean not needed
    {
        
    }

    else if (boolean < 3)
    {
        if(boolean == 1)
        {
            F2Dot14 Period = roundState & 192;
            F2Dot14 Phase = Period/(roundState  & 48);
            Arg -= Phase;
            Arg+= (F2Dot14)(((roundState  & 15) - 1)/Period);
            Arg *= (Period - Phase);
            floor(Arg);
            Arg /= Period;
            Arg += Phase;
        }
        else
        {
            F2Dot14 Period = (roundState & 192) * (sqrt(2)/2);
            F2Dot14 Phase = Period/(roundState  & 48);
            Arg -= Phase;
            Arg += (F2Dot14)(((roundState  & 15) - 1)/Period);
            Arg *= (Period - Phase);
            floor(Arg);
            Arg /= Period;
            Arg += Phase;
        }

    }

    else if (boolean < 12 )
    {
        Arg += boolean - 8;
    }
    else
    {
        Arg += boolean - 12;
        roundState = 0;
    }

    if (roundState & 128 == 180)
        {

        }

    else if (roundState == 1)       //Round down to Grid
            {
                    ft_floor(Arg);
            }

    else if (roundState == 2)    //Actually have no idea what this is supposed to be
        {
           Arg = PointFiveRound(Arg);
        }

    else if  (roundState == 3)  //Round to Grid
        {
            if (StandardRound(Arg) == 1)
                {
                ft_ceil(Arg);
                }
            else
                {
                ft_floor(Arg);
                }
        }

    else if (roundState == 4)       //Round up to Grid
            {
                    ft_ceil(Arg);
            }

    else if (roundState == 5)   // Round to DG
            {
            if (StandardRound(Arg) == 1)
                {
                Arg = ft_ceil(Arg) + 1;
                }
            else
                {
                Arg = ft_floor(Arg) - 1;
                }
            }
}
