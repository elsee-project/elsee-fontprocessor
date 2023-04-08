#include <Instructions/InstructionGroups/Core.h>
#include <FontData/FontMachineRoutines.h>


uint32_t fpgmentry (uint8_t *fpgm, struct InstructionKit *EngineInit)
{
    instructionentry(fpgm, EngineInit, NULL);
}

uint32_t prepentry (uint8_t *prep, struct InstructionKit *EngineInit)
{
    instructionentry(prep, EngineInit, NULL);
}

uint32_t glyphentry(uint8_t *instructionlist, struct InstructionKit *MemoryLocations, struct glyf *glyf)
{
    // (This should point to area where the glyph should be displayed)
    uint16_t totalpoints;
    for (int i = 0; i < numberOfContours; i++)
    {
        totalpoints = *(glyf -> endPtsOfContours) + i;
    }
    for(int i = 0 ; i < totalpoints; i++)
    {
        F2Dot14 *Points = Points + (long)MemoryLocations -> GlyphZone[0][i] +(long)MemoryLocations -> GlyphZone[1][i] * 2048;  // casts and rounds value
    }
    instructionentry(instructionlist, MemoryLocations, glyf);
}
