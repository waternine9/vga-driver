#include "../../io.h"
#include "vga.h"

#define VGA_MISCELLANEOUS_REG_READ 0x03CC
#define VGA_MISCELLANEOUS_REG_WRITE 0x03C2

#define VGA_SEQUENCER_ADDR 0x03C4
#define VGA_SEQUENCER_DATA 0x03C5

#define VGA_SEQUENCER_RESET 0x00
#define VGA_SEQUENCER_CLOCKING_MODE 0x01
#define VGA_SEQUENCER_MAP_MASK 0x02
#define VGA_SEQUENCER_CHARACTER_MAP_SELECT 0x03
#define VGA_SEQUENCER_MEMORY_MODE 0x04

#define VGA_CRT_ADDR_FIRST 0x03D4
#define VGA_CRT_ADDR_SECOND 0x03B4
#define VGA_CRT_DATA_FIRST 0x03D5
#define VGA_CRT_DATA_SECOND 0x03B5

#define VGA_CRT_HORIZONTAL_TOTAL 0x00
#define VGA_CRT_HORIZONTAL_DISPLAY_ENABLE_END 0x01
#define VGA_CRT_START_HORIZONTAL_BLANKING 0x02
#define VGA_CRT_END_HORIZONTAL_BLANKING 0x03
#define VGA_CRT_START_HORIZONTAL_RETRACE 0x04
#define VGA_CRT_END_HORIZONTAL_RETRACE 0x05
#define VGA_CRT_VERTICAL_TOTAL 0x06
#define VGA_CRT_OVERFLOW 0x07
#define VGA_CRT_PRESET_ROW_SCAN 0x08
#define VGA_CRT_MAXIMUM_SCAN_LINE 0x09
#define VGA_CRT_CURSOR_START 0x0A
#define VGA_CRT_CURSOR_END 0x0B
#define VGA_CRT_START_ADDRESS_HIGH 0x0C
#define VGA_CRT_START_ADDRESS_LOW 0x0D
#define VGA_CRT_CURSOR_LOCATION_HIGH 0x0E
#define VGA_CRT_CURSOR_LOCATION_LOW 0x0F
#define VGA_CRT_VERTICAL_RETRACE_START 0x10
#define VGA_CRT_VERTICAL_RETRACE_END 0x11
#define VGA_CRT_VERTICAL_DISPLAY_ENABLE_END 0x12
#define VGA_CRT_OFFSET 0x13
#define VGA_CRT_UNDERLINE_LOCATION 0x14
#define VGA_CRT_START_VERTICAL_BLANKING 0x15
#define VGA_CRT_END_VERTICAL_BLANKING 0x16
#define VGA_CRT_MODE 0x17
#define VGA_CRT_LINE_COMPARE 0x18

#define VGA_ATTRIBUTE_ADDR 0x03C0
#define VGA_ATTRIBUTE_WRITE 0x03C0
#define VGA_ATTRIBUTE_READ 0x03C1

#define VGA_ATTRIBUTE_MODE_CONTROL 0x10
#define VGA_ATTRIBUTE_OVERSCAN_COLOR 0x11
#define VGA_ATTRIBUTE_COLOR_PLANE_ENABLE 0x12
#define VGA_ATTRIBUTE_HORIZONTAL_PEL_PANNING 0x13
#define VGA_ATTRIBUTE_COLOR_SELECT 0x14

#define VGA_GRAPHICS_ADDR 0x03CE
#define VGA_GRAPHICS_DATA 0x03CF

#define VGA_GRAPHICS_SET_RESET 0x00
#define VGA_GRAPHICS_DATA_ROTATE 0x03
#define VGA_GRAPHICS_READ_MAP_SELECT 0x04
#define VGA_GRAPHICS_MODE 0x05
#define VGA_GRAPHICS_MISC 0x06
#define VGA_GRAPHICS_COLOR_DONT_CARE 0x07
#define VGA_GRAPHICS_BIT_MASK 0x08

uint8_t IoAddressBit;

void WriteSequencer(uint8_t Index, uint8_t Data)
{
    IO_Out8(VGA_SEQUENCER_ADDR, Index);
    IO_Out8(VGA_SEQUENCER_DATA, Data);
}

uint8_t ReadSequencer(uint8_t Index)
{
    IO_Out8(VGA_SEQUENCER_ADDR, Index);
    return IO_In8(VGA_SEQUENCER_DATA);
}

void WriteGraphics(uint8_t Index, uint8_t Data)
{
    IO_Out8(VGA_GRAPHICS_ADDR, Index);
    IO_Out8(VGA_GRAPHICS_DATA, Data);
}

uint8_t ReadGraphics(uint8_t Index)
{
    IO_Out8(VGA_GRAPHICS_ADDR, Index);
    return IO_In8(VGA_GRAPHICS_DATA);
}

uint16_t GetCRTAddrReg()
{
    if (!IoAddressBit) return VGA_CRT_ADDR_SECOND;
    return VGA_CRT_ADDR_FIRST;
}

uint16_t GetCRTDataReg()
{
    if (!IoAddressBit) return VGA_CRT_DATA_SECOND;
    return VGA_CRT_DATA_FIRST;
}

uint16_t GetInputStatus1Addr()
{
    if (!IoAddressBit) return 0x3BA;
    return 0x3DA;
}

void WriteCRT(uint8_t Index, uint8_t Data)
{
    IO_Out8(GetCRTAddrReg(), Index);
    IO_Out8(GetCRTDataReg(), Data);
}

uint8_t ReadCRT(uint8_t Index)
{
    IO_Out8(GetCRTAddrReg(), Index);
    return IO_In8(GetCRTDataReg()); 
}

void WriteAttribute(uint8_t Index, uint8_t Data)
{
    IO_Out8(VGA_ATTRIBUTE_ADDR, Index);
    IO_Out8(VGA_ATTRIBUTE_WRITE, Data);
    IO_In8(GetInputStatus1Addr()); // Set the attributes to be indexed
    IO_Out8(VGA_ATTRIBUTE_ADDR, 0x20);
}

uint8_t ReadAttribute(uint8_t Index)
{
    IO_Out8(VGA_ATTRIBUTE_ADDR, Index);
    uint8_t Desired = IO_In8(VGA_ATTRIBUTE_READ);
    IO_In8(GetInputStatus1Addr()); // Set the attributes to be indexed
    IO_Out8(VGA_ATTRIBUTE_ADDR, 0x20);
    return Desired;
}

void VGA_Init()
{
    uint8_t Misc = IO_In8(VGA_MISCELLANEOUS_REG_READ);

    IoAddressBit = 1;

    Misc = 0b11000111; // Set the VSP and HSP to NEGATIVE (1) and set ERAM

    IO_Out8(VGA_MISCELLANEOUS_REG_WRITE, Misc);


    WriteCRT(VGA_CRT_VERTICAL_RETRACE_END, 0x0C);
    WriteCRT(VGA_CRT_VERTICAL_RETRACE_START, 0xEA);
    WriteCRT(VGA_CRT_HORIZONTAL_TOTAL, 0x5F);
    WriteCRT(VGA_CRT_VERTICAL_TOTAL, 0x0B);
    WriteCRT(VGA_CRT_START_VERTICAL_BLANKING, 0xE7);
    WriteCRT(VGA_CRT_END_VERTICAL_BLANKING, 0x04);
    WriteCRT(VGA_CRT_UNDERLINE_LOCATION, 0x00);
    WriteCRT(VGA_CRT_PRESET_ROW_SCAN, 0x00);
    WriteCRT(VGA_CRT_HORIZONTAL_DISPLAY_ENABLE_END, 0x4F);
    WriteCRT(VGA_CRT_VERTICAL_DISPLAY_ENABLE_END, 0xDF);
    WriteCRT(VGA_CRT_START_HORIZONTAL_BLANKING, 0x50);
    WriteCRT(VGA_CRT_END_HORIZONTAL_BLANKING, 0x82);
    WriteCRT(VGA_CRT_START_HORIZONTAL_RETRACE, 0x54);
    WriteCRT(VGA_CRT_END_HORIZONTAL_RETRACE, 0x80);
    WriteCRT(VGA_CRT_MAXIMUM_SCAN_LINE, 0x40);
    WriteCRT(VGA_CRT_OFFSET, 0x28);
    WriteCRT(VGA_CRT_OVERFLOW, 0x3E);
    WriteCRT(VGA_CRT_MODE, 0b11000011);
    WriteCRT(VGA_CRT_VERTICAL_RETRACE_END, 0x8C);

    WriteGraphics(VGA_GRAPHICS_MISC, 0x5);
    WriteGraphics(VGA_GRAPHICS_MODE, 0b00000000);
    WriteGraphics(VGA_GRAPHICS_BIT_MASK, 0b11111111);
    WriteGraphics(VGA_GRAPHICS_DATA_ROTATE, 0);

    WriteSequencer(VGA_SEQUENCER_MEMORY_MODE, 0b00000010);
    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00001111);
    WriteSequencer(VGA_SEQUENCER_CHARACTER_MAP_SELECT, 0b0);
    WriteSequencer(VGA_SEQUENCER_CLOCKING_MODE, 0b00000001);

    IO_In8(GetInputStatus1Addr()); // Set the attributes to be indexed at the start
    IO_Out8(VGA_ATTRIBUTE_ADDR, 0x20);
    
    WriteAttribute(VGA_ATTRIBUTE_MODE_CONTROL, 0b00000001);
    WriteAttribute(VGA_ATTRIBUTE_COLOR_PLANE_ENABLE, 0b00001111);
    WriteAttribute(VGA_ATTRIBUTE_COLOR_SELECT, 0b00000000);
    WriteAttribute(VGA_ATTRIBUTE_OVERSCAN_COLOR, 0b00000000);
    WriteAttribute(VGA_ATTRIBUTE_HORIZONTAL_PEL_PANNING, 0b00000000);
}

void VGA_TurnScreenOff()
{
    WriteSequencer(VGA_SEQUENCER_CLOCKING_MODE, (ReadSequencer(VGA_SEQUENCER_CLOCKING_MODE) & 0b00111111) | 0b00100010);
}

void VGA_TurnScreenOn()
{
    WriteSequencer(VGA_SEQUENCER_CLOCKING_MODE, (ReadSequencer(VGA_SEQUENCER_CLOCKING_MODE) & 0b00011111) | 0b00000010);
}

uint8_t CacheMask = 1;

void VGA_SetColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha)
{
    CacheMask = Alpha ? 0b1000 : 0;
    CacheMask |= Red ? 0b100 : 0;
    CacheMask |= Green ? 0b10 : 0;
    CacheMask |= Blue ? 0b1 : 0;
}

void VGA_WritePixel(uint32_t X, uint32_t Y)
{
    if (X >= 640) return;
    if (Y >= 480) return;

    WriteGraphics(VGA_GRAPHICS_BIT_MASK, 0b11111111);
    
    WriteGraphics(VGA_GRAPHICS_READ_MAP_SELECT, 0b00000000);
    uint8_t CurrentMap0 = ((uint8_t*)0xA0000)[X / 8 + Y * 80];
    WriteGraphics(VGA_GRAPHICS_READ_MAP_SELECT, 0b00000001);
    uint8_t CurrentMap1 = ((uint8_t*)0xA0000)[X / 8 + Y * 80];
    WriteGraphics(VGA_GRAPHICS_READ_MAP_SELECT, 0b00000010);
    uint8_t CurrentMap2 = ((uint8_t*)0xA0000)[X / 8 + Y * 80];
    WriteGraphics(VGA_GRAPHICS_READ_MAP_SELECT, 0b00000011);
    uint8_t CurrentMap3 = ((uint8_t*)0xA0000)[X / 8 + Y * 80];

    CurrentMap0 &= ~(1 << (7 - (X % 8)));
    CurrentMap1 &= ~(1 << (7 - (X % 8)));
    CurrentMap2 &= ~(1 << (7 - (X % 8)));
    CurrentMap3 &= ~(1 << (7 - (X % 8)));
    
    if (CacheMask & 0b1) CurrentMap0 |= 1 << (7 - (X % 8));
    if (CacheMask & 0b10) CurrentMap1 |= 1 << (7 - (X % 8));
    if (CacheMask & 0b100) CurrentMap2 |= 1 << (7 - (X % 8));
    if (CacheMask & 0b1000) CurrentMap3 |= 1 << (7 - (X % 8));

    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00000001);
    ((uint8_t*)0xA0000)[X / 8 + Y * 80] = CurrentMap0;
    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00000010);
    ((uint8_t*)0xA0000)[X / 8 + Y * 80] = CurrentMap1;
    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00000100);
    ((uint8_t*)0xA0000)[X / 8 + Y * 80] = CurrentMap2;
    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00001000);
    ((uint8_t*)0xA0000)[X / 8 + Y * 80] = CurrentMap3;
}

void VGA_Clear()
{
    WriteSequencer(VGA_SEQUENCER_MAP_MASK, 0b00001111);
    WriteGraphics(VGA_GRAPHICS_BIT_MASK, 0b11111111);

    for (int i = 0;i < 640 * 480;i++)
    {
        ((uint8_t*)0xA0000)[i] = 0x0;
    }

    WriteSequencer(VGA_SEQUENCER_MAP_MASK, CacheMask);

    for (int i = 0;i < 640 * 480;i++)
    {
        ((uint8_t*)0xA0000)[i] = 0xFF;
    }
}

void VGA_WriteRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    WriteGraphics(VGA_GRAPHICS_BIT_MASK, 0b11111111);
    for (int i = 0;i < 4;i++)
    {
        WriteGraphics(VGA_GRAPHICS_READ_MAP_SELECT, i);
        WriteSequencer(VGA_SEQUENCER_MAP_MASK, (1 << i));
        for (int _x = x;_x < x + w;_x++)
        {
            for (int _y = y;_y < y + h;_y++)
            {
                ((uint8_t*)0xA0000)[_x / 8 + _y * 80] |= 1 << (7 - (_x % 8));
            }
        }
    }
}
