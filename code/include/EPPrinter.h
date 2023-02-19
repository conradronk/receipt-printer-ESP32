#ifndef EPPrinter_h
#define EPPrinter_h

#include "Arduino.h"

#include "escposcodes.h"


#define baud_rate 9600

class EPPrinter : public Print {
    public:
        size_t write(uint8_t character);
        EPPrinter(Stream *s = &Serial);
        void initialize();
        void writeDefaults();
        void setBold(uint8_t value);
        void setUnderline(uint8_t value);
        void setStrikethrough(uint8_t value);
        void setSize(uint8_t value);
        void setJustify(uint8_t value);
        void setLineHeight(uint8_t value);
        uint8_t getPaperWidthInChars();
        void feed(uint8_t value);
        void cut();
    private:
        uint8_t widthInChars;
        Stream *stream;
        void writeCommand(uint8_t a);
        void writeCommand(uint8_t a, uint8_t b);
        void writeCommand(uint8_t a, uint8_t b, uint8_t c);
        void writeCommand(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
};

#endif