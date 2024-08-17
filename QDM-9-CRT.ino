#include "NopDelay.h"

unsigned int row = 0;
unsigned char font_row = 0;
unsigned char character_row = 0;

volatile unsigned char nondraw_row = 0;

unsigned char dot_row = 0;
unsigned char dot_column = 10;
unsigned char dot_row_old = 0;
unsigned char dot_column_old = 10;

const unsigned int VSYNC_ROWS = 10; // The number of rows to keep the pulse on
const unsigned int SCREEN_END = 306;

const unsigned char TEXT_ROW_START = 1;
const unsigned char TEXT_ROW_END = 30;

const unsigned char TEXT_COLUMNS = 15;
const unsigned char TEXT_ROWS = 28;

const unsigned char bitMask = (1 << PB4);  // Bit mask for the pin

unsigned char memory[28][30] = {
  {'|','A','A',' ',' ',' ',' ',' ','A',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','B','B',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','C','C',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','D','D',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','E','E',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','F','F',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','G','G',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','H','H',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','I','I',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','J','J',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','K','K',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','L','L',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','M','M',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','N','N',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','O','O',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','P','P',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','Q','Q',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','R','R',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','S','S',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','T','T',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','Y','Y',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','V','V',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','W','W',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','X','X',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','Y','Y',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','Z','Z',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'|','!','!',' ',' ',' ',' ',' ',' ',' ', ' ',' ',' ',' ',' ',' ','|','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
  {'#',126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,'#','|','8','9', '0','1','2','3','4','5','6','7','8','9'},
};

const unsigned char bitmap[10][40] = {
    // Example pattern repeated to fill 200 rows
    {0x00, 0b01111110, 0b01000000, 0b01000000, 0b00111100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x00, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x00, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x06, 0b01111100, 0b01000000, 0b01000000, 0b01000010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x06, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x06, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x06, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
    {0x06, 0b01111110, 0b01111110, 0b01111110, 0b00111100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x00, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0x06, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

const char FONT[97][10] = {
{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x30,0x78,0x78,0x30,0x30,0x00,0x30 },{ 0x00,0x00,0x00,0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C },{ 0x00,0x00,0x00,0x30,0x7C,0xC0,0x78,0x0C,0xF8,0x30 },{ 0x00,0x00,0x00,0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6 },{ 0x00,0x00,0x00,0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76 },{ 0x00,0x00,0x00,0x60,0x60,0xC0,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x18,0x30,0x60,0x60,0x60,0x30,0x18 },{ 0x00,0x00,0x00,0x60,0x30,0x18,0x18,0x18,0x30,0x60 },{ 0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00 },{ 0x00,0x00,0x00,0x00,0x30,0x30,0xFC,0x30,0x30,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30 },{ 0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80 },{ 0x00,0x00,0x00,0x7C,0xC6,0xCE,0xDE,0xF6,0xE6,0x7C },{ 0x00,0x00,0x00,0x30,0x70,0x30,0x30,0x30,0x30,0xFC },{ 0x00,0x00,0x00,0x78,0xCC,0x0C,0x38,0x60,0xC4,0xFC },{ 0x00,0x00,0x00,0x78,0xCC,0x0C,0x38,0x0C,0xCC,0x78 },{ 0x00,0x00,0x00,0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x1E },{ 0x00,0x00,0x00,0xFC,0xC0,0xF8,0x0C,0x0C,0xCC,0x78 },{ 0x00,0x00,0x00,0x38,0x60,0xC0,0xF8,0xCC,0xCC,0x78 },{ 0x00,0x00,0x00,0xFC,0xCC,0x0C,0x18,0x30,0x30,0x30 },{ 0x00,0x00,0x00,0x78,0xCC,0xCC,0x78,0xCC,0xCC,0x78 },{ 0x00,0x00,0x00,0x78,0xCC,0xCC,0x7C,0x0C,0x18,0x70 },{ 0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30 },{ 0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x30,0x30,0x60 },{ 0x00,0x00,0x00,0x18,0x30,0x60,0xC0,0x60,0x30,0x18 },{ 0x00,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0xFC,0x00 },{ 0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x18,0x30,0x60 },{ 0x00,0x00,0x00,0x78,0xCC,0x0C,0x18,0x30,0x00,0x30 },{ 0x00,0x00,0x00,0x7C,0xC6,0xDE,0xDE,0xDE,0xC0,0x78 },{ 0x00,0x00,0x00,0x30,0x78,0xCC,0xCC,0xFC,0xCC,0xCC },{ 0x00,0x00,0x00,0xFC,0x66,0x66,0x7C,0x66,0x66,0xFC },{ 0x00,0x00,0x00,0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C },{ 0x00,0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x6C,0xF8 },{ 0x00,0x00,0x00,0xFE,0x62,0x68,0x78,0x68,0x62,0xFE },{ 0x00,0x00,0x00,0xFE,0x62,0x68,0x78,0x68,0x60,0xF0 },{ 0x00,0x00,0x00,0x3C,0x66,0xC0,0xC0,0xCE,0x66,0x3E },{ 0x00,0x00,0x00,0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC },{ 0x00,0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x78 },{ 0x00,0x00,0x00,0x1E,0x0C,0x0C,0x0C,0xCC,0xCC,0x78 },{ 0x00,0x00,0x00,0xE6,0x66,0x6C,0x78,0x6C,0x66,0xE6 },{ 0x00,0x00,0x00,0xF0,0x60,0x60,0x60,0x62,0x66,0xFE },{ 0x00,0x00,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6 },{ 0x00,0x00,0x00,0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6 },{ 0x00,0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38 },{ 0x00,0x00,0x00,0xFC,0x66,0x66,0x7C,0x60,0x60,0xF0 },{ 0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0xDC,0x78,0x1C },{ 0x00,0x00,0x00,0xFC,0x66,0x66,0x7C,0x6C,0x66,0xE6 },{ 0x00,0x00,0x00,0x78,0xCC,0xE0,0x70,0x1C,0xCC,0x78 },{ 0x00,0x00,0x00,0xFC,0xB4,0x30,0x30,0x30,0x30,0x78 },{ 0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xFC },{ 0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30 },{ 0x00,0x00,0x00,0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6 },{ 0x00,0x00,0x00,0xC6,0xC6,0x6C,0x38,0x38,0x6C,0xC6 },{ 0x00,0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x30,0x78 },{ 0x00,0x00,0x00,0xFE,0xC6,0x8C,0x18,0x32,0x66,0xFE },{ 0x00,0x00,0x00,0x78,0x60,0x60,0x60,0x60,0x60,0x78 },{ 0x00,0x00,0x00,0xC0,0x60,0x30,0x18,0x0C,0x06,0x02 },{ 0x00,0x00,0x00,0x78,0x18,0x18,0x18,0x18,0x18,0x78 },{ 0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x30,0x30,0x18,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,0xCC,0x76 },{ 0x00,0x00,0x00,0xE0,0x60,0x60,0x7C,0x66,0x66,0xDC },{ 0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xC0,0xCC,0x78 },{ 0x00,0x00,0x00,0x1C,0x0C,0x0C,0x7C,0xCC,0xCC,0x76 },{ 0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xFC,0xC0,0x78 },{ 0x00,0x00,0x00,0x38,0x6C,0x60,0xF0,0x60,0x60,0xF0 },{ 0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C },{ 0x00,0x00,0x00,0xE0,0x60,0x6C,0x76,0x66,0x66,0xE6 },{ 0x00,0x00,0x00,0x30,0x00,0x70,0x30,0x30,0x30,0x78 },{ 0x00,0x00,0x00,0x0C,0x00,0x0C,0x0C,0x0C,0xCC,0xCC },{ 0x00,0x00,0x00,0xE0,0x60,0x66,0x6C,0x78,0x6C,0xE6 },{ 0x00,0x00,0x00,0x70,0x30,0x30,0x30,0x30,0x30,0x78 },{ 0x00,0x00,0x00,0x00,0x00,0xCC,0xFE,0xFE,0xD6,0xC6 },{ 0x00,0x00,0x00,0x00,0x00,0xF8,0xCC,0xCC,0xCC,0xCC },{ 0x00,0x00,0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0x78 },{ 0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x7C,0x60 },{ 0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C },{ 0x00,0x00,0x00,0x00,0x00,0xDC,0x76,0x66,0x60,0xF0 },{ 0x00,0x00,0x00,0x00,0x00,0x7C,0xC0,0x78,0x0C,0xF8 },{ 0x00,0x00,0x00,0x10,0x30,0x7C,0x30,0x30,0x34,0x18 },{ 0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x76 },{ 0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30 },{ 0x00,0x00,0x00,0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x6C },{ 0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6 },{ 0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0x7C,0x0C },{ 0x00,0x00,0x00,0x00,0x00,0xFC,0x98,0x30,0x64,0xFC },{ 0x00,0x00,0x00,0x1C,0x30,0x30,0xE0,0x30,0x30,0x1C },{ 0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x18,0x18,0x18 },{ 0x00,0x00,0x00,0xE0,0x30,0x30,0x1C,0x30,0x30,0xE0 },{ 0x00,0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x00 },{ 0x00,0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE },{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}};

ISR(TIMER1_COMPA_vect)
{
  OCR1A += 865; // Advance The COMPA Register

  // Start of hsync trace 18.5khz - 5us
  PORTB |= (1 << PB1); // Turn on the pulse

  // Handle VSYNC - technically this pulse should overlap with the hsync but it makes little difference in this case
  if (row == 0) {
    PORTB &= ~(1 << PB0); // Start VSYNC (active low)
  }

  if (row == VSYNC_ROWS) {
    PORTB |= (1 << PB0);  // End VSYNC (inactive high)
  }

  // Delay for 5us (80 NOPs), this timing is very important.
  // It is the on time of the flyback, having it on for too long will damage it
  NopDelay<50>();
  PORTB &= ~(1 << PB1); // Turn off the pulse


  // Delay for row re-trace (has to be just right)
  // Note this delay could be replaced with some other slow action
  NopDelay<250>();


  bool drawtime = ((character_row > TEXT_ROW_START) & (character_row < TEXT_ROW_END));
  if (drawtime) {
    const char line = character_row - (TEXT_ROW_START + 1);
    const unsigned char* text_row = *(memory + line);

    for (char i = 0; i < TEXT_COLUMNS; i++) {
      const char char_i = *(text_row + i) - 32;
      const char* pixel_data = *(FONT + char_i);
      const char* offset = pixel_data + font_row;

      PORTD = *offset; 
      PORTB &= ~bitMask; // Set LOW
      PORTB |= bitMask; // Set HIGH 
    }
  }

  // font_row is 9 then we are at the end of the char line
  if (font_row > 8 || row > SCREEN_END) {
    character_row++;
    font_row = 0;
  } else {
    NopDelay<1>();
    font_row++;
  }

  if (character_row > TEXT_ROWS + 1 || row > SCREEN_END) {
    character_row = 0;
  } else {
    NopDelay<1>();
  }

  // Reset row counter
  if (row > SCREEN_END) {
    row = 0;
  } else {
    row++;
  }

  if (!drawtime) {

    // There are many rows that do not have data to be drawn
    // for each of those lines we have about 600 CPU cycles untill
    // the next line (next ISR), we are also at the end of the ISR
    // so all the house keeing tasks have been completed
    // so the time that remains is free for our use,
    // in cause !drawtime where !drawtime we sould have close to 600 CPU cycles
    // We can do many 600 cycles takses if we keep count as you see below

    // This uses 900 cycles over 3 ish frames and works well
    /* 
    if (nondraw_row == 0) {
      NopDelay<300>();
    }

    if (nondraw_row == 1) {
      NopDelay<300>();
    }

    if (nondraw_row == 3) {
      NopDelay<300>();
    }
    */

    // if (nondraw_row == 0) {
    //   NopDelay<600>();
    // } 
    // if (nondraw_row == 1) {
    //   NopDelay<300>();
    // }

    // NopDelay<10>();


    // This moves a character up and down the screen
    // It is done in steps, each step uses less then 600 cycles
    // despite this if the three tasks take different amounts of time
    // the image on the screen will oscillate horizontally
    // For some reason I can't figure out a variation in runtime caues jitter on the screen
    // I am certain that the ISR is completing within the required time
    if (nondraw_row == 0) {
      memory[dot_row][dot_column] = 96 + 32;
      memory[dot_row_old][dot_column_old] = ' ';
    }
    
    if (nondraw_row == 1) {
      // save the old position
      dot_row_old = dot_row;
      dot_column_old = dot_column;

      if (dot_row > 10){
        dot_row = 5;
      } else {
        dot_row++;
        NopDelay<1>();
      }
    }

    // No need for out counter to go all the way to 255
    if (nondraw_row > 60) {
      nondraw_row = 0;
    } else {
      nondraw_row++;
      NopDelay<1>();
    }
  }

}

void setup()
{
  cli();
  noInterrupts(); // Disable all interrupts

  pinMode(8, OUTPUT);   // Vsync (PB0)
  pinMode(9, OUTPUT);   // Hsync (Timer1 - Port B) 
  pinMode(10, OUTPUT);  // Data (PB2)
  pinMode(11, OUTPUT);  // DOT Clock (Timer2 - Port B)
  pinMode(12, OUTPUT);  // parallel load (PB4) (74LS165)

  // Set parallel load LOW (Parallel Entry)
  PORTB &= ~(1 << PB4);

  // Set all port D pins to output
  DDRD = B11111111;

  // Disable Timer0
  TCCR0B = 0; 
  TIMSK0 &= ~(1 << TOIE0);  // Disable Timer0 overflow interrupt

  // Disable Timer2
  TCCR2B = 0;
  TIMSK2 &= ~(1 << TOIE2);  // Disable Timer2 overflow interrupt

  TCCR1A = 0;           // Init Timer1A
  TCCR1B = 0;           // Init Timer1B
  TCCR1B |= B00000001;  // Prescaler = 1
  OCR1A = 865;          // Timer Compare1A Register
  TIMSK1 |= B00000010;  // Enable Timer COMPA Interrupt

  // Dot clock Timer2
  TCCR2A = _BV(COM2A0) | _BV(WGM21) | _BV(WGM20); // Fast PWM, toggle OC2A
  TCCR2B = _BV(WGM22) | _BV(CS20);  // No prescaler, timer running at 16 MHz
  OCR2A = 0; // 4mhz (16 / (1+1))

  interrupts();   // Re-enable interrupts
  sei();
  while (true) {}
}

void loop()
{
  // Do Nothing
}