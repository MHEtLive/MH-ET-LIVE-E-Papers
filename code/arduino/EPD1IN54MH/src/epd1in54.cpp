#include <stdlib.h>
#include "epd1in54.h"

Epd::Epd()
{
    reset_pin = RST_PIN;
    dc_pin    = DC_PIN;
    cs_pin    = CS_PIN;
    busy_pin  = BUSY_PIN;
    width     = EPD_WIDTH;
    height    = EPD_HEIGHT;
};

Epd::~Epd() {};

unsigned char Epd::Init(const unsigned char* lut)
{
    if (IfInit() != 0)
    {
        return -1;
    }

    this->lut = lut;
    Reset();
    SendCommand(DRIVER_OUTPUT_CONTROL);
    SendData((EPD_HEIGHT - 1));
    SendData((EPD_HEIGHT - 1) >> 8);
    SendData(0x00);
    SendCommand(BOOSTER_SOFT_START_CONTROL);
    SendData(0xD7);
    SendData(0xD6);
    SendData(0x9D);
    SendCommand(WRITE_VCOM_REGISTER);
    SendData(0xA8);
    SendCommand(SET_DUMMY_LINE_PERIOD);
    SendData(0x1A);
    SendCommand(SET_GATE_LINE_WIDTH);
    SendData(0x08);
    SendCommand(DATA_ENTRY_MODE_SETTING);
    SendData(0x03);
    SetLut(this->lut);

    return 0;
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          You can use Epd::Init() to awaken
 */
void Epd::Sleep() {
    SendCommand(DEEP_SLEEP_MODE);
    WaitUntilIdle();
}

/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void Epd::WaitUntilIdle(void)
{
    while(digitalRead(busy_pin) == HIGH) {      //LOW: idle, HIGH: busy
        delay(100);
    }      
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
    digitalWrite(reset_pin, LOW);                //module reset    
    delay(200);
    digitalWrite(reset_pin, HIGH);
    delay(200);    
}

/**
 *  @brief: set the look-up table register
 */
void Epd::SetLut(const unsigned char* lut)
{
    this->lut = lut;
    SendCommand(WRITE_LUT_REGISTER);
    /* the length of look-up table is 30 bytes */
    for (int i = 0; i < 30; i++) {
        SendData(this->lut[i]);
    }
}

/**
 *  @brief: put an image buffer to the Pattern memory.
 *          this won't update the display.
 */
void Epd::SetPatternMemory(
    const unsigned char* image_buffer,
    int x, int y,
    int image_width, int image_height)
{
    int x_end;
    int y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= this->width) {
        x_end = this->width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= this->height) {
        y_end = this->height - 1;
    } else {
        y_end = y + image_height - 1;
    }
    SetMemoryArea(x, y, x_end, y_end);
    SetMemoryPointer(x, y);
    SendCommand(WRITE_RAM);
    /* send the image data */
    for (int j = 0; j < y_end - y + 1; j++) {
        for (int i = 0; i < (x_end - x + 1) / 8; i++) {
            SendData(image_buffer[i + j * (image_width / 8)]);
        }
    }
}

/**
 *  @brief: put an image buffer to the Pattern memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of 
 *          void SetPatternMemory(
 *              const unsigned char* image_buffer,
 *              int x,  int y,
 *              int image_width, int image_height );
 *          Answer: SetPatternMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers 
 *          from the flash).
 */
void Epd::SetPatternMemory(const unsigned char* image_buffer)
{
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(WRITE_RAM);
    /* send the image data */
    for (int i = 0; i < this->width / 8 * this->height; i++)
    {
        SendData(~(pgm_read_byte(&image_buffer[i])));
    }
}

/**
 *  @brief: clear the Pattern memory with the specified color.
 *          this won't update the display.
 */
void Epd::ClearPatternMemory(unsigned char color)
{
    SetMemoryArea(0, 0, this->width - 1, this->height - 1);
    SetMemoryPointer(0, 0);
    SendCommand(WRITE_RAM);
    /* send the color data */
    for (int i = 0; i < this->width / 8 * this->height; i++)
    {
        SendData(color);
    }
}

/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetPatternMemory or ClearPattern will 
 *          set the other memory area.
 */
void Epd::DisplayPattern(void)
{
    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xC4);
    SendCommand(MASTER_ACTIVATION);
    SendCommand(NOP);
    WaitUntilIdle();
}

/**
 *  @brief: private function to specify the memory area for data R/W
 */
void Epd::SetMemoryArea(int x_start, int y_start,
                        int x_end, int y_end)
{
    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x_start >> 3) & 0xFF);
    SendData((x_end >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    SendData(y_start & 0xFF);
    SendData((y_start >> 8) & 0xFF);
    SendData(y_end & 0xFF);
    SendData((y_end >> 8) & 0xFF);
}

/**
 *  @brief: private function to specify the start point for data R/W
 */
void Epd::SetMemoryPointer(int x, int y)
{
    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    SendData((x >> 3) & 0xFF);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(y & 0xFF);
    SendData((y >> 8) & 0xFF);
    WaitUntilIdle();
}


/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
    digitalWrite(dc_pin, LOW);
    Transfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data)
{
    digitalWrite(dc_pin, HIGH);
    Transfer(data);
}

const unsigned char lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
