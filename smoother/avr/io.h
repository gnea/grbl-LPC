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
extern DummyReg OCIE0A;
extern DummyReg OCIE0B;
extern DummyReg OCIE1A;
extern DummyReg OCR1A;
extern DummyReg PCICR;
extern DummyReg PCIE0;
extern DummyReg PCIE1;
extern DummyReg PCMSK0;
extern DummyReg PCMSK1;
extern DummyReg SREG;
extern DummyReg TCCR0A;
extern DummyReg TCCR0B;
extern DummyReg TCCR0B;
extern DummyReg TCCR1A;
extern DummyReg TCCR1B;
extern DummyReg TCNT0;
extern DummyReg TIMSK0;
extern DummyReg TIMSK1;
extern DummyReg TOIE0;

static const int COM1A0 = 0;
static const int COM1A1 = 0;
static const int COM1B0 = 0;
static const int COM1B1 = 0;
static const int CS01 = 0;
static const int CS10 = 0;
static const int CS11 = 0;
static const int CS12 = 0;
static const int EEMWE = 0;
static const int EERE = 0;
static const int EEWE = 0;
static const int WGM10 = 0;
static const int WGM11 = 0;
static const int WGM12 = 0;
static const int WGM13 = 0;
