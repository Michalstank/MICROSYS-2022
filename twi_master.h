#ifndef TWI_MASTER_H_
#define TWI_MASTER_H_

#include <stdbool.h>
#include <avr/io.h>
#define ADRESS 0x27

#define WRITE_FLAG (TWI0.MSTATUS & 1 << 6)
#define RECEIVE_ACK (TWI0.MSTATUS & 1 << 4)

void twi_init(void);
bool twi_write_adress(uint8_t);
void send_stop(void);
void twi_send_data(unsigned char);

#endif /* TWI_MASTER_H_ */