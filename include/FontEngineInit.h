#ifndef FONTENGINEINIT_H
#define FONTENGINEINIT_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <FontData/TrueFontTypes.h>

struct InstructionKit
{
    uint8_t    *instructionpointer;
    F2Dot14  (*OriginalPoints)[2];
    F2Dot14 (*GlyphZone)[2];
    F2Dot14 (*TwilightZone)[2];
    uint32_t (*TouchedPoints)[2];
    uint32_t *Storage;
    uint32_t *Stack;
    uint32_t *StackPointer;
    F2Dot14 PixelSize;
    F2Dot14 *MasterGrid;
    uint32_t PointSize;
    uint16_t *InstructionDefs;
    uint16_t *FunctionDefs;
    struct glyf *GlyphSets[1];
    uint8_t Rotation;
    int8_t Stretch;
    uint8_t SetIndex;
    uint16_t *GlyphEntry;
    uint16_t GlyphCharacterOffsets[255];
    uint8_t  *GlyphFlags;
    uint16_t *ComponentFlags;
    struct loca *loca;
    FWord *controlvaluetable;
    struct hmtx *htmx;
};

struct InstructionKit *InitializeMemoryLocations(struct maxp *maxp, struct head *head, uint32_t PointSize, uint32_t dpi)
{
    struct InstructionKit *instance;
    (*instance).PointSize = PointSize;
    (*instance).PixelSize = (F2Dot14)(PointSize * dpi)/(72* head->unitsPerEm);
    //*(*instance).MasterGrid = (F2Dot14)(PointSize * dpi)/(72 * head->unitsPerEm);
    (*instance).Stack = (uint32_t*)calloc((uint16_t)*(maxp->Stack), sizeof(uint16_t));
    (*instance).StackPointer = (*instance).Stack;
    (*instance).Storage =  (uint32_t*)calloc((uint16_t)(maxp->maxStorage), sizeof(uint16_t));
    (*instance).OriginalPoints = (F2Dot14(*)[2])calloc(sizeof(F2Dot14(*)[2]), (uint16_t)*(maxp ->maxComponentPoints));
    (*instance).GlyphZone = (F2Dot14(*)[2])calloc(sizeof(F2Dot14(*)[2]), (uint16_t)*(maxp ->maxComponentPoints));
    (*instance).TwilightZone =  (F2Dot14(*)[2])calloc(sizeof(F2Dot14(*)[2]), (uint16_t)*(maxp -> maxTwilightPoints));
    (*instance).TouchedPoints = (uint32_t(*)[2])calloc(sizeof(F2Dot14(*)[2]), (uint16_t)*(maxp ->maxComponentPoints));
    (*instance).FunctionDefs =  (uint16_t*)calloc((maxp -> maxFunctionDefs), sizeof(uint32_t));
    *(*instance).GlyphSets = (struct glyf*)calloc((maxp -> maxComponentElements), sizeof(struct glyf));
    (*instance).SetIndex = 0;
    (*instance).InstructionDefs = (uint16_t*)calloc(((maxp -> maxFunctionDefs)), sizeof(uint16_t));
    (*instance).instructionpointer = (uint8_t*)calloc((maxp -> maxSizeOfInstructions), sizeof(uint8_t));
    (*instance). GlyphFlags = (uint8_t*)calloc(*maxp -> maxComponentPoints, sizeof(uint8_t));
    (*instance). ComponentFlags = (uint16_t*)calloc(maxp -> maxComponentElements, sizeof(uint16_t));
    (*instance).GlyphEntry;
    (*instance). GlyphCharacterOffsets[255];
    (*instance).controlvaluetable;
    (*instance).loca -> offsets = (uint16_t *)calloc(sizeof(uint16_t), (*maxp).numGlyphs * ((*head).indexToLocFormat*2) + 1 );
    (*instance).htmx;
    return instance;
}
#endif