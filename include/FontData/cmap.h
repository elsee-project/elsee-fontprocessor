#ifndef CMAP_H
#define CMAP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


struct cmapformat4
{
    uint16_t format;
    uint16_t length;
    uint16_t language;
    uint16_t segCountX2;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
    uint16_t* endCode;
    uint16_t reservedPad;
    uint16_t* startCode;
    uint16_t* idDelta;
    uint16_t* idRangeOffset;
    uint8_t *glyphindexArray;  // Needs caloc funciton that points to data structure
};

struct cmapformat6
 {
    // Needs caloc funciton that points to data structure
    uint16_t format;
    uint16_t length;
    uint16_t language;
    uint16_t firstCode;
    uint16_t entryCount;
    uint8_t *glyphindexArray;
};

#endif
