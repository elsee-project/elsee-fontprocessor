#ifndef FONTTABLESETUP_H
#define FONTTABLESETUP_H


#include <FontData/cmap.h>

uint16_t searchglyph(uint32_t *character, struct cmap *cmap, struct InstructionKit *EngineInit)
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
        while((startCode[i] < *character && endCode[i] > *character ) || endCode[i] == 65535)   //0xFFFFh is the last value in the search and maps to null
        {
            i++;
        }
        if (endCode[i] != 65535)
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
    return (uint16_t)*EngineInit -> GlyphEntry + EngineInit-> loca->offsets[glyphindex];
}

uint16_t *cmapformatsetup(uint8_t *instructionpointer, struct cmap *cmap)
{
    u_int16_t *instance;
    if (*(*cmap).format == 4)
    {
        struct cmapformat4 *formatinstance;
        (*formatinstance).format = (uint16_t)*instructionpointer;
        (*formatinstance).language = (uint16_t)*(instructionpointer + 2);
        (*formatinstance).segCountX2 = (uint16_t)*(instructionpointer + 4);   // For now, I will store segCountX2 instead of simply storing just SegCount
        (*formatinstance).searchRange = (uint16_t)*(instructionpointer + 6);
        (*formatinstance).entrySelector = (uint16_t)*(instructionpointer + 8);
        (*formatinstance).rangeShift = (uint16_t)*(instructionpointer + 10);
        (*formatinstance).endCode = (uint16_t*)(instructionpointer + 12);
        (*formatinstance).endCode = (uint16_t*)calloc(sizeof(uint8_t), (*formatinstance).segCountX2);
        instructionpointer += sizeof(sizeof(uint8_t), 12 +(*formatinstance).segCountX2);
        (*formatinstance).reservedPad = *instructionpointer;
        (*formatinstance).startCode = (uint16_t*)(instructionpointer + 2);
        (*formatinstance).startCode = (uint16_t*)calloc(sizeof(uint8_t), (*formatinstance).segCountX2);
         instructionpointer += sizeof(sizeof(uint8_t), 2 + (*formatinstance).segCountX2);
        (*formatinstance).idDelta = (uint16_t*)instructionpointer;
        (*formatinstance).idDelta= (uint16_t*)calloc(sizeof(uint8_t), (*formatinstance).segCountX2);
        instructionpointer += sizeof(sizeof(uint8_t), (*formatinstance).segCountX2);
        (*formatinstance).idRangeOffset = (uint16_t*)instructionpointer;
        (*formatinstance).idRangeOffset = (uint16_t*)calloc(sizeof(uint8_t), (*formatinstance).segCountX2);
        instructionpointer += sizeof(sizeof(uint8_t), (*formatinstance).segCountX2);
        (*formatinstance).glyphindexArray = (uint8_t*)instructionpointer; // Needs to be calculated by glyphIndexAddress = idRangeOffset[i] + 2 * (c - startCode[i]) + (Ptr) &idRangeOffset[i]
        instance = (uint16_t*)formatinstance;
        instance = (uint16_t*)calloc(sizeof(*formatinstance), 1);
        formatinstance = 0;
        free(formatinstance);
        return instance;
    }

    else if (*(*cmap).format == 6)  //OPTIONAL TO-DO, add the other formats other than 10
    {
        struct cmapformat6 *formatinstance;
        (*formatinstance).format = *instructionpointer;
        (*formatinstance).length = (uint8_t)*(instructionpointer + 2);
        (*formatinstance).language = (uint8_t)*(instructionpointer + 4);
        (*formatinstance).firstCode = (uint8_t)*(instructionpointer + 6);
        (*formatinstance).entryCount = (uint8_t)*(instructionpointer + 8);
        (*formatinstance).glyphindexArray = (uint8_t*)instructionpointer + 10;
        (*formatinstance).glyphindexArray = (uint8_t*)calloc(sizeof(uint8_t), (*formatinstance).entryCount);
        instance = (uint16_t*)calloc(sizeof(*formatinstance), 1);
        formatinstance = 0;
        free(formatinstance);
         return instance;
    }

    else  //No support for gathering information on other formats. As of December 23rd 2020, the current aim is being able to process the font and thus the format of Dyslexia Font on Unicode which is format 6
    {

    }
    return  NULL;

}

struct cmap *cmapsetup(uint8_t *instructionpointer, struct FontTable cmapentry)  //only implements for format 10
{
    long *Origin;
    uint32_t offset;

    instructionpointer = (uint8_t*)Origin + (uint8_t)*cmapentry.length;
    struct cmap *instance;
    instance = (struct cmap *) instructionpointer;
    (*instance).version = (uint16_t)*instructionpointer;
    (*instance).platformID  = 2;     //This is a reserved value so I will initialize it to this until searched value is found
 if ((*instance).platformID != 0)    // Only supporting Unicode
 {
    (*instance).numberSubtables = (uint16_t)*(instructionpointer + 1);
    (*instance).platformID = (uint16_t)*(instructionpointer + 2);
    (*instance).platformSpecificID = (uint16_t)*(instructionpointer + 3);
    (*instance).offset = (uint32_t)*(instructionpointer + 4);
    instructionpointer = (uint8_t *)&instance->version + (*instance).offset;
    (*instance).format = cmapformatsetup(instructionpointer, instance);
    return instance;
 }
}



struct glyf *glyfsetup(struct glyf *glyf, struct InstructionKit *FontEngine, uint16_t glyphindex)  //only implements for format 10
{
    uint16_t *GlyphLoc;
    uint32_t offset;
    GlyphLoc = (*FontEngine).GlyphEntry + glyphindex;
    uint8_t flagtest[6] = {1,2,3,4,5,6};
    (*glyf).numberOfContours = *GlyphLoc;     //These values are in simple and compound glyphs
    (*glyf).xMin = (FWord)*(GlyphLoc + 1);
    (*glyf).yMin = (FWord)*(GlyphLoc + 2);
    (*glyf).xMax = (FWord)*(GlyphLoc + 3);
    (*glyf).yMax = (FWord)*(GlyphLoc + 4);
    (*glyf).endPtsOfContours = (uint16_t*)((*FontEngine).GlyphEntry + 5);
    (*glyf).endPtsOfContours = (uint16_t*)calloc(sizeof(uint16_t), ((*glyf).numberOfContours));
    GlyphLoc +=  12 + sizeof(*(*glyf).endPtsOfContours);
    (*glyf).instructionLength = *GlyphLoc;
    (*glyf).instructions =  (uint8_t*)(GlyphLoc + 2);
    (*glyf).instructions =  (uint8_t*)calloc(sizeof(uint8_t), ((*glyf).instructionLength));
    GlyphLoc += sizeof((*glyf).instructions);
    (*glyf).flags = (uint8_t*)(GlyphLoc);
    return glyf;
}

struct maxp *maxpsetup(uint8_t *instructionpointer, struct FontTable maxpentry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(maxpentry).length;
    struct  maxp *instance;
    (*instance).version = *instructionpointer;
    (*instance).numGlyphs = *(instructionpointer + 2);
    (*instance).maxPoints = (F2Dot14(*)[1][1])calloc(sizeof(F2Dot14(*)[2]), (uint16_t)*(instructionpointer + 4));
    (*instance).maxContours = *(instructionpointer + 6);
    (*instance).maxComponentPoints = (uint8_t*)(instructionpointer + 8);
    (*instance).maxComponentContours = *(instructionpointer + 10);
    (*instance).maxZones = *(instructionpointer + 12);
    (*instance).maxTwilightPoints = (uint16_t *)(instructionpointer + 14);
    (*instance).maxStorage =  (uint16_t)*(instructionpointer + 16);
    (*instance).maxFunctionDefs =  (uint32_t)*(instructionpointer + 18);
    (*instance).maxInstructionDefs =(uint16_t)*(instructionpointer + 20);
    (*instance).Stack = (uint32_t*)calloc((uint16_t)*(instructionpointer + 22), sizeof(uint16_t));
    (*instance).maxSizeOfInstructions = (uint16_t)*(instructionpointer + 24);
    (*instance).maxComponentElements = *(instructionpointer + 26);
    (*instance).maxComponentDepth = *(instructionpointer + 28);
    return instance;
}

struct head *headsetup(uint8_t *instructionpointer, struct FontTable headentry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(headentry).length;
    struct head *instance;
    (*instance).version = *instructionpointer;
    (*instance).fontRevision = *(instructionpointer + 2);
    (*instance).checkSumAdjustment = (uint32_t)*(instructionpointer + 4);
    (*instance).magicNumber =  (uint32_t)*(instructionpointer + 8);
    (*instance).flags = *(instructionpointer + 12);
    (*instance).unitsPerEm = *(instructionpointer + 14);
    (*instance).created = *(instructionpointer + 16);
    (*instance).modified = *(instructionpointer + 32);
    (*instance).xMin = *(instructionpointer + 48);
    (*instance).yMin = *(instructionpointer + 50);
    (*instance).xMax = *(instructionpointer + 52);
    (*instance).yMax = *(instructionpointer + 54);
    (*instance).macStyle = *(instructionpointer + 56);
    (*instance).lowestRecPPEm = *(instructionpointer + 58);
    (*instance).fontDirectionHint= *(instructionpointer + 60);
    (*instance).indexToLocFormat = *(instructionpointer + 62);
    (*instance).glyphDataFormat = *(instructionpointer + 64);
    return instance;
}

struct hhea *hheasetup(uint8_t *instructionpointer, struct FontTable hheaentry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(hheaentry).length;
    struct hhea *instance;
    (*instance).version = *instructionpointer;
    (*instance).ascent = *(instructionpointer + 2);
    (*instance).descent = (uint16_t)*(instructionpointer + 4);
    (*instance).lineGap =  (uint16_t)*(instructionpointer + 6);
    (*instance).advanceWidthMax = *(instructionpointer + 8);
    (*instance).minLeftSideBearing = *(instructionpointer + 10);
    (*instance).minRightSideBearing = *(instructionpointer + 12);
    (*instance).xMaxExtent = *(instructionpointer + 14);
    (*instance).caretSlopeRise = *(instructionpointer + 16);
    (*instance).caretSlopeRun = *(instructionpointer + 18);
    (*instance).caretOffset = *(instructionpointer + 20);
    (*instance).metricDataFormat = *(instructionpointer + 28);
    (*instance).numOfLongHorMetric = *(instructionpointer + 30);
    return instance;
}

struct hmtx *hmtxsetup(uint8_t *instructionpointer, struct FontTable hmtxentry, struct hhea hhea)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(hmtxentry).length;
    struct hmtx *instance;
    (*instance).hMetrics = (uint16_t*)calloc(sizeof(uint64_t), hhea.numOfLongHorMetric);
    (*instance).leftSideBearing; //TO-DO Find number of glyphs
    return instance;
}

struct loca *locasetup(char *Origin, struct FontTable locaentry)  //only implements for format 10
{
    struct loca *instance = (struct loca*)(Origin + *locaentry.length);
    return instance;
}

FWord *cvtsetup(uint8_t *instructionpointer, struct FontTable cvtentry)  //only implements for format 10
{   long *Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)(*cvtentry.length);
    FWord *instance = (FWord *)calloc(sizeof(uint32_t), (*cvtentry.length)/4);
    instance = (FWord*)instructionpointer;        //Copies control value table
return instance;
}
uint8_t *prepsetup(uint8_t *instructionpointer, struct FontTable prepentry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    uint8_t *instance = (uint8_t *)calloc(sizeof(uint32_t), *prepentry.length);
    return instance;
}

uint8_t *fpgmsetup(uint8_t*instructionpointer, struct FontTable fpgmentry)
{
    long Origin;
    uint32_t offset;
    uint8_t *instance = (uint8_t *)calloc(sizeof(uint32_t), *fpgmentry.length);
    return instance;
}

struct kern *kernsetup(uint8_t *instructionpointer, struct FontTable kernentry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(kernentry).length;
    struct kern *instance;
    (*instance).version = (uint32_t)*instructionpointer;
    (*instance).nTables = (uint32_t)*(instructionpointer + 4);
    (*instance).length = (uint32_t)*(instructionpointer + 8);
    (*instance).coverage = (uint32_t)*(instructionpointer + 10);
    (*instance).tupleIndex = (uint32_t)*(instructionpointer + 12);
    return instance;
}

struct post *postsetup(uint8_t *instructionpointer, struct FontTable postentry, struct head head)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(postentry).length;
    struct post *instance;
    (*instance).format = *instructionpointer;
    (*instance).italicAngle = *(instructionpointer + 2);
    (*instance).underlinePosition = *(instructionpointer + 4);
    (*instance).underlineThickness = *(instructionpointer + 6);
    (*instance).isFixedPitch = *(instructionpointer + 8);
    (*instance).minMemType42 = *(instructionpointer + 12);
    (*instance).maxMemType42 = *(instructionpointer + 16);
    (*instance).minMemType1 = *(instructionpointer + 20);
    (*instance).maxMemType1= *(instructionpointer + 24);
    return instance;
}


struct OS_2 *OS_2setup(uint8_t *instructionpointer, struct FontTable OS_2entry)  //only implements for format 10
{   long Origin;
    uint32_t offset;
    instructionpointer = (uint8_t*)Origin + (uint8_t)*(OS_2entry).length;
    struct OS_2 *instance;
    (*instance).version = (uint16_t)*instructionpointer;
    (*instance).xAvgCharWidth = (int16_t)*(instructionpointer + 2);
    (*instance).usWeightClass = (uint16_t)*(instructionpointer + 4);
    (*instance).usWidthClass = (uint16_t)*(instructionpointer + 6);
    (*instance).fsType = (int16_t)*(instructionpointer + 8);
    (*instance).ySubscriptXSize = (int16_t)*(instructionpointer + 10);
    (*instance).ySubscriptYSize = (int16_t)*(instructionpointer + 12);
    (*instance).ySubscriptXOffset = (int16_t)*(instructionpointer + 14);
    (*instance).ySubscriptYOffset = (int16_t)*(instructionpointer + 16);
    (*instance).ySuperscriptXSize = (int16_t)*(instructionpointer + 18);
    (*instance).ySuperscriptYSize = (int16_t)*(instructionpointer + 20);
    (*instance).ySuperscriptXOffset = (int16_t)*(instructionpointer + 22);
    (*instance).ySuperscriptYOffset = (int16_t)*(instructionpointer + 24);
    (*instance).yStrikeoutSize = (int16_t)*(instructionpointer + 26);
    (*instance).yStrikeoutPosition = (int16_t)*(instructionpointer + 28);
    (*instance).sFamilyClass = (int16_t)*(instructionpointer + 30);
    (*instance).panose[10] = (int8_t)*(instructionpointer + 32);
    (*instance).ulUnicodeRange[4] = (int8_t)*(instructionpointer + 42);
    (*instance).achVendID[4] = (int32_t)*(instructionpointer + 46);
    (*instance).fsSelection = (int8_t)*(instructionpointer + 62);
    (*instance).fsFirstCharIndex = (int8_t)*(instructionpointer + 64);
    (*instance).fsLastCharIndex = (int8_t)*(instructionpointer + 66);
    return instance;
}

extern void instructionentry(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct glyf *glyf);

long *Origin;
struct InstructionKit *InitializeFontEngine(uint8_t *parser, struct FontTable *FontTables, uint32_t dpi, uint32_t PointSize)
{
    char *Origin = parser;  //Should point to beginning of the file
    uint32_t characterlist[255];
    uint8_t *fpgm = fpgmsetup(parser, FontTables[fpgm_X]);   //Runs on very first setup
    uint8_t *prep  = prepsetup(parser, FontTables[prep_X]);   // Runs every execution and font transformation
    struct OS_2 *OS_2 = NULL; //Windows Specific Information
    struct post *post = NULL; // Is not needed unless Post Printer is present which is checked by later statement
    struct maxp *maxp = maxpsetup(parser, FontTables[maxp_X]);  // Defines memory requirements
    struct head *head = headsetup(parser, FontTables[head_X]);
    struct hhea *hhea = hheasetup(parser, FontTables [hhea_X]);
    struct kern *kern = kernsetup(parser, FontTables[kern_X]);
    struct InstructionKit *EngineTools = InitializeMemoryLocations(maxp, head, PointSize, dpi);
    (*EngineTools).GlyphEntry = (uint16_t *)(Origin + *(FontTables[glyf_X]).length);
    struct cmap *cmap = cmapsetup(parser, FontTables[cmap_X]);   // Maps the arrangment of character in host OS to arrangment inside font file
    (*EngineTools).loca  = locasetup(Origin, FontTables[loca_X]);
    (*EngineTools).htmx  = hmtxsetup(parser, FontTables[hmtx_X], *hhea);
    (*EngineTools).controlvaluetable = cvtsetup(parser, FontTables[cvt_X]);
    for (uint16_t i = 0; i < sizeof(EngineTools->GlyphCharacterOffsets); i++)  //Stores addresses of each glyph in Font File
        {
            EngineTools -> GlyphCharacterOffsets[i] = searchglyph((characterlist + i), cmap, EngineTools);
        }
    instructionentry(fpgm, EngineTools, (struct glyf*) NULL);
    instructionentry(prep, EngineTools, (struct glyf*) NULL);
    return EngineTools;
}

struct OffsetTable SetTable(uint8_t *instructionpointer, struct OffsetTable Table)
{
    Table.scalar_type = (uint32_t)*instructionpointer;
    if (Table.scalar_type == 65336)   //Winodw or Adobe Types
    {
        instructionpointer += 4;
        Table.numTables = ((uint16_t*)instructionpointer  + 1);
        Table.rangeShift = ((uint16_t*)instructionpointer+ 2);
        Table.searchSelector = ((uint16_t*)instructionpointer + 3);
        Table.searchRange = ((uint16_t*)instructionpointer + 4);
        instructionpointer += 8; //Points to very entry of Table
        return Table;
    }
    else if (Table.scalar_type == 1330926671)     // Version Number  OTTO in Decimal  TO-DO, add support hexademical OTTO 0x4F54544F
    {
        printf("Error, Mac OS is not supported");
    }
}

static inline struct FontTable loadTable(uint8_t *instructionpointer, struct FontTable Table)
{
    uint32_t *tag = (uint32_t*)instructionpointer;     // *Loads the font table to the respect struct
    uint32_t *length = ((uint32_t*)instructionpointer + 1);
    uint32_t *offset = ((uint32_t*)instructionpointer + 2);
    uint32_t *checksum = ((uint32_t*)instructionpointer + 3);

    return Table;
}

char PointZone(uint8_t *instructionpointer, FWord *Zone[255])
{
    FWord *ZonePointer = Zone[*instructionpointer];       //The point being manipulated should be referred as the value stored in the array
}

struct FontTable *FontDirectory(uint8_t *instructionpointer)
{
    instructionpointer += 6;   //Since every entry in a Font directory is 6 bytes long, it shall jump head and the following pointers will take their values in respect to this
    long *Origin;
    *Origin  =  ftell((FILE*)instructionpointer);
    fseek((FILE*)instructionpointer, 0L, SEEK_END);                 /* This will seek the end of file in order to know to stop searching for tables*/
    long EndF = ftell((FILE*)instructionpointer);
    // This will store the length and offset of the last found table
    struct FontTable *FontTables = (struct FontTable*)calloc(sizeof(struct FontTable), NumOfSupportedFTables);

    int i; //array value i pointer
    const char cmap[5] = "cmap";  //Required Tables
    const char glyf[5] = "glyf";
    const char head[5] = "head";
    const char hhea[5] = "hhea";
    const char hmtx[5] = "hmtx";
    const char loca[5] = "loca";
    const char maxp[5] = "maxp";
    const char name[5] = "name";
    const char post[5] = "post";

    const char cvt[5] = "cvt "; //optional Tables
    const char fpgm[5] = "fpgm";
    const char hdmx[5] = "hdmx";
    const char kern[5] = "kern";
    const char OS_2[5] = "OS/2";
    const char prep[5] = "prep";

    int FoundEntrees;
    long LastTable;

    while ((LastTable + 3) != EndF)
    {
        long LastTable;
        if ((char*)instructionpointer == OS_2)
        {
            struct FontTable OS_2_FT;
            FontTables[OS_2_X] = loadTable(instructionpointer, OS_2_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == cmap)
        {
            struct FontTable cmap_FT;
            FontTables[cmap_X] = loadTable(instructionpointer, cmap_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == cvt)
        {
            struct FontTable cvt_FT;
            FontTables[cvt_X] = loadTable(instructionpointer, cvt_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == fpgm)
        {
            struct FontTable fpgm_FT;
            FontTables[fpgm_X] = loadTable(instructionpointer, fpgm_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == glyf)
        {
            struct FontTable glyf_FT;
            FontTables[glyf_X] = loadTable(instructionpointer, glyf_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == head)
        {
            struct FontTable head_FT;
            FontTables[head_X] = loadTable(instructionpointer, head_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == hhea)
        {
            struct FontTable hhea_FT;
            FontTables[hhea_X] = loadTable(instructionpointer, hhea_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == hmtx)
        {
            struct FontTable hmtx_FT;
            FontTables[hmtx_X] = loadTable(instructionpointer, hmtx_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == kern)
        {
            struct FontTable kern_FT;
            FontTables[kern_X] = loadTable(instructionpointer, kern_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == loca)
        {
            struct FontTable loca_FT;
            FontTables[loca_X] = loadTable(instructionpointer, loca_FT);
        instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == maxp)
        {
            struct FontTable maxp_FT;
            FontTables[maxp_X] = loadTable(instructionpointer, maxp_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == post)
        {
            struct FontTable post_FT;
            FontTables[post_X] = loadTable(instructionpointer, post_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        if ((char*)instructionpointer == prep)
        {
            struct FontTable prep_FT;
            FontTables[prep_X] = loadTable(instructionpointer, prep_FT);
            instructionpointer += 8;
            FoundEntrees++;
        }
        struct FontTable *dummystruct;
        dummystruct-> offset = FontTables[FoundEntrees].offset;
        dummystruct-> length = FontTables[FoundEntrees].length;
        LastTable = *dummystruct->length +  *dummystruct->offset  + *Origin;   // It will add the length and address of the last found table; If at the very last table, it should equal to EOL which ends the loop
        free(dummystruct);
        }
    instructionpointer = (uint8_t*)Origin + 15; // This will bring pointer back to pointing at first found element
    return FontTables;
}


#endif //FONTTABLESETUP_H