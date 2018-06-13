#ifndef _EPDIF_H
#define _EPDIF_H

#include <arduino.h>

// EPD Pin definition
#define RST_PIN         8
#define DC_PIN          9
#define CS_PIN          10
#define BUSY_PIN        7

class EpdIf
{
  public:
    EpdIf(void);
    ~EpdIf(void);

    unsigned char IfInit(void);
    void Transfer(unsigned char data);
};

#endif
