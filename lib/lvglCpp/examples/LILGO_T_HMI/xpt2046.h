#ifndef _XPT2046_H
#define _XPT2046_H


#include <SPI.h>
#include <Arduino.h>

#define _RAWERR 10

class XPT2046 {
public:
    XPT2046(SPIClass &spi, byte cs, uint8_t tirq);
    void begin(uint16_t xres, uint16_t yres);
    boolean pressed(void);
    uint16_t RawX(void);
    uint16_t RawY(void);
    uint16_t X(void);
    uint16_t Y(void);

    void setCal(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax, uint16_t xres, uint16_t yres);
    void setRotation(byte rotation);

    uint16_t _hmin_logic, _hmax_logic, _vmin_logic, _vmax_logic, _hres_logic, _vres_logic;

private:
    SPIClass *_spi = nullptr;
    byte _cs;
    byte _tirq;
    int16_t _xraw, _yraw, _zraw, _xcoord, _ycoord;
    uint16_t _hmin, _hmax, _vmin, _vmax, _hres, _vres;
    uint8_t _rotation = 0;
};

#endif
