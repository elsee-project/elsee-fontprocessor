#include <FontEngineInit.h>

struct GraphicStates *InitializeGraphicStates(void) {
    struct GraphicStates *instance;
    (*instance).zp[3];
    (*instance).rp[3];
    (*instance).conditonalpointer;
    (*instance).DeltaBase = 9;
    (*instance).DeltaShift = 3;
    (*instance).loop = 1;
    (*instance).InstructControl = 0;
    (*instance).autoFlip = 1;
    (*instance).roundState = 1;
    *(*instance).SuperBool = 0;
    (*instance).controlvalue_cut_in = 17;
    (*instance).single_width_cut_in = 0;
    (*instance).single_width_value = 0;
    (*instance).ProjectionVector[2];
    (*instance).ProjectionVector[0] = 0;
    (*instance).ProjectionVector[1] = 1; //default x
    (*instance).FreedomVector[2];  //default x
    (*instance).FreedomVector[0] = 0;
    (*instance).FreedomVector[1] = 1;
    (*instance).DualProjectionVector[2];
    return instance;
};

extern void FunctionGroupZero(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupOne(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupTwo(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupThree(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupFour(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupFive(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupSix(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupSeven(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates);
extern void FunctionGroupEight(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RunTimeStates);
extern void MDRP(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RunTimeStates);
extern void MIRP(uint8_t *instructionpointer , struct InstructionKit *MemoryLocations, struct GraphicStates *RunTimeStates);

uint32_t NPUSH(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{
    uint8_t *dummy;
    *dummy = *(instructionpointer + 1);
    if (*instructionpointer = 41)
    {
        instructionpointer += 2;
        for(int i = 0; i < *dummy; i++)
        {
            *instructionpointer = (uint8_t)*(MemoryLocations -> StackPointer + i);
        }
    }

    else if (*dummy == 7)  //Instruction contained amount to be added -1
    {
        *dummy =  *(instructionpointer++)*2;  //This should increment the instruction pointer; need to validate
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }

    else if (*dummy < 7)  //Instruction contained amount to be added -1
    {
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }
    else
    {
        *dummy = (*dummy - 8) *2;
        instructionpointer++;
        for (int i =0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }


    free(dummy);
    dummy = 0;

}

uint32_t PUSH(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations)
{   uint8_t *dummy;
    *dummy = (*instructionpointer%10) -1;
    if (*dummy == 255)  //Next byte shall determine number loops and the loop starts on the next value; it compares to 255 since this is an unsigned value
    {
        *dummy =  *(instructionpointer++);  //This should increment the instruction pointer; need to validate
        instructionpointer++;
        for (int i = 0; i < *dummy; i++)
        {
            MemoryLocations -> StackPointer++;
            *MemoryLocations -> StackPointer = *instructionpointer;
        }
    }
}

void InstructionExecute(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    char HighEnd = *instructionpointer & 240; //Turns the higher 8 bits into 0s, leaving the lower 8 bits
    if(HighEnd < 8)
    {
        if (HighEnd <4)
            {
                if (HighEnd < 2)
                {
                    switch (HighEnd)
                    {
                        case 0:
                            {
                                FunctionGroupZero(instructionpointer, MemoryLocations, RuntimeStates);
                                break;
                            }
                        case 1:
                            {
                                FunctionGroupOne(instructionpointer, MemoryLocations, RuntimeStates);
                                break;
                            }
                    }
                }
                else
                {
                    switch (HighEnd)
                    {
                        case 2:
                            {
                                FunctionGroupTwo(instructionpointer, MemoryLocations, RuntimeStates);
                                break;
                            }
                        case 3:
                            {
                                FunctionGroupThree(instructionpointer, MemoryLocations, RuntimeStates);
                                break;
                            }
                    }
                }
            }
            else if (HighEnd < 6)
            {
                switch (HighEnd)
                {
                    case 4:
                        {
                            FunctionGroupFour(instructionpointer, MemoryLocations, RuntimeStates);
                            break;
                        }
                    case 5:
                        {
                            FunctionGroupFive(instructionpointer, MemoryLocations, RuntimeStates);
                            break;
                        }
                    }
            }

            else
            {
                switch (HighEnd)
                {
                    case 6 :
                        {
                            FunctionGroupSix(instructionpointer, MemoryLocations, RuntimeStates);
                            break;
                        }
                    case 7 :
                        {
                            FunctionGroupSeven(instructionpointer, MemoryLocations, RuntimeStates);
                            break;
                        }
                }
            }
    }

    else if (HighEnd < 12)
    {
        if (HighEnd < 10)
        {
            switch (HighEnd)
            {
                case 8 :
                    {
                        FunctionGroupEight(instructionpointer, MemoryLocations, RuntimeStates);
                        break;
                    }
            }
        }
        else
        {
            NPUSH(instructionpointer, MemoryLocations);
        }
    }

    else if (HighEnd < 14)
    {
        MDRP(instructionpointer, MemoryLocations, RuntimeStates);
    }

    else
    {
        MIRP(instructionpointer, MemoryLocations, RuntimeStates);
    }

    instructionpointer++;
}

void StartExecution(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct GraphicStates *RuntimeStates)
{
    while ((*instructionpointer++) != 45) //Char 45 is the "End Function" Command; instruction pointer will increment until equals "End Function"
    {
    InstructionExecute(instructionpointer, MemoryLocations, RuntimeStates);
    }
}



uint8_t instructionentry(uint8_t *instructionpointer, struct InstructionKit *MemoryLocations, struct glyf *glyf)
{
    if(glyf != NULL)
    {
        struct GraphicStates *RuntimeStates = InitializeGraphicStates();
        StartExecution(instructionpointer, MemoryLocations, RuntimeStates);
    }
    else
    {
        StartExecution(instructionpointer, MemoryLocations, NULL);
    }
}

void fpgmentry (uint8_t *fpgm, struct InstructionKit *EngineInit)
{
    instructionentry(fpgm, EngineInit, NULL);
}

void prepentry (uint8_t *prep, struct InstructionKit *EngineInit)
{
    instructionentry(prep, EngineInit, NULL);
}

void glyphentry(uint8_t *instructionlist, struct InstructionKit *MemoryLocations, struct glyf *glyf)
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