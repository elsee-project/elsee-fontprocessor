#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <FontEngineInit.h>

uint8_t *point_8;
uint16_t *point_16;

uint8_t SimpleGlyphProcess(struct glyf *glyftable, struct InstructionKit *MemoryLocations, uint32_t *point)
{
    for (int i = 0; i < *(*glyftable).endPtsOfContours; i++)
    {
        if (*(*glyftable).flags & 1 == 1)
        {

        }
        if (*(*glyftable).flags & 2 == 1)
        {
            *point_8 = (uint8_t*)*(*glyftable).xCoordinates;
            (*MemoryLocations).OriginalPoints[0][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 16 == 0)
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  -((*MemoryLocations).OriginalPoints[0][*point]);
            }
        }
        else
        {
            *point_16 = (uint16_t*)*(*glyftable).xCoordinates;
            (*MemoryLocations).OriginalPoints[0][*point] = (float)*point_16 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 16 == 1)
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  (*MemoryLocations).OriginalPoints[0][i -1];
            }
            else
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  (*MemoryLocations).OriginalPoints[0][*point] + (*MemoryLocations).OriginalPoints[0][i -1];
            }
        }
        if (*(*glyftable).flags & 4 == 0)
        {
            *point_8 = (uint8_t*)*(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 16 == 0)
            if (*(*glyftable).flags & 32 == 0)
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  -((*MemoryLocations).OriginalPoints[0][*point]);
            }
        }
        else
        {
            *point_16 = (uint16_t*)*(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_16 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 32 == 1)
            {
                (*MemoryLocations).OriginalPoints[1][*point] =  (*MemoryLocations).OriginalPoints[1][i -1];
            }
            else
            {
                (*MemoryLocations).OriginalPoints[1][*point] =  (*MemoryLocations).OriginalPoints[1][*point] + (*MemoryLocations).OriginalPoints[1][i -1];
            }
        }
        MemoryLocations -> GlyphFlags[i];
        if (*(*glyftable).flags & 8 == 0)
        {
            (*glyftable).flags++;  //Flag values should only be moving if this is set to 0, positive value means this flag shall be repeated
        }
        *point++;
    }
}

struct glyf *ComplexGlyphProcess(struct glyf *glyftable, struct InstructionKit *MemoryLocations, uint32_t *point)
{
    glyftable -> componentflags;
    uint16_t *Transformation;
    Transformation = glyftable -> transformation;
    while (*glyftable -> componentflags &  32 != 0)
    {
    struct glyf *componentglyf = glyfsetup(componentglyf, MemoryLocations, MemoryLocations -> loca->offsets[(*glyftable).glyphIndex]); // Double Check, it should add the Glyph Entry to the Offset and the Entry
    setPoints(componentglyf,  MemoryLocations);
        if (*(*glyftable).flags & 1 == 1)
        {
            if (*(*glyftable).flags &  2 == 2)
            {uint8_t *point_8 = (uint8_t*)*(*glyftable).xCoordinates;
            ((*MemoryLocations).GlyphZone[0][*point]) =  (float)*point_8;
            ((*MemoryLocations).GlyphZone[1][*point]) =  (float)*(point_8 +1);
                if (*(*glyftable).flags &  4 == 1)
                {
                    round((*MemoryLocations).GlyphZone[*point][*point]);
                }
            }
        }
        else
        {
            uint16_t *point_16 = (uint16_t*)*(*glyftable).xCoordinates;
            (*glyftable).arg2 += 2;
            if (*(*glyftable).flags &  2 == 2)
            {
                ((*MemoryLocations).GlyphZone[0][*point]) =  (float)*(point_16);;
                ((*MemoryLocations).GlyphZone[1][*point]) =  (float)*(point_16 +2);
                if (*(*glyftable).flags &  4 == 1)
                {
                    round((*MemoryLocations).GlyphZone[*point][*point]);
                }
            }
        }

        if (*(*glyftable).flags & 8 == 0)
        {
            Transformation[0] = 1;
            Transformation[4] = 1;
        }
        if (*(*glyftable).flags & 32 == 1)
        {
            (*MemoryLocations).SetIndex++;
        }
        if (*(*glyftable).flags & 64 == 0)
        {
            Transformation[0] = *glyftable -> transformation;
            Transformation[4] = Transformation[0];
        }
        else
        {
            Transformation[0] = *(*glyftable).transformation;
            Transformation[4] = *(*glyftable).transformation + 3;
        }
        if (*(*glyftable).flags & 128 == 0)
        {
            Transformation[2] = *(*glyftable).transformation + 1;
            Transformation[3] = *(*glyftable).transformation + 2;
        }
        if (*(*glyftable).flags & 256 == 0)
        {
            *point_8 = (uint8_t)*(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
        }
        if (*(*glyftable).flags & 512 == 0)
        {
            *point_8 = (uint8_t)*(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
        }
        if (*(*glyftable).flags & 1024 == 0)
        {
            *point_8 = (uint8_t)*(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
        }

        }
}


uint16_t *searchglyph(uint32_t *character, struct cmap *cmap, struct InstructionKit *EngineInit)
{
    int i = 0;
    uint16_t glyphindex;
    if (*cmap -> format == 6)
    {
        uint16_t *segcount = (uint16_t*)(*cmap).format + 3;
        uint16_t *endCode =  (uint16_t*)(*cmap).format + 8;
        uint16_t *startCode = endCode + *segcount + 1;
        uint16_t *idDelta = startCode + *segcount;
        uint16_t *idRangeOffset = idDelta + *segcount;
        while((startCode[i] < *character && endCode[i] > *character ) || endCode[i] == 65355)   //0xFFFFh is the last value in the search and maps to null
        {
            i++;
        }
        if (endCode[i] != 65355)
            {
            glyphindex = *(&idRangeOffset[i] + (idRangeOffset[i]/2) + (*character -startCode[i]));
            }
            glyphindex = idDelta[i] + glyphindex;

        if(*character == 255)
            {
            segcount = 0;
            endCode = 0;
            startCode = 0;
            idRangeOffset = 0;
            glyphindex = 0;
            free(segcount);
            free(endCode);
            free(startCode);
            free(idRangeOffset);
            }
    }
    return (uint16_t*)EngineInit -> GlyphEntry + EngineInit-> loca->offsets[glyphindex];
}

void setPoints(struct glyf *glyftable,  struct InstructionKit *MemoryLocations)
{
    uint8_t MoreGlyphs = 1;  //Boolean
    uint32_t *point = (uint32_t*)(*MemoryLocations).OriginalPoints + sizeof((*MemoryLocations).OriginalPoints);
    MemoryLocations -> GlyphSets[*point];
    while (MoreGlyphs != 0)
    {
        uint8_t *ComponentPointer;
        glyftable->flags;
        int totalpoints; //Remove
        if (numberOfContours  <= 0)
        {
            MoreGlyphs = 0;
            SimpleGlyphProcess(glyftable, MemoryLocations, point);
        }
        else
        {
            struct glyf *componentglyf = ComplexGlyphProcess(glyftable, MemoryLocations, point);
            //This will add the simple glyph to the list of instructions ot execute
            setPoints(componentglyf,  MemoryLocations);
        }
    }
    for(int i = 0 ; i < *point; i++)
    {
        (*MemoryLocations).GlyphZone[*point - i ][*point - i] = (*MemoryLocations).OriginalPoints[i][i];
        (*MemoryLocations).GlyphZone[i][i] = (*MemoryLocations).OriginalPoints[i][i];
    }
    /*
    if Rotate == true, change or
    Rotate is false by default
    Rotate 1 is 90 Rotate 2 is 180 Rotate 3 is 270
    if Stretched, signed. if zero, no stretch, if positive, the number is the magnittude of strentching in terms of x value
    if y the magnitude is in terms of y value




    */

}
