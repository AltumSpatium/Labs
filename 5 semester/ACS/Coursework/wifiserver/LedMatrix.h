#ifndef ledmatrix_header
#define ledmatrix_header

//**************************************************************************************************

#include "arduino.h"

//**************************************************************************************************

#define LEDMATRIX_CMD_NOOP         0x00
#define LEDMATRIX_CMD_DIGIT0       0x01
#define LEDMATRIX_CMD_DIGIT1       0x02
#define LEDMATRIX_CMD_DIGIT2       0x03
#define LEDMATRIX_CMD_DIGIT3       0x04
#define LEDMATRIX_CMD_DIGIT4       0x05
#define LEDMATRIX_CMD_DIGIT5       0x06
#define LEDMATRIX_CMD_DIGIT6       0x07
#define LEDMATRIX_CMD_DIGIT7       0x08
#define LEDMATRIX_CMD_DECODEMODE   0x09
#define LEDMATRIX_CMD_INTENSITY    0x0A
#define LEDMATRIX_CMD_SCANLIMIT    0x0B
#define LEDMATRIX_CMD_SHUTDOWN     0x0C
#define LEDMATRIX_CMD_DISPLAYTEST  0x0F

//..................................................................................................

#define LEDMATRIX_FONT_WIDTH   5
#define LEDMATRIX_FONT_HEIGHT  7

//**************************************************************************************************

class LedMatrix
{
  //................................................................................................

private:

  static const uint8_t PROGMEM font[];

  uint8_t pin_cs;
  uint8_t pin_din;
  uint8_t pin_clk;
  uint8_t mod_cnt;
  uint8_t uselock;
  uint8_t* bitmap;

  //................................................................................................

protected:

  void SendCmd(uint8_t _command, uint8_t _argument);
  void SendLine(uint8_t _line);
  void SendBitmap(void);

  //................................................................................................

public:

  LedMatrix(uint8_t _cs, uint8_t, _din, uint8_t _clk, uint8_t _cnt);
  uint16_t Width(void);
  uint16_t Height(void);
  void UpdateLock(uint8_t _uselock);
  void Fill(uint8_t _color);
  void Scroll(int8_t _direction, int16_t _size);
  void DrawPixel(int16_t _left, int16_t _top, uint8_t _color);
  void DrawChar(int16_t _left, int16_t _top, uint8_t _chr);
  void DrawString(int16_t _left, int16_t _top, uint8_t* _string);

  //................................................................................................
};

//**************************************************************************************************

#endif

//**************************************************************************************************
