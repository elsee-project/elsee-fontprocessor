#include  "Core.h"

static inline uint32_t IUP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
        uint32_t TouchedPointA = 0;
        uint32_t TouchedPointB = 0;
        F2Dot14 *TouchedA;
        F2Dot14 *TouchedB;
        if (*instructionpointer == 30)
        {
            int i = 0;
            while (i < sizeof(MemoryLocations -> TouchedPoints))
            {
                if (TouchedPointA != 1 && Point(MemoryLocations, MemoryLocations -> OriginalPoints[i][1], RuntimeStates -> zp[2]))
                    {
                        TouchedPointA = i;
                        i++;
                        for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                        {
                            F2Dot14 ShortestDis; //(Should be set to Distance between Touched end of grid );
                            F2Dot14 Line = Distance(Point(MemoryLocations, TouchedPointA, RuntimeStates -> zp[2]), Point(MemoryLocations, TouchedPointB, RuntimeStates -> zp[2]));
                            if (MemoryLocations -> OriginalPoints[i][1] != 0 && MemoryLocations -> OriginalPoints[i][1] > ShortestDis && i != TouchedPointA)    //Untouched points has the color of the area of the display it is pointing to which should be white
                            {
                                TouchedB = Point(MemoryLocations, i , RuntimeStates -> zp[2]);
                            }
                            if (TouchedPointA == TouchedPointB)
                            {

                                for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                                {
                                    if (ShortestDis > Line && Line != 0)
                                    {
                                        ShortestDis = Line;
                                    }
                                }
                            }
                        }
                    }
            }
        }


        else if (*instructionpointer == 30)
        {
            int i = 0;
            while (i < sizeof(MemoryLocations -> TouchedPoints))
            {
                if (TouchedPointA != 1 && Point(MemoryLocations, MemoryLocations -> OriginalPoints[i][1], RuntimeStates -> zp[2]))
                    {
                        TouchedPointA = i;
                        i++;
                        for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                        {
                            F2Dot14 ShortestDis; //(Should be set to Distance between Touched end of grid );
                            F2Dot14 Line = Distance(Point(MemoryLocations, TouchedPointA, RuntimeStates -> zp[2]), Point(MemoryLocations, TouchedPointB, RuntimeStates -> zp[2]));
                            if (MemoryLocations -> OriginalPoints[i][1] != 0 && MemoryLocations -> OriginalPoints[i][1] > ShortestDis && i != TouchedPointA)    //Untouched points has the color of the area of the display it is pointing to which should be white
                            {
                                TouchedB = Point(MemoryLocations, i , RuntimeStates -> zp[2]);
                            }
                            if (TouchedPointA == TouchedPointB)
                            {

                                for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                                {
                                    if (ShortestDis > Line && Line != 0)
                                    {
                                        ShortestDis = Line;
                                    }
                                }
                            }
                        }
                    }
            }
        }


    else
        {
            int i = 0;
            while (i < sizeof(MemoryLocations -> TouchedPoints))
            {
                if (TouchedPointA != 1 && Point(MemoryLocations, MemoryLocations -> OriginalPoints[i][1], RuntimeStates -> zp[2]))
                    {
                        TouchedPointA = i;
                        i++;
                        for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                        {
                            F2Dot14 ShortestDis; //(Should be set to Distance between Touched end of grid );
                            F2Dot14 Line = Distance(Point(MemoryLocations, TouchedPointA, RuntimeStates -> zp[2]), Point(MemoryLocations, TouchedPointB, RuntimeStates -> zp[2]));
                            if (MemoryLocations -> OriginalPoints[i][1] != 0 && MemoryLocations -> OriginalPoints[i][1] > ShortestDis && i != TouchedPointA)    //Untouched points has the color of the area of the display it is pointing to which should be white
                            {
                                TouchedB = Point(MemoryLocations, i , RuntimeStates -> zp[2]);
                            }
                            if (TouchedPointA == TouchedPointB)
                            {

                                for (int k = 0; k < sizeof(MemoryLocations -> TouchedPoints); k++)
                                {
                                    if (ShortestDis > Line && Line != 0)
                                    {
                                        ShortestDis = Line;
                                    }
                                }
                            }
                        }
                    }
            }
        }


     TouchedB[0] = 0;
     TouchedB[1] = 0;

}

static inline uint32_t IP(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {   uint32_t *zp = RuntimeStates -> zp;
        uint32_t *rp = RuntimeStates -> rp;
        F2Dot14 OrigDisRPX = ((MemoryLocations -> OriginalPoints[0][rp[1]]) - (MemoryLocations -> OriginalPoints[0][rp[2]]));
        F2Dot14  OrigDisRPY = ((MemoryLocations -> OriginalPoints[1][rp[1]]) - (MemoryLocations -> OriginalPoints[1][rp[2]]));
        F2Dot14 OrigDisP[2];
        OrigDisP[0] = ((MemoryLocations -> OriginalPoints[0][rp[1]]) - (MemoryLocations -> OriginalPoints[0][*MemoryLocations -> StackPointer]));
        POP(*MemoryLocations -> StackPointer);
        OrigDisP[1] = ((MemoryLocations -> OriginalPoints[1][rp[1]]) - (MemoryLocations -> OriginalPoints[1][*MemoryLocations -> StackPointer]));
        POP(*MemoryLocations -> StackPointer);
        uint32_t RelDisRP [2] = {(*Point(MemoryLocations,rp[1], zp[0]) - *Point(MemoryLocations,rp[2], zp[1])), (*Point(MemoryLocations,rp[1], zp[0]) - *Point(MemoryLocations,rp[2], zp[1]))};
        uint32_t RatioRP = RelDisRP[0]/OrigDisRPX;
        F2Dot14 *dummy;
        dummy = Point(MemoryLocations, *MemoryLocations -> StackPointer, zp[2]);
        *dummy += *(Point(MemoryLocations, rp[1], zp[0]));
        *dummy += (OrigDisP[0] * RelDisRP[0]);
        *(dummy + 1) += (OrigDisP[1] * RelDisRP[1]);
    }

static inline F2Dot14 SHP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
      MemoryLocations -> StackPointer  -= 3;
      RuntimeStates -> rp[*MemoryLocations -> StackPointer];
    }

static inline F2Dot14 SHC(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
        if (*instructionpointer % 10 == 3)
        {
            F2Dot14 *point = Point(MemoryLocations,  RuntimeStates -> rp[1],  RuntimeStates -> zp[0]);
        }
        else
        {
            F2Dot14 *point = Point(MemoryLocations,  RuntimeStates -> rp[0],  RuntimeStates -> zp[1]);
        }
    }

 static inline F2Dot14 SHZ(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
        uint32_t point = RuntimeStates ->  rp[1];
        MemoryLocations -> StackPointer -= 3;
        if (*instructionpointer % 10 == 7)
            {

                if (RuntimeStates -> zp[1] == 1)
                {
                    for (int i = 0; i < sizeof(MemoryLocations -> GlyphZone); i++)
                    {
                    MemoryLocations -> TwilightZone[i][0] += MemoryLocations -> GlyphZone[point][0]- MemoryLocations -> OriginalPoints[point][0];
                    MemoryLocations -> TwilightZone[i][1] += MemoryLocations -> GlyphZone[point][1]- MemoryLocations -> OriginalPoints[1][point];
                    }
                }
                else
                {
                   for (int i = 0; i < sizeof(MemoryLocations -> GlyphZone); i++)
                    {
                    MemoryLocations -> GlyphZone[i][0] += MemoryLocations ->  TwilightZone[point][0] - MemoryLocations -> OriginalPoints[point][0];
                    MemoryLocations -> GlyphZone[i][0] += MemoryLocations ->  TwilightZone[point][1] - MemoryLocations -> OriginalPoints[point][1];
                    }
                }
            }
            else if (RuntimeStates -> zp[0] == 1)
            {
                for (int i = 0; i < sizeof(MemoryLocations -> GlyphZone); i++)
                {
                MemoryLocations -> TwilightZone[i][0] += MemoryLocations -> GlyphZone[point][0]- MemoryLocations -> OriginalPoints[point][0];
                MemoryLocations -> TwilightZone[i][0] += MemoryLocations -> GlyphZone[point][1]- MemoryLocations -> OriginalPoints[point][1];
                }
            }
            else
            {
                for (int i = 0; i < sizeof(MemoryLocations -> GlyphZone); i++)
                {
                MemoryLocations -> GlyphZone[i][0] += MemoryLocations ->  TwilightZone[point][0] - MemoryLocations -> OriginalPoints[point][0];
                MemoryLocations -> GlyphZone[i][0] += MemoryLocations ->  TwilightZone[point][1] - MemoryLocations -> OriginalPoints[point][1];
                }
            }
    }


static inline F2Dot14 SHPIX(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
    {
        F2Dot14 *p1 = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[2]);
        F2Dot14 *p2 = Point(MemoryLocations, *(MemoryLocations -> StackPointer - 1) , RuntimeStates -> zp[2]);
        F2Dot14 FreedomValues[2];
        FreedomValues[0] = FreeTransform(RuntimeStates -> FreedomVector, *(MemoryLocations -> StackPointer - 2), 0);
        FreedomValues[1] = FreeTransform(RuntimeStates -> FreedomVector, *(MemoryLocations -> StackPointer - 1), 0);
        p1[0] += FreedomValues[0];
        p2[0] += FreedomValues[0];
        p1[1] += FreedomValues[1];
        p1[1] += FreedomValues[1];
        POP(MemoryLocations -> StackPointer);
        POP(MemoryLocations -> StackPointer);
        POP(MemoryLocations -> StackPointer);
        free(p1);
        free(p2);
        free(FreedomValues);
    }
static inline F2Dot14 MSIRP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
  F2Dot14 distance;
  if (*instructionpointer & 1 == 1)
  {
      RuntimeStates -> rp[0] = *(MemoryLocations -> StackPointer);
  }
  POP(MemoryLocations -> StackPointer);
}

static inline uint32_t ALIGNRP(struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
        F2Dot14 *Coord1 = Point(MemoryLocations, RuntimeStates -> rp[0], RuntimeStates -> zp[0]);
        F2Dot14 *Coord2 = Point(MemoryLocations, *MemoryLocations -> StackPointer, RuntimeStates -> zp[1]);
        ProjectionDistance(RuntimeStates -> ProjectionVector, Coord1);
        ProjectionDistance(RuntimeStates -> ProjectionVector, Coord2);
        F2Dot14 distance = Distance(Coord1, Coord2);   //x2 *x1 + y2 +y1
    if (*(MemoryLocations -> StackPointer) > *(MemoryLocations -> StackPointer - 1))
    {
        FreedomTransform(MemoryLocations -> StackPointer, RuntimeStates -> FreedomVector, distance/2);
        FreedomTransform((MemoryLocations -> StackPointer - 1), RuntimeStates -> FreedomVector, -distance/2);
    }
    else
    {
        FreedomTransform(MemoryLocations -> StackPointer, RuntimeStates -> FreedomVector, -distance/2);
        FreedomTransform((MemoryLocations -> StackPointer - 1), RuntimeStates -> FreedomVector, distance/2);
    }
    POP(MemoryLocations -> StackPointer);
}


static inline uint32_t SRP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations,  struct GraphicStates *RuntimeStates)
    {
        RuntimeStates -> rp[*MemoryLocations ->StackPointer] = (*instructionpointer - 10);
        POP(MemoryLocations -> StackPointer);
    }

static inline F2Dot14 MIAP(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
      F2Dot14 *Coordinates;
      Coordinates = Point( MemoryLocations, *(MemoryLocations -> StackPointer + 1), RuntimeStates -> zp[0]);
      *(MemoryLocations -> StackPointer + 2) = MemoryLocations -> controlvaluetable[(*MemoryLocations -> StackPointer + 2)];
      MemoryLocations -> StackPointer -= 2;
      if (*instructionpointer == 62)
      {
            if(*(MemoryLocations -> StackPointer + 2) - *(MemoryLocations -> StackPointer + 1) >  RuntimeStates -> controlvalue_cut_in)
                {
                  ROUND((char) NULL, RuntimeStates -> roundState, Coordinates[0]);
                  ROUND((char) NULL, RuntimeStates -> roundState, Coordinates[1]);
                  Coordinates = MoveRelProjFree(Coordinates, RuntimeStates, *(MemoryLocations -> StackPointer + 2));

                }
            else
                {
                    ROUND((char) NULL, RuntimeStates -> roundState, (F2Dot14) *(MemoryLocations -> StackPointer + 2));
                    Coordinates = MoveRelProjFree(Coordinates, RuntimeStates , *(MemoryLocations -> StackPointer + 2));
                }
      }
      else
      {
          Coordinates = MoveRelProjFree(Coordinates, RuntimeStates ,*(MemoryLocations -> StackPointer + 2));
      }

}

static inline F2Dot14 RTDG(uint32_t roundState)
    {
       roundState = 2;
       return roundState;
    }


static inline void FunctionGroupThree(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
uint8_t LowEnd  = *instructionpointer & 15;
uint32_t *StackPointer = MemoryLocations -> StackPointer;
    if(LowEnd < 8)
    {
            if (LowEnd < 4)
            {
                if (LowEnd < 2)
                {
                    IUP(instructionpointer, MemoryLocations, RuntimeStates);
                }
                else
                {
                    SHP(instructionpointer, MemoryLocations, RuntimeStates);
                }
            }
            else if (LowEnd < 6)
            {
                SHC(instructionpointer, MemoryLocations, RuntimeStates);
            }
                else
            {
                SHZ(instructionpointer, MemoryLocations, RuntimeStates);
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
                            SHPIX(MemoryLocations, RuntimeStates);
                        }
                case 9 :
                        {
                            IP(MemoryLocations, RuntimeStates);
                        }
            }
        }
        else
        {
            MSIRP(instructionpointer, MemoryLocations, RuntimeStates);
        }
    }
    else if (LowEnd < 14)
    {
        switch (LowEnd)
            {
            case 12 :
                    {
                        ALIGNRP(MemoryLocations, RuntimeStates);
                    }
            case 13 :
                    {
                        RuntimeStates -> roundState = RTDG (RuntimeStates -> roundState);
                    }
            }
    }
    else
    {
        MIAP(instructionpointer, MemoryLocations, RuntimeStates);
    }
}
