/*
 * Ful488
 * (c) 2015 Fredrik Ahlberg <fredrik@z80.se>
 */

#include <avr/io.h>
#include <util/delay.h>
#include "pins.h"

static inline void set_dio(uint8_t data)
{
	/* GPIB uses negative logic, so flip dem bits */
	data = ~data;

	/* Write data to pins */
	DATA_LOW_PORT &= ~DATA_LOW_BITS;
	DATA_LOW_PORT |= data & DATA_LOW_BITS;

	DATA_HIGH_PORT &= ~DATA_HIGH_BITS;
	DATA_HIGH_PORT |= data & DATA_HIGH_BITS;

	/* Set pins as outputs */
	DATA_LOW_DDR |= DATA_LOW_BITS;
	DATA_HIGH_DDR |= DATA_HIGH_BITS;
}

static inline void release_dio(void)
{
	/* Set pins as inputs */
	DATA_LOW_DDR &= ~DATA_LOW_BITS;
	DATA_HIGH_DDR &= ~DATA_HIGH_BITS;

	/* Enable pull-ups */
	DATA_LOW_PORT |= DATA_LOW_BITS;
	DATA_HIGH_PORT |= DATA_HIGH_BITS;
}

static inline uint8_t read_dio(void)
{
	return ~((DATA_LOW_PIN & DATA_LOW_BITS) |
		(DATA_HIGH_PIN & DATA_HIGH_BITS));
}

uint8_t gpib_write(uint8_t *buf, uint16_t len, uint8_t atn, uint8_t eoi)
{
	/* Assert ATN if requested */
	if (atn) {
		ATN_PORT &= ~ATN_BIT;
	}
	ATN_DDR |= ATN_BIT;

	/* Release NRFD & NDAC */
	NRFD_PORT |= NRFD_BIT;
	NRFD_DDR &= ~NRFD_BIT;
	NDAC_PORT |= NDAC_BIT;
	NDAC_DDR &= ~NDAC_BIT;

	/* Unassert DAV */
	DAV_PORT |= DAV_BIT;
	DAV_DDR |= DAV_BIT;

	while (len--) {
		/* Are both NRFD and NDAC unasserted? */
		if ((NRFD_PIN & NRFD_BIT) && (NDAC_PIN & NDAC_BIT)) {
			/* That's an error condition, sir. */
			ATN_PORT |= ATN_BIT;
			ATN_DDR &= ~ATN_BIT;
			DAV_DDR &= ~DAV_BIT;
			return 1;
		}

		/* Put data on bus */
		set_dio(*buf++);

		if (eoi && !len) {
			/* Assert EOI */
			EOI_PORT &= ~EOI_BIT;
			EOI_DDR |= EOI_BIT;
		}

		/* Wait for RFD */
		while (!(NRFD_PIN & NRFD_BIT)) ;

		/* Assert DAV */
		DAV_PORT &= ~DAV_BIT;

		/* Wait for NDAC */
		while (!(NDAC_PIN & NDAC_BIT)) ;

		/* Deassert DAV */
		DAV_PORT |= DAV_BIT;
	}

	/* Assert NRFD & NDAC */
	NRFD_PORT &= ~NRFD_BIT;
	NRFD_DDR |= NRFD_BIT;
	NDAC_PORT &= ~NDAC_BIT;
	NDAC_DDR |= NDAC_BIT;

	/* Release data lines */
	release_dio();
	
	/* Release DAV */
	DAV_DDR &= ~DAV_BIT;

	/* Deassert EOI */
	EOI_PORT |= EOI_BIT;
	EOI_DDR &= ~EOI_BIT;

	/* Deassert ATN */
	ATN_PORT |= ATN_BIT;
	ATN_DDR &= ~ATN_BIT;

	return 0;
}

uint8_t gpib_command(uint8_t cmd)
{
	return gpib_write(&cmd, 1, 1, 0);
}

uint8_t gpib_read(uint8_t *buf)
{
	uint8_t eoi;

	/* Unassert NRFD */
	NRFD_PORT |= NRFD_BIT;
	NRFD_DDR &= ~NRFD_BIT;

	/* Wait for DAV */
	while (DAV_PIN & DAV_BIT) ;

	/* Assert NRFD */
	NRFD_PORT &= ~NRFD_BIT;
	NRFD_DDR |= NRFD_BIT;

	/* Capture data lines and EOI */
	*buf = read_dio();
	eoi = (EOI_PIN & EOI_BIT) == 0;

	/* Unassert NDAC */
	NDAC_PORT |= NDAC_BIT;
	NDAC_DDR &= ~NDAC_BIT;

	/* Wait for DAV */
	while (!(DAV_PIN & DAV_BIT)) ;

	/* Assert NDAC */
	NDAC_PORT &= ~NDAC_BIT;
	NDAC_DDR |= NDAC_BIT;

	return eoi;
}

void gpib_init(void)
{
	release_dio();

	ATN_PORT |= ATN_BIT;

	NRFD_PORT &= ~NRFD_BIT;
	NRFD_DDR |= NRFD_BIT;

	NDAC_PORT &= ~NDAC_BIT;
	NDAC_DDR |= NDAC_BIT;

	IFC_PORT |= IFC_BIT;
	
	SRQ_PORT |= SRQ_BIT;
}

void gpib_ren(uint8_t enable)
{
	if (enable) {
		REN_PORT &= ~REN_BIT;
		REN_DDR |= REN_BIT;
	} else {
		REN_PORT |= REN_BIT;
		REN_DDR &= ~REN_BIT;
	}
}

void gpib_ifc(void)
{
	IFC_PORT &= ~IFC_BIT;
	IFC_DDR |= IFC_BIT;

	_delay_us(100);

	IFC_PORT |= IFC_BIT;
	IFC_DDR &= ~IFC_BIT;

	_delay_us(100);
}

uint8_t gpib_srq(void)
{
	return !(SRQ_PIN & SRQ_BIT);
}

