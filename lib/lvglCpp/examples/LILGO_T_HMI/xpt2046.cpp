

#include "xpt2046.h"

#define Z_THRESHOLD     0
#define Z_THRESHOLD_INT 75

#define SPI_SETTING SPISettings(2000000, MSBFIRST, SPI_MODE0)

/* Constructor to initialise the GPIO */
XPT2046::XPT2046(SPIClass &spi, byte cs, uint8_t tirq) {
    _spi = &spi;
    _cs = cs;
    _tirq = tirq;

    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);

    _hmin = 0;
    _hmax = 4095;
    _vmin = 0;
    _vmax = 4095;
    _hres = 320;
    _vres = 240;
    _hmin_logic = _hmin;
    _hmax_logic = _hmax;
    _vmin_logic = _vmin;
    _vmax_logic = _vmax;
    _hres_logic = _hres;
    _vres_logic = _vres;
}


static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
    int16_t da, db, dc;
    int16_t reta = 0;
    if ( x > y ) {
        da = x - y;
    } else {
        da = y - x;
    }
    if ( x > z ) {
        db = x - z;
    } else {
        db = z - x;
    }
    if ( z > y ) {
        dc = z - y;
    } else {
        dc = y - z;
    }

    if ( da <= db && da <= dc ) {
        reta = (x + y) >> 1;
    } else if ( db <= da && db <= dc ) {
        reta = (x + z) >> 1;
    } else {
        reta = (y + z) >> 1;
    }

    return (reta);
}


/* Detects if the touch screen is currently pressed. Returns TRUE if pressed */
boolean XPT2046::pressed(void)
{
    int16_t data[6];

    if (digitalRead(_tirq) == HIGH) {
        return false;
    }

    if (_spi) {
        _spi->beginTransaction(SPI_SETTING);
        digitalWrite(_cs, LOW);
        int16_t z1 = _spi->transfer16(0xB0) >> 4; // Z1
        int16_t z2 = _spi->transfer16(0xC0) >> 4; //Z2
        _zraw = abs(z1) - abs(z2);
         //Serial.print("z1: ");
         //Serial.print(z1);
         //Serial.print(" z2: ");
         //Serial.print(z2);
         //Serial.print(" _zraw: ");
         //Serial.println(_zraw);
        // if (abs(_zraw) >= Z_THRESHOLD) {
            // make 3 x-y measurements
            data[0] = _spi->transfer16(0x90) >> 4; // X
            data[1] = _spi->transfer16(0xD0) >> 4; // Y
            data[2] = _spi->transfer16(0x90) >> 4; // X
            data[3] = _spi->transfer16(0xD0) >> 4; // Y
            data[4] = _spi->transfer16(0x90) >> 4; // X
            data[5] = _spi->transfer16(0xD0) >> 4; // Last Y touch power down
        // } else {
        //     data[0] = data[1] = data[2] = data[3] = 0;
        // }
        _spi->transfer16(0);
        digitalWrite(_cs, HIGH);
        _spi->endTransaction();
    } else {
        return false;
    }

    // if (abs(_zraw) >= Z_THRESHOLD) {
        _xraw = besttwoavg(data[0], data[2], data[4]);
        _yraw = besttwoavg(data[1], data[3], data[5]);
        if (_rotation == 0 || _rotation == 2) {
            _xcoord = constrain(map(_xraw, _hmin_logic, _hmax_logic, 0, _hres_logic), 0, _hres_logic);
            _ycoord = constrain(map(_yraw, _vmin_logic, _vmax_logic, 0, _vres_logic), 0, _vres_logic);
        } else {
            _xcoord = constrain(map(_yraw, _hmin_logic, _hmax_logic, 0, _hres_logic), 0, _hres_logic);
            _ycoord = constrain(map(_xraw, _vmin_logic, _vmax_logic, 0, _vres_logic), 0, _vres_logic);
        }

    // } else {
    //     return false;
    // }

    return true;
}


/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t XPT2046::RawX(void)
{
    return _xraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t XPT2046::RawY(void)
{
    return _yraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t XPT2046::X(void)
{
    return _xcoord;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t XPT2046::Y(void)
{
    return _ycoord;
}


/* Set the screen resolution in pixels. */
void XPT2046::begin(uint16_t hres, uint16_t vres)
{
    _hres = hres;
    _vres = vres;
    _hres_logic = _hres;
    _vres_logic = _vres;
}

/* Set the screen calibration values */
void XPT2046::setCal(uint16_t hmin, uint16_t hmax,
                       uint16_t vmin, uint16_t vmax,
                       uint16_t hres, uint16_t vres
                       )
{
    _hmin = hmin;
    _hmax = hmax;
    _vmin = vmin;
    _vmax = vmax;
    _hres = hres;
    _vres = vres;
    _hmin_logic = _hmin;
    _hmax_logic = _hmax;
    _vmin_logic = _vmin;
    _vmax_logic = _vmax;
    _hres_logic = _hres;
    _vres_logic = _vres;
}


void XPT2046::setRotation(byte rotation) {
    switch(rotation % 4) {
        case 0:
            _hmin_logic = _hmin;
            _hmax_logic = _hmax;
            _vmin_logic = _vmin;
            _vmax_logic = _vmax;
            _hres_logic = _hres;
            _vres_logic = _vres;
        break;
        case 1:
            _hmin_logic = _vmin;
            _hmax_logic = _vmax;
            _vmin_logic = _hmax;
            _vmax_logic = _hmin;
            _hres_logic = _vres;
            _vres_logic = _hres;
        break;
        case 2:
            _hmin_logic = _vmax;
            _hmax_logic = _vmin;
            _vmin_logic = _hmax;
            _vmax_logic = _hmin;
            _hres_logic = _hres;
            _vres_logic = _vres;
        break;
        case 3:
            _hmin_logic = _vmax;
            _hmax_logic = _vmin;
            _vmin_logic = _hmin;
            _vmax_logic = _hmax;
            _hres_logic = _vres;
            _vres_logic = _hres;
        break;
    }
    _rotation = rotation % 4;
}
