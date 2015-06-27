/*
 * AVRGPIB
 * (c) 2015 Fredrik Ahlberg <fredrik@z80.se>
 */

#ifndef PINS_H
#define PINS_H

/* Helper macros */

#define BIT0	_BV(0)
#define BIT1	_BV(1)
#define BIT2	_BV(2)
#define BIT3	_BV(3)
#define BIT4	_BV(4)
#define BIT5	_BV(5)
#define BIT6	_BV(6)
#define BIT7	_BV(7)

/* GPIO to GPIB pin configuration */

#define DATA_LOW_PORT	PORTB
#define DATA_LOW_PIN	PINB
#define DATA_LOW_DDR	DDRB
#define DATA_LOW_BITS	(BIT0|BIT1|BIT2|BIT3|BIT4|BIT5)

#define DATA_HIGH_PORT	PORTD
#define DATA_HIGH_PIN	PIND
#define DATA_HIGH_DDR	DDRD
#define DATA_HIGH_BITS	(BIT6|BIT7)

#define EOI_PORT	PORTD
#define EOI_PIN		PIND
#define EOI_DDR		DDRD
#define EOI_BIT		BIT5

#define DAV_PORT	PORTD
#define DAV_PIN		PIND
#define DAV_DDR		DDRD
#define DAV_BIT		BIT3

#define NRFD_PORT	PORTC
#define NRFD_PIN	PINC
#define NRFD_DDR	DDRC
#define NRFD_BIT	BIT0

#define NDAC_PORT	PORTC
#define NDAC_PIN	PINC
#define NDAC_DDR	DDRC
#define NDAC_BIT	BIT1

#define IFC_PORT	PORTC
#define IFC_PIN		PINC
#define IFC_DDR		DDRC
#define IFC_BIT		BIT2

#define SRQ_PORT	PORTC
#define SRQ_PIN		PINC
#define SRQ_DDR		DDRC
#define SRQ_BIT		BIT3

#define ATN_PORT	PORTC
#define ATN_PIN		PINC
#define ATN_DDR		DDRC
#define ATN_BIT		BIT4

#define REN_PORT	PORTC
#define REN_PIN		PINC
#define REN_DDR		DDRC
#define REN_BIT		BIT5

#endif
