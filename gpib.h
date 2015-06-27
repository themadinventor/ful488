/*
 * Ful488
 * (c) 2015 Fredrik Ahlberg <fredrik@z80.se>
 */

#ifndef GPIB_H
#define GPIB_H

#define GPIB_CMD_GTL	0x01	/* Go To Local */
#define GPIB_CMD_LLO	0x11	/* Local Lock Out */
#define GPIB_CMD_DCL	0x14	/* Device Clear */
#define GPIB_PAD_MASK	0x1f
#define GPIB_CMD_MLA	0x20	/* My Listen Address */
#define GPIB_CMD_MTA	0x40	/* My Talk Address */
#define GPIB_CMD_UNT	0x5f	/* Untalk */
#define GPIB_CMD_UNL	0x3f	/* Unlisten */
#define GPIB_CMD_MSA	0x60	/* My Secondary Address */

uint8_t	gpib_write(uint8_t *buf, uint16_t len, uint8_t atn, uint8_t eoi);
uint8_t	gpib_command(uint8_t cmd);
uint8_t	gpib_read(uint8_t *buf);
void	gpib_init(void);
void	gpib_ren(uint8_t enable);
void	gpib_ifc(void);
uint8_t	gpib_srq(void);

#endif
