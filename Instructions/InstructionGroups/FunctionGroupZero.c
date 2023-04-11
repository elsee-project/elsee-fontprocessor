#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "Core.h"

F2Dot14 Coordinates[2];

static inline F2Dot14 SVTCA(uint8_t *instructionpointer,  F2Dot14 *FreedomVector,   F2Dot14 *ProjectionVector)
{
  if (*instructionpointer == 0)
  {
    ProjectionVector[0] = 1;
    ProjectionVector[1] = 0;
  }
  else
  {
    ProjectionVector[0] = 0;
    ProjectionVector[1] = 1;
  }
  FreedomVector[0] = ProjectionVector[0];
  FreedomVector[1] = ProjectionVector[1];

}
static inline F2Dot14 *SVFTCA(uint8_t *instructionpointer, F2Dot14 *FreedomVector)
    {
       if (*instructionpointer == 0)
       {
        FreedomVector[0] = 1;
        FreedomVector[1] = 0;
       }
       else
       {
        FreedomVector[0] = 0;
        FreedomVector[1] = 1;
       }
       return FreedomVector;
    }

static inline F2Dot14 *SVPTCA(uint8_t *instructionpointer, F2Dot14 *ProjectionVector)
    {
       if (*instructionpointer == 0)
       {
        ProjectionVector[0] = 1;
        ProjectionVector[1] = 0;
       }
       else
       {
        ProjectionVector[0] = 0;
        ProjectionVector[1] = 1;
       }
       return ProjectionVector;
    }

static inline F2Dot14 SPVTL(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations , struct GraphicStates *RuntimeStates)
{
    F2Dot14 *PointA;
    PointA = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[1]);
    POP(MemoryLocations -> StackPointer);
    F2Dot14 *PointB = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[2]);
    POP(MemoryLocations -> StackPointer);
    F2Dot14 Magnitude = Distance(PointA, PointB);
    if (*instructionpointer == 6)
    {
        RuntimeStates -> ProjectionVector[0]  = (PointA[0] - PointB[0])/Magnitude;
        RuntimeStates -> ProjectionVector[1]  = (PointA[1] - PointB[1])/Magnitude;
    }
    else
    {
        RuntimeStates -> ProjectionVector[1]  = -(PointA[0] - PointB[0])/Magnitude;
        RuntimeStates -> ProjectionVector[0]  = (PointA[1] - PointB[1])/Magnitude;
    }
        RuntimeStates -> ProjectionVector[0] = 1;
        RuntimeStates -> ProjectionVector[1] = 0;
}

static inline F2Dot14 *SFVTL(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations , struct GraphicStates *RuntimeStates)
    {
     F2Dot14 *PointA = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[1]);
       POP(MemoryLocations -> StackPointer);
       F2Dot14 *PointB = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[2]);
       POP(MemoryLocations -> StackPointer);
       F2Dot14 Magnitude = Distance(PointA, PointB);
      if (*instructionpointer == 6)
      {
        RuntimeStates -> FreedomVector[0]  = (PointA[0] - PointB[0])/Magnitude;
        RuntimeStates -> FreedomVector[1]  = (PointA[1] - PointB[1])/Magnitude;
      }
      else
      {
        RuntimeStates -> FreedomVector[1]  = -(PointA[0] - PointB[0])/Magnitude;
        RuntimeStates -> FreedomVector[0]  = (PointA[1] - PointB[1])/Magnitude;
      }
    }


static inline F2Dot14 *SPVFS(F2Dot14 *ProjectionVector, uint32_t *StackPointer)
{
    ProjectionVector[2] = *StackPointer;
    POP(StackPointer);
    ProjectionVector[1] = *StackPointer;
    POP(StackPointer);
    return ProjectionVector;
}

static inline F2Dot14 *SFVFS(F2Dot14 *FreedomVector, uint32_t *StackPointer)

    {

      FreedomVector[0] = *StackPointer;
      POP(StackPointer);
      FreedomVector[0] = *StackPointer;
      POP(StackPointer);
      return FreedomVector;
    }


static inline void *SFVTPV(F2Dot14 *FreedomVector, F2Dot14 *ProjectionVector)

    {
      FreedomVector[0] = ProjectionVector[0];
      FreedomVector[1] = ProjectionVector[1];
    }



static inline void GPV(F2Dot14 *ProjectionVector, uint32_t *StackPointer)
{
    StackPointer++;
    *StackPointer = ProjectionVector[0];
    StackPointer++;
    *StackPointer = ProjectionVector[1];
}

static inline void GFV(F2Dot14 *FreedomVector, uint32_t *StackPointer)
{
    StackPointer++;
    *StackPointer = FreedomVector[0];
    StackPointer++;
    *StackPointer = FreedomVector[1];
}

static inline F2Dot14 ISECT(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    MemoryLocations -> StackPointer -= 5;
    F2Dot14 MidPointA = (Point(MemoryLocations, *(MemoryLocations ->StackPointer + 4), RuntimeStates -> zp[0]))  - Point(MemoryLocations, *(MemoryLocations ->StackPointer + 3), RuntimeStates -> zp[0]);
    F2Dot14 MidPointB = (Point(MemoryLocations, *(MemoryLocations ->StackPointer + 2), RuntimeStates -> zp[1]))  - Point(MemoryLocations, *(MemoryLocations ->StackPointer + 1), RuntimeStates -> zp[1]);
    F2Dot14 *Coordinates;
    Coordinates = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[2]);
    Coordinates[0] = MidPointA;
    Coordinates[1] = MidPointB;
    }



// Totaly distance is Proj Vector times Coordinate divided by mag of coordinate, M
//Mutliply  Magnitude of Displacement by x and y





static inline F2Dot14 MIAP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    F2Dot14 *Coordinates;
    Coordinates = Point(MemoryLocations, *(MemoryLocations -> StackPointer + 1), RuntimeStates -> zp[0]);
    *(MemoryLocations -> StackPointer + 2) = MemoryLocations -> controlvaluetable[*(MemoryLocations -> StackPointer + 2)];
    MemoryLocations -> StackPointer -= 2;
    if (*instructionpointer == 62)
    {
        if(*(MemoryLocations -> StackPointer + 2) - *(MemoryLocations -> StackPointer + 1) > RuntimeStates -> controlvalue_cut_in)
        {
            ROUND(-127, RuntimeStates -> roundState, *Coordinates);
            ROUND(-127, RuntimeStates -> roundState, *(Coordinates + 1));
            Coordinates = MoveRelProjFree(Coordinates, RuntimeStates, *MemoryLocations -> StackPointer);
        }
        else
        {
            ROUND(0, RuntimeStates -> roundState, (F2Dot14)(*MemoryLocations -> StackPointer + 2));
            Coordinates = MoveRelProjFree(Coordinates, RuntimeStates, *MemoryLocations -> StackPointer);
        }
    }
    else
    {
        Coordinates = MoveRelProjFree(Coordinates, RuntimeStates, *MemoryLocations -> StackPointer);
    }
}

static inline void FunctionGroupZero(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
F2Dot14 *FreedomVector =  RuntimeStates -> FreedomVector;
F2Dot14 *ProjectionVector =  RuntimeStates -> ProjectionVector;
uint32_t *StackPointer = MemoryLocations -> StackPointer;
uint8_t LowEnd  = *instructionpointer & 15;

  if(LowEnd < 8)
  {
      if (LowEnd <4)
      {
        if (LowEnd < 2)
        {
          SVTCA(instructionpointer,  FreedomVector,  ProjectionVector);
        }
        else
        {
          ProjectionVector = SVPTCA(instructionpointer,  ProjectionVector);
        }
      }
      else if (LowEnd < 6)
      {
        FreedomVector = SVFTCA(instructionpointer,  FreedomVector);
      }
      else
      {
          SPVTL(instructionpointer, MemoryLocations, RuntimeStates);
      }
  }
  else if (LowEnd < 12)
  {
    if (LowEnd < 10)
    {
      FreedomVector = SFVFS(FreedomVector,  StackPointer);
    }
    else
    {
      ProjectionVector = SPVFS(ProjectionVector,  StackPointer);
      }
  }
  else if (LowEnd < 14)

  {   switch (LowEnd)
    {
          case 12 :
                  {
                  GPV(ProjectionVector, StackPointer);
                  }

          case 13 :
          {
                  GFV(FreedomVector, StackPointer);
          }
    }
  }
  else
  {
      switch (LowEnd)
      {
          case 14 :
                  {
                      SFVTPV(FreedomVector, ProjectionVector);
                  }
          case 15 :
                  {
                    ISECT(MemoryLocations, RuntimeStates);
                  }
      }
  }
}
