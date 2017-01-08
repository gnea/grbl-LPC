#pragma once
#include <stdint.h>

struct DummyReg {
    operator uint8_t() {return 0;}
    uint8_t operator=(uint8_t x) {return x;}
    uint8_t operator&=(uint8_t x) {return *this = *this & x;}
    uint8_t operator|=(uint8_t x) {return *this = *this | x;}
};

extern DummyReg DDRA;
extern DummyReg DDRB;
extern DummyReg DDRC;
extern DummyReg DDRD;

extern DummyReg PORTA;
extern DummyReg PORTB;
extern DummyReg PORTC;
extern DummyReg PORTD;

extern DummyReg PINA;
extern DummyReg PINB;
extern DummyReg PINC;
extern DummyReg PIND;

extern DummyReg EEAR;
extern DummyReg EECR;
extern DummyReg EEDR;
extern DummyReg EEMPE;
extern DummyReg EEPE;
extern DummyReg OCR2A;
extern DummyReg PCICR;
extern DummyReg PCIE0;
extern DummyReg PCIE1;
extern DummyReg PCMSK0;
extern DummyReg PCMSK1;
extern DummyReg TCCR2A;
extern DummyReg TCCR2B;

extern DummyReg UCSR0A;
extern DummyReg UCSR0B;
extern DummyReg U2X0;
extern DummyReg UBRR0H;
extern DummyReg UBRR0L;
extern DummyReg UDR0;


static const int COM1A0 = 0;
static const int COM1A1 = 0;
static const int COM1B0 = 0;
static const int COM1B1 = 0;
static const int COM2A0 = 0;
static const int COM2A1 = 0;
static const int COM2B0 = 0;
static const int COM2B1 = 0;

 static const int CS22 = 0;
static const int EEMWE = 0;
static const int EERE = 0;
static const int EEWE = 0;
static const int WGM10 = 0;
static const int WGM11 = 0;
static const int WGM12 = 0;
static const int WGM13 = 0;
static const int WGM20 = 0;
static const int WGM21 = 0;

static const int RXEN0 = 0;
static const int TXEN0 = 0;
static const int RXCIE0 = 0;
static const int UDRIE0 = 0;
