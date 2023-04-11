#include <stdio.h>
#include <FontMachineRoutines.h>
#include <FontEngineInit.h>


extern void glyphentry(uint8_t *instructionlist, struct InstructionKit *MemoryLocations, struct glyf *glyf); 
char FunctionFlags; //1st bit being true means that there is valid data from a previous run, 2nd being true  means a Font Transformation(Ex: Font size change) was requested, 3rd being true means post printer is present, 4th means OS is Windows, 8th bit set means to exit exit

char FontEntry(char filename[255], uint32_t dpi, uint32_t FontSize)    //Only needed input for producing characters from Font File for a display is a TrueType Font file, the dpi on screen and the size of the characters requested 
{
    long *ActiveFontEngines;              //In the case why someone asks why I am aggressive with pointers; it is mainly to ensure that seperate data is used in the event that mutliple fonts are used.                             //1st bit being true means that there is valid data from a previous run, 2nd being true means a Font Transformation(Ex: Font size change) was requested, 3rd being true means post printer is present, 4th means OS is Windows, 8th bit set means to exit exit
    FILE *FontFile = fopen(filename, "rb"); 
    fseek(FontFile, 0, SEEK_END);
    int FF_Size = ftell(FontFile);
    char *parser = (char *)malloc(FF_Size * sizeof(char));
    rewind(FontFile);
    fread(parser, FF_Size, 1, FontFile); // Make a copy of Font file
    fclose(FontFile);  
    //checktable(parser); TODO: Check if font already processed
    struct FontTable *FontTables = FontDirectory(parser);
    struct InstructionKit *EngineTools = InitializeFontEngine(parser, FontTables, dpi, FontSize);
    for (int i = 0; i < sizeof(EngineTools -> GlyphCharacterOffsets); i++)
    {
        struct glyf *glyf = glyfsetup(glyf, EngineTools, EngineTools -> GlyphCharacterOffsets[i]);
        setPoints(glyf, EngineTools);
        glyphentry(parser, EngineTools, glyf);
        //TO-DO Complete and put Scan command here

        //Command should exit an array of pointers pointing to glyphs
    }
}

char FontChange(char filename[255], uint32_t dpi, uint32_t FontSize)  // Font file, screen dpi and 
{
    // long (*ActiveFontEngines)[1];              //In the case why someone asks why I am aggressive with pointers; it is mainly to ensure that seperate data is used in the event that mutliple fonts are used.
                    
    FILE *FontFile = fopen(filename, "rb"); 
    fseek(FontFile, 0, SEEK_END);
    int FF_Size = ftell(FontFile);
    char *parser = (char *)malloc(FF_Size * sizeof(char));
    rewind(FontFile);
    fread(parser, FF_Size, 1, FontFile); // Make a copy of Font file
    fclose(FontFile);  
    //checktable(parser); TODO: Check if font already processed
    struct FontTable *FontTables = FontDirectory(parser);
    struct InstructionKit *EngineTools = InitializeFontEngine(parser, FontTables, dpi, FontSize);
    for (int i = 0; i < sizeof(EngineTools -> GlyphCharacterOffsets); i++)
    {
        struct glyf *glyf = glyfsetup(glyf, EngineTools, EngineTools -> GlyphCharacterOffsets[i]);
        setPoints(glyf, EngineTools);
        int numofGlyphs = 0;
        for(int k = 0; k < sizeof(EngineTools -> ComponentFlags); k++)
        {
            if (EngineTools -> ComponentFlags[k] != 0)
             {
                 numofGlyphs++;
             }
        }
        for (int k = numofGlyphs; k >= 0; k++)
        {
            glyphentry(parser, EngineTools, EngineTools -> GlyphSets[k]);
        }
        //TO-DO Complete and put Scan command here
    }
}

char exitFontEngine(struct InstructionKit *FontEngine)
{
free(FontEngine);
}
