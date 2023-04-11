
#ifndef FONTMACHINEROUTINES_H
#define FONTMACHINEROUTINES_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <FontEngineInit.h>
#include <FontData/FontTableSetup.h>

uint8_t *point_8;
uint16_t *point_16;

void PointRound(F2Dot14 Points);



uint8_t SimpleGlyphProcess(struct glyf *glyftable, struct InstructionKit *MemoryLocations, uint32_t *point)
{
    for (int i = 0; i < *(*glyftable).endPtsOfContours; i++)
    {
        if (*(*glyftable).flags & 1 == 1)
        {

        }
        if (*(*glyftable).flags & 2 == 1)
        {
            point_8 = (uint8_t*)(*glyftable).xCoordinates;
            (*MemoryLocations).OriginalPoints[0][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 16 == 0)
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  -((*MemoryLocations).OriginalPoints[0][*point]);
            }
        }
        else
        {
            point_16 = (uint16_t*)(*glyftable).xCoordinates;
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
            point_8 = (uint8_t*)(*glyftable).yCoordinates;
            (*MemoryLocations).OriginalPoints[1][*point] = (float)*point_8 * MemoryLocations -> PixelSize;
            if (*(*glyftable).flags & 16 == 0)
            if (*(*glyftable).flags & 32 == 0)
            {
                (*MemoryLocations).OriginalPoints[0][*point] =  -((*MemoryLocations).OriginalPoints[0][*point]);
            }
        }
        else
        {
            point_16 = (uint16_t*)(*glyftable).yCoordinates;
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

struct glyf *ComplexGlyphProcess(struct glyf *glyftable, struct InstructionKit *MemoryLocations, uint32_t *point, void *AddrSetPoints)
{   
    /*This function, ComplexGlyphProcess, is part of a recursive function which is defined beneath it.
     ComplexGlyphProcess uses the called function as Complex Glyphs are made up of multiple component glyphs 
     which are essentially defined in the same format as simple glyphs unless made up of other component glyphs. 
     As such, each component should be processed as a glyf you'd define through the cmap */
    void (*setPoints)(struct glyf *glyftable,  struct InstructionKit *MemoryLocations) = AddrSetPoints;  // Sets address for calling function
    glyftable -> componentflags;
    uint16_t *Transformation;
    Transformation = glyftable -> transformation;
    while (*glyftable -> componentflags &  32 != 0)
    {
    struct glyf *componentglyf = glyfsetup(componentglyf, MemoryLocations, MemoryLocations -> loca->offsets[(*glyftable).glyphIndex]); // Double Check, it should add the Glyph Entry to the Offset and the Entry
    setPoints(componentglyf,  MemoryLocations); //Calls the calling function
        if (*(*glyftable).flags & 1 == 1)
        {
            if (*(*glyftable).flags &  2 == 2)
            {uint8_t *point_8 = (uint8_t*)(*glyftable).xCoordinates;
            ((*MemoryLocations).GlyphZone[0][*point]) =  (float)*point_8;
            ((*MemoryLocations).GlyphZone[1][*point]) =  (float)*(point_8 +1);
                if (*(*glyftable).flags &  4 == 1)
                {
                    PointRound((*MemoryLocations).GlyphZone[*point][*point]);
                }
            }
        }
        else
        {
            uint16_t *point_16 = (uint16_t*)(*glyftable).xCoordinates;
            (*glyftable).arg2 += 2;
            if (*(*glyftable).flags &  2 == 2)
            {
                ((*MemoryLocations).GlyphZone[0][*point]) =  (float)*(point_16);;
                ((*MemoryLocations).GlyphZone[1][*point]) =  (float)*(point_16 +2);
                if (*(*glyftable).flags &  4 == 1)
                {
                    PointRound((*MemoryLocations).GlyphZone[*point][*point]);
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

void setPoints(struct glyf *glyftable,  struct InstructionKit *MemoryLocations)
{
    bool GlyphsToDo = 1;  //Flag that determines whether there's glyphs to process
    uint32_t *point = (uint32_t*)(*MemoryLocations).OriginalPoints + sizeof((*MemoryLocations).OriginalPoints);
    MemoryLocations -> GlyphSets[*point];
    while (GlyphsToDo != 0) 
    {
        uint8_t *ComponentPointer;
        glyftable->flags;
        int totalpoints; //Remove
        if (numberOfContours >= 0)
        {
            GlyphsToDo = 0;
            SimpleGlyphProcess(glyftable, MemoryLocations, point);
        }
        else
        {
            struct glyf *componentglyf = ComplexGlyphProcess(glyftable, MemoryLocations, point, &setPoints);
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
#endif