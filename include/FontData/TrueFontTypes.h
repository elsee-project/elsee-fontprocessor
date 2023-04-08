#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <FontData/cmap.h>


#define NumOfSupportedFTables 15

typedef float shortFrac;
typedef float Fixed;
typedef int16_t FWord;
typedef uint16_t uFWord;
typedef float  F2Dot14;         //Extended 16 bits to 32
typedef uint32_t tag;
typedef uint32_t checkSum;
typedef uint32_t length;
typedef double longDateTime;
typedef uint32_t flags;

FWord xMin;                 //These variables shall be global
FWord yMin;
FWord xMax;
FWord yMax;
uint16_t numberOfContours;

enum Unicode_Specific{Version1=0, Version1_1, ISO_10646, UnicodeBMP_U, UnicodeExtra, UnicodeVariation, LastResort};
enum Windows_Specific{Symbol=0, UnicodeBMP_W, Shift_JIS, PRC, BigFive, Johab, UCS_4};
enum platform{Unicode=0, Macintosh, TT_reserved, Microsoft};

// Control value porgram, handles any changes to the font
//Stores the addresses of the tags
struct GraphicStates
{
    uint32_t zp[3];
    uint32_t rp[3];
    uint8_t *instructionpointer;
    uint32_t *conditonalpointer;
    uint32_t DeltaBase ;
    uint32_t DeltaShift;
    uint32_t loop;
    F2Dot14  minDistance;
    uint32_t scanControl;
    uint32_t InstructControl;
    uint32_t autoFlip;
    uint32_t roundState; //to grid
    uint8_t *SuperBool;
    F2Dot14 controlvalue_cut_in;
    F2Dot14 single_width_cut_in;
    F2Dot14 single_width_value;
    F2Dot14 ProjectionVector[2]; //default x
    F2Dot14 FreedomVector[2]; //default x
    F2Dot14 DualProjectionVector[2];
};

enum Tables{ OS_2_X, cmap_X, glyf_X, head_X, hhea_X, hmtx_X, loca_X, maxp_X, name_X, post_X, cvt_X, fpgm_X, hdmx_X, kern_X, prep_X};


FWord (*Zone)[255];  // Point to an array of 255 with 255 being the set maximum amount of points. It is a pointer to an array of 255 since we won't need to jump to the original memory values

struct OffsetTable
{
    uint32_t scalar_type;
    uint16_t *numTables;
    uint16_t *searchRange;
    uint16_t *searchSelector;
    uint16_t *rangeShift;
};

struct FontTable
{
    uint32_t *tag;
    uint32_t *checksum;
    uint32_t *offset;
    uint32_t *length;
};

struct OS_2
{
    Fixed version;
    int16_t  xAvgCharWidth;
    uint16_t *endPtsOfContours; // = (uint16_t*)calloc(sizeof(uint16_t), numberOfContours);
    int16_t  usWeightClass;
    int16_t  usWidthClass;
    int16_t  fsType;
    int16_t  ySubscriptXSize;
    int16_t  ySubscriptYSize;
    int16_t  ySubscriptXOffset;
    int16_t  ySubscriptYOffset;
    int16_t  ySuperscriptXSize;
    int16_t  ySuperscriptYSize;
    int16_t  ySuperscriptXOffset;
    int16_t  ySuperscriptYOffset;
    int16_t  yStrikeoutSize;
    int16_t  yStrikeoutPosition;
    int16_t  sFamilyClass;
    int8_t   panose[10];
    uint32_t ulUnicodeRange[4];
    uint8_t  achVendID[4];
    int16_t  fsSelection;
    int16_t  fsFirstCharIndex;
    int16_t  fsLastCharIndex;

   /* uint8 	flags[variable];
    int16 	xCoordinates[];
    int16 	yCoordinates[];*/

};

struct cmap
{
    uint16_t version;
    uint16_t numberSubtables;
    uint16_t platformID;

    uint16_t platformSpecificID;

    uint32_t offset;
    uint16_t *format;
};

struct glyf
{
    int16_t numberOfContours;
    FWord xMin;
    FWord yMin;
    FWord xMax;
    FWord yMax;
    uint16_t instructionLength;
    uint8_t *instructions;
    uint16_t *endPtsOfContours; //=  (uint8_t*)calloc(sizeof(uint8_t), instructionLength);
    uint8_t *xCoordinates;
    uint8_t *yCoordinates;
    uint8_t *flags;  //allocate based on number of points when accessing a glyf
    uint16_t *componentflags;
    int16_t	glyphIndex;
    uint8_t arg1;
    uint8_t arg2;
    uint16_t	transformation[4];
};

struct head
{
   Fixed version;
   Fixed fontRevision;
   uint32_t checkSumAdjustment;
   uint32_t magicNumber;
   uint16_t flags;
   uint16_t unitsPerEm;
   longDateTime created;
   longDateTime modified;
   FWord xMin;
   FWord yMin;
   FWord xMax;
   FWord yMax;
   uint16_t macStyle;
   uint16_t lowestRecPPEm;
   uint16_t fontDirectionHint;
   uint16_t indexToLocFormat;
   uint16_t glyphDataFormat;
};

struct hhea
{   Fixed version;
    FWord ascent;
    FWord 	descent;
    FWord 	lineGap;
    uFWord 	advanceWidthMax;
    FWord 	minLeftSideBearing;
    FWord 	minRightSideBearing;
    FWord 	xMaxExtent;
    int16_t caretSlopeRise;
    int16_t 	caretSlopeRun;
    FWord 	caretOffset;
    int16_t 	metricDataFormat;
    uint16_t   	numOfLongHorMetric;
};

struct hmtx
{
    uint16_t hMetrics; //= (uint16_t*)calloc(sizeof(uint16_t), (*numOfLongHorMetric));
    FWord leftSideBearing;
};

struct loca
{
    uint16_t *offsets; // = (uint16_t*)calloc(sizeof(glyphs), glyphs);
};

struct maxp
{
    Fixed 	version;
    uint16_t numGlyphs;
    F2Dot14 (*maxPoints)[1][1];
    uint16_t maxContours;
    uint8_t  *maxComponentPoints;
    uint16_t maxComponentContours;
    uint16_t maxZones;
    uint16_t *maxTwilightPoints;
    uint32_t *maxStorage;
    uint16_t maxFunctionDefs;
    uint16_t maxInstructionDefs;
    uint32_t *Stack;
    uint16_t maxSizeOfInstructions;
    uint16_t maxComponentElements;
    uint16_t maxComponentDepth;  //Level of Recursion. 0 if simple

};


struct name
{
    uint16_t count;
    uint16_t name;
    uint16_t *nameRecord; // = (uint16_t*)calloc(sizeof(uint16_t), (count));
    uint16_t language;
    uint8_t glyphindexArray[255];
    uint16_t platformID;
    uint16_t platformSpecificID;
    uint16_t nameID;
    uint16_t languageID;
    uint16_t length;
    uint32_t offset;
    uint16_t format;


};

struct post
{
    Fixed format;
    Fixed 	italicAngle;
    FWord 	underlinePosition;
    FWord 	underlineThickness;
    uint32_t 	isFixedPitch;
    uint32_t 	minMemType42;
    uint32_t 	maxMemType42;
    uint32_t 	minMemType1;
    uint32_t 	maxMemType1;

};

struct kern
{
    uint32_t version;
    uint32_t nTables;
    uint32_t length;
    uint16_t coverage;
    uint16_t tupleIndex;
};

char flagsForLater(uint16_t flags)
{
    char flagbool[14];   //Following flags definitions are from the Apple Devleopment True Font Refererence Manual
    uint16_t ybaseline = 1;                 //y value of 0 specifies baseline
    uint16_t XPosLSB =   2;                 //x position of left most black bit is LSB
    uint16_t ScalePointDiffer = 4;          //scaled point size and actual point size will differ (i.e. 24 point glyph differs from 12 point glyph scaled by factor of 2)
    uint16_t IntegerScale  = 8;             //use integer scaling instead of fractional
    uint16_t MicrosoftScaler = 16;          //used by the Microsoft implementation of the TrueType scaler
    uint16_t VerticalLayoutAndXZero = 32;   //This bit should be set in fonts that are intended to e laid out vertically, and in which the glyphs have been drawn such that an x-coordinate of 0 corresponds to the desired vertical baseline.
    uint16_t RESERVED = 64;                 //This bit must be set to zero.
    uint16_t SpecialLayout = 128;           //This bit should be set if the font requires layout for correct linguistic rendering (e.g. Arabic fonts).
    uint16_t MetamorphEffects = 256;        //This bit should be set for an AAT font which has one or more metamorphosis effects designated as happening by default.
    uint16_t R2LPresent = 512;              //This bit should be set if the font contains any strong right-to-left glyphs.
    uint16_t IndicStyleEffects = 1024;      //This bit should be set if the font contains Indic-style rearrangement effects.
    uint16_t AdobeDef1 = 2048;              //Defined by Adobe.
    uint16_t AdobeDef2 = 4096;              //Defined by Adobe.
    uint16_t AdobeDef3 = 8192;              //Defined by Adobe.
    uint16_t GenericSymbols = 16384;        //This bit should be set if the glyphs in the font are simply generic symbols for code point ranges, such as for a last resort font.
}

char static inline flagcheck(uint16_t func, uint16_t *flag)
{
    if ((func & *flag) == 1)
    {
        return 1;
    }
    else
        {
            return 0;
        }
}

uint32_t VerifyChecksum(uint32_t **Table, uint32_t *length)
{
    uint32_t result = 0;
    uint32_t Counter = (*length + 3)/4;
    while (Counter-- > 0)
    {
        result += **Table;
    }
    return result;
}
