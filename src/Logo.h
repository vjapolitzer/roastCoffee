#ifndef LOGO_H
#define LOGO_H

#include <Arduino.h>

#define LOGO_WIDTH 87
#define LOGO_HEIGHT 30
static const uint8_t logo_bits[] PROGMEM =
{
    0x80, 0x1F, 0x03, 0x00, 0xC0, 0x8F, 0x01, 0x00, 0xE0, 0xC7, 0x00, 0x60, 
    0x20, 0x05, 0x00, 0x70, 0x95, 0x03, 0x00, 0xF8, 0xCF, 0x01, 0x10, 0x20, 
    0x19, 0x00, 0x98, 0x9A, 0x0C, 0x00, 0xFC, 0xCF, 0x07, 0x08, 0x20, 0x21, 
    0x00, 0xB4, 0x94, 0x1B, 0x00, 0xFE, 0xCF, 0x0F, 0x04, 0x40, 0x22, 0x00, 
    0xA6, 0x35, 0x19, 0x00, 0xFF, 0x9F, 0x0F, 0x04, 0x40, 0x42, 0x00, 0x2A, 
    0x35, 0x23, 0x00, 0xFF, 0x9F, 0x1F, 0x02, 0x40, 0x42, 0x00, 0xD5, 0x24, 
    0x3D, 0x80, 0xFF, 0x9F, 0x1F, 0x02, 0x40, 0x82, 0x00, 0x2B, 0x2B, 0x63, 
    0x80, 0xFF, 0x9F, 0x3F, 0x02, 0x40, 0x82, 0x00, 0xD5, 0x36, 0x57, 0x80, 
    0xFF, 0x9F, 0x3F, 0x01, 0x40, 0x82, 0x80, 0x35, 0x29, 0x55, 0xC0, 0xFF, 
    0x9F, 0x3F, 0x01, 0x20, 0x82, 0x80, 0x44, 0x15, 0x69, 0xC0, 0xFF, 0x8F, 
    0x3F, 0x01, 0x20, 0x01, 0x81, 0xB5, 0x9A, 0xD6, 0xC0, 0xFF, 0xCF, 0x7F, 
    0x01, 0x20, 0x01, 0x81, 0xA5, 0x9A, 0xA9, 0xC0, 0xFF, 0xCF, 0x7F, 0x01, 
    0x90, 0x00, 0x81, 0x9A, 0xCA, 0xAA, 0xC0, 0xFF, 0xE7, 0x7F, 0x01, 0x90, 
    0x00, 0x81, 0x65, 0x4D, 0xD5, 0xC0, 0xFF, 0xE7, 0x7F, 0x01, 0x48, 0x00, 
    0x81, 0x4D, 0x65, 0xA9, 0xC0, 0xFF, 0xF3, 0x7F, 0x01, 0x48, 0x00, 0x81, 
    0xAA, 0xA6, 0xAA, 0xC0, 0xFF, 0xF3, 0x7F, 0x01, 0x24, 0x00, 0x81, 0x49, 
    0xB3, 0xEA, 0xC0, 0xFF, 0xF9, 0x7F, 0x01, 0x24, 0x00, 0x81, 0xB6, 0x52, 
    0x95, 0xC0, 0xFF, 0xF9, 0x7F, 0x01, 0x12, 0x00, 0x81, 0x49, 0xA9, 0xEA, 
    0xC0, 0xFF, 0xFC, 0x7F, 0x01, 0x12, 0x80, 0x80, 0x55, 0xA9, 0x4A, 0xC0, 
    0xFF, 0xFC, 0x3F, 0x02, 0x11, 0x80, 0x00, 0xAB, 0x58, 0x75, 0x80, 0x7F, 
    0xFC, 0x3F, 0x02, 0x09, 0x80, 0x00, 0xD5, 0xA4, 0x4A, 0x80, 0x7F, 0xFE, 
    0x3F, 0x02, 0x09, 0x40, 0x00, 0xD5, 0x5C, 0x35, 0x80, 0x7F, 0xFE, 0x1F, 
    0x04, 0x09, 0x40, 0x00, 0x96, 0x44, 0x25, 0x00, 0x7F, 0xFE, 0x1F, 0x04, 
    0x09, 0x40, 0x00, 0xEA, 0xBC, 0x3A, 0x00, 0x7F, 0xFE, 0x1F, 0x08, 0x09, 
    0x20, 0x00, 0x94, 0xA4, 0x14, 0x00, 0x7E, 0xFE, 0x0F, 0x10, 0x09, 0x10, 
    0x00, 0xE8, 0xAC, 0x0A, 0x00, 0x7C, 0xFE, 0x07, 0x60, 0x09, 0x0C, 0x00, 
    0xB0, 0x54, 0x07, 0x00, 0x78, 0xFE, 0x03, 0x80, 0xF1, 0x03, 0x00, 0xC0, 
    0xF8, 0x01, 0x00, 0x60, 0xFC, 0x00
};

#endif