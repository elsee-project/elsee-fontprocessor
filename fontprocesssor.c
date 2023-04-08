#include <stdio.h>
#include <FontMachineRoutines.h>


char FontEntry(FILE* FontFile , uint32_t dpi, uint32_t PointSize)
{
    long *ActiveFontEngines;              //In the case why someone asks why I am aggressive with pointers; it is mainly to ensure that seperate data is used in the event that mutliple fonts are used.
    char FunctionFlags;                         //1st bit being true means that there is valid data from a previous run, 2nd being true  means a Font Transformation(Ex: Font size change) was requested, 3rd being true means post printer is present, 4th means OS is Windows, 8th bit set means to exit exit
    char *parser = (char)FontFile;
    checktable(parser);
    struct FontTable *FontTables = FontDirectory(parser);
    struct InstructionKit *EngineTools = InitializeFontEngine(parser, FontTables, dpi, PointSize);
    for (int i = 0; i < sizeof(EngineTools -> GlyphCharacterOffsets); i++)
    {
        struct glyf *glyf = glyfsetup(glyf, EngineTools, EngineTools -> GlyphCharacterOffsets[i]);
        SetPoints(glyf, EngineTools);
        glyphentry(parser, EngineTools, glyf);
        //TO-DO Complete and put Scan command here

        //Command should exit an array of pointers pointing to glyphs
    }
}

char FontChange(FILE* FontFile , uint32_t dpi, uint32_t PointSize)
{
    long (*ActiveFontEngines)[1];              //In the case why someone asks why I am aggressive with pointers; it is mainly to ensure that seperate data is used in the event that mutliple fonts are used.
    char FunctionFlags;                         //1st bit being true means that there is valid data from a previous run, 2nd being true  means a Font Transformation(Ex: Font size change) was requested, 3rd being true means post printer is present, 4th means OS is Windows, 8th bit set means to exit exit
    char *parser = (char)FontFile;
    struct FontTable *FontTables = FontDirectory(parser);
    struct InstructionKit *EngineTools = InitializeFontEngine(parser, FontTables, dpi, PointSize);
    for (int i = 0; i < sizeof(EngineTools -> GlyphCharacterOffsets); i++)
    {
        struct glyf *glyf = glyfsetup(glyf, EngineTools, EngineTools -> GlyphCharacterOffsets[i]);
        SetPoints(glyf, EngineTools);
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

char exitFontEngine(struct instructionKit *FontEngine)
{
free(FontEngine);
}

int FileReference(FILE* FontEntry, long *ActiveFontEngines)
    {
        for (int i = 0; i < sizeof(ActiveFontEngines); i ++)
        {
            if (ActiveFontEngines[i] == &FontEntry )
                {
                    if (FontEntry == 0)
                    {
                        fclose(ActiveFontEngines[i]);
                        ActiveFontEngines[i] = 0;
                    }

                }
             }
        ActiveFontEngines = malloc(sizeof(ActiveFontEngines)+1);
    }
