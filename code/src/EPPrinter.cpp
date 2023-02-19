#include "Arduino.h"
#include "EPPrinter.h"

/* Much of this is derived from the Adafruit's thermal printer
 * library. Entirely re-written, but I used their project for
 * guidance.
 * https://github.com/adafruit/Adafruit-Thermal-Printer-Library
 */

uint8_t widthInChars;

EPPrinter::EPPrinter(Stream *s) :
    stream(s) {
}

void EPPrinter::initialize() {
    writeDefaults();
}

void EPPrinter::writeDefaults() {
    setSize(0);
}

void EPPrinter::writeCommand(uint8_t a) {
    stream->write(a);
}

void EPPrinter::writeCommand(uint8_t a, uint8_t b) {
    stream->write(a);
    stream->write(b);
}

void EPPrinter::writeCommand(uint8_t a, uint8_t b, uint8_t c) {
    stream->write(a);
    stream->write(b);
    stream->write(c);
}

void EPPrinter::writeCommand(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    stream->write(a);
    stream->write(b);
    stream->write(c);
    stream->write(d);
}

size_t EPPrinter::write(uint8_t character) {
    stream->write(character);
    return 1;
}

// ---- Font Options ---- //
void EPPrinter::setBold(uint8_t value) {
    value = (bool) value;
    writeCommand(char_esc, 'E', value);
}

void EPPrinter::setUnderline(uint8_t value) {
    // 0-2, no underline to bold underline
    writeCommand(char_esc, '-', value);
}

void EPPrinter::setStrikethrough(uint8_t value) {
    value = (bool) value;
    writeCommand(char_esc, 'G', value);
}

void EPPrinter::setSize(uint8_t value) {
    // 0-2 are valid. Small Medium Large
    char size;

    switch(value) {
        case 2:
            size = 0x11;
            widthInChars = 21;
            break;
        case 1:
            size = 0x01;
            widthInChars = 21;
            break;
        default: // case 0:
            size = 0x00;
            widthInChars = 42;
            break;
    }

    writeCommand(char_gs, '!', size);
}

void EPPrinter::setJustify(uint8_t value) {
    //accepts 0-2, left, center, and right justify
    writeCommand(char_esc, 'a', value);
}

void EPPrinter::setLineHeight(uint8_t value) {
    // setting to 0 will set default
    if (value > 0) {
        writeCommand(char_esc, '3', value);
    } else {
        writeCommand(char_esc, '2');
    }
}

// ---- Printer Status ---- //
uint8_t EPPrinter::getPaperWidthInChars() {
    return widthInChars;
}

// ---- Printer Commands ---- //
void EPPrinter::feed(uint8_t value) {
    writeCommand(char_esc, 'd', value);
}

void EPPrinter::cut() {
    print(char_lf);
    writeCommand(char_gs, 'V', 0);
}


// so there's a write function. Not sure if I need it?
