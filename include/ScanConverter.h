#include <FontData/FontTableSetup.h>
#include <stdbool.h>
#include <math.h>

uint8_t *ScanGlyf(struct InstructionKit *MemoryLocations, struct glyf *glyf, uint16_t scanType)
{
    uint32_t *point;
    F2Dot14 lastpoint;
    uint32_t PixelSize = MemoryLocations -> PixelSize;
    int i = 0;
    int j = 0;
    uint8_t *Pixels;
    Pixels = (uint8_t*)calloc(sizeof(uint8_t), (glyf -> yMax - glyf -> yMin) * (glyf -> xMax - glyf -> xMin));
    while (point [i] != lastpoint)
    {
        while (MemoryLocations -> GlyphFlags[i] & 1 == 0) //Will skip any off curve points
         {
            j++;
         }
        if (MemoryLocations -> GlyphZone[1][j] <= MemoryLocations -> GlyphZone[1][i] +  PixelSize)
            {   //Will check each point to see if points are more than a pixel in width apart. If not, keep going
                j++;
            }
        else if (MemoryLocations -> GlyphZone[0][j] + (PixelSize) >=  MemoryLocations -> GlyphZone[0][i] )
            {
                F2Dot14 *Bezierlocation = (F2Dot14*)malloc(sizeof(F2Dot14) * j-i);
                Bezierlocation = BezierCalculation(point[i], j-i, MemoryLocations ->GlyphZone);
               /* if (Round(MemoryLocations -> GlyphZone[0][j] && MemoryLocations -> GlyphZone[0][i], 0) == true)
                {

                }
                */
                //if the midpoint between the two rounds to .5, the distance of the line from the pixel rounds to 5.
                // Will need to set threshold
            }
        else
            {
                F2Dot14 *Bezierlocation = (F2Dot14*)malloc(sizeof(F2Dot14)*ceil(MemoryLocations -> GlyphZone[1][i]  - MemoryLocations -> GlyphZone[1][j]));
                Bezierlocation = BezierCalculation(point[i], j-i, MemoryLocations ->GlyphZone);
                for (int l = 0; l < sizeof(Bezierlocation); l++)
                {
                    for (int k = glyf -> yMin ; k < glyf -> yMax; k++)
                    {
                        uint32_t value = ceil(MemoryLocations -> GlyphZone[1][i] + Bezierlocation[l]);
                        Pixels[value][k] = ~(*Pixels[ROUND((char) 0, (uint32_t) 3, MemoryLocations -> GlyphZone[1][i + Bezierlocation])][k]);
                        //If Points  Intersect, turn on.  Still need to manually turn off every point past this point
                    }
                }
                i += j; // Start from j
            }
    }
    for (int j = 0; j < glyf -> yMax; j++)
    {
        for (int i = 0; i < xMax; i++)
        {
            if (Pixels[i][j] != 0)
                {
                    //Pixel = DisplayColor;
                }
            else
                {
                   //Pixel = BackgroundColor;
                }
        }
    }
return Pixels;  // This is the final product; the glyph to be displayed on the screen

}

static F2Dot14 *BezierCalculation(uint32_t PointZero, uint32_t numofPoints, F2Dot14 (*Glyphzone)[2])
{
    uint8_t Value = sizeof(numofPoints);
    uint8_t *binomial = (uint8_t *) calloc(sizeof(Value),Value);
    uint8_t *Pixels = (uint8_t *)malloc(sizeof(ceil(Glyphzone[1][PointZero] - Glyphzone[1][PointZero + numofPoints])));
    int Dec = (Value - 1);
    F2Dot14 Tee;
    F2Dot14 OneMinusTee;
    //int Distance = Distance()/(Glyphzone[1][PointZero] - Glyphzone[1][PointZero + numofPoints]; //Denominator is the number of pixels that has been passed
    //F2Dot14 OneMinusTee = 1 - Distance;
    for (int i = 0; i < sizeof(Pixels); i++)
    {
        Pixels[i] = i * .5;   //Every 5th point of pixel
    }
    int j;
    int k;
    for (int i = 0; i < sizeof(Value/2); i++)
    {
        for (j=i; j> 0 ;j--)
            {
                binomial[j + 1] = binomial[j] + binomial[j+1];
                binomial[Dec - j] = binomial[j+1];
                j = i;
                binomial[j + 1] = binomial[j] + binomial[j+1];
                binomial[Dec - j] = binomial[j+1];
            }
    }
    if (sizeof(Value) & 1 == 1)  //In the case of an odd number, perform the last loop needed
    {
        for (j=sizeof((Value/2) + 1); j> 0 ;j--)
        {
            binomial[j + 1] = binomial[j] + binomial[j+1];
            binomial[Dec - j] = binomial[j+1];
        }
    }
    for (int a = 0; a < sizeof(Pixels); a++)
    {
        for (int i = Dec; i> 1; i++)
        {
            for (int k = i; k > 1; i++)
            {
                binomial[k] *= Tee;
            }
        }

        for (int i = 1; i < Value; i++)
        {
            for (int k = i; k > 0; i++)
            {
                binomial[k] *= (OneMinusTee);
                binomial[k] += binomial[k - 1];
            }
        }
        Pixels[a] *= binomial[k]; //  Final Value should be change in X from the axis of Point 0
    }
    // T will be 1 divided by num of pixels being passed minus a half
    // any pixel at a head and to the side of a certain direction will be turned on
}


static F2Dot14 DropoutControl(struct InstructionKit *MemoryLocations, struct glyf *glyf)
{
    uint32_t *point;
    F2Dot14 lastpoint;
    int i = 0;
    int j = 0;
    uint8_t *Pixels;
    Pixels = (uint8_t*)calloc(sizeof(uint8_t), (glyf -> yMax - glyf -> yMin) * (glyf -> xMax - glyf -> xMin));
    while (point [i] != lastpoint)
    {
    F2Dot14 *Bezierlocation = (F2Dot14*)malloc(sizeof(F2Dot14) * ceil(MemoryLocations -> GlyphZone[1][i]  - MemoryLocations -> GlyphZone[1][j]));
    Bezierlocation = BezierCalculation(point[i], j-i, MemoryLocations ->GlyphZone);
                for (int l = 0; l < sizeof(Bezierlocation); l++)
                {
                    for ( int k = 0; k < (*glyf -> endPtsOfContours + 1); k++)
                    {
                    if(MemoryLocations -> GlyphZone[1][glyf -> endPtsOfContours + k]  < Pixels[1][i] || Glyphzone[1][glyf -> endPtsOfContours  + k]  < Pixels[1][j])
                    {
                    *Pixels[floor(MemoryLocations -> GlyphZone[0][i])][ceil(MemoryLocations -> GlyphZone[1][i+  Bezierlocation[l]]), 3] = DisplayColor;
                    // All pixels to the horizontally opposite directions of the points shall be flipped to their opposite flags.
                    // All points are rounded to nearest whole pixel
                    }
                    }
                //if the midpoint between the two rounds to .5, the distance of the line from the pixel rounds to 5.
                // Will need to set threshold
                }

    }
}
