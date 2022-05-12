#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

// Inspirasjon her er hentet fra AVR databladet om TWI eller Two Wire Interface
// De hadde ogs� drivere som gjorde det
// Akkurat denne koden er inspirert  av embedded Castle sin instruksjon p� hvordan bruke TWI

// AVR har ogs� et datablad som forklarer hvordan man opprettet TWI for slave-master kommunkasjon
// Disse driverene burde legges til slik at servoen kan styres fra en master. Kan tenke seg at flere slike
// servoer st�r langs et vannl�p, kan v�re greit � styre de fra en felles master da ;)

#include <stdbool.h>
#include <avr/io.h>
#define ADRESS 0x27

#define WRITE_FLAG (TWI0.MSTATUS & 1 << 6)
#define RECEIVE_ACK (TWI0.MSTATUS & 1 << 4)

void TWI_init(void);
bool twi_write_adress(uint8_t);
void send_stop(void);
void twi_send_data(unsigned char);

#endif /* TWI_MASTER_H_ */