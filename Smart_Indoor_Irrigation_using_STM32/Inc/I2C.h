

#ifndef I2C_H_
#define I2C_H_

void I2C_init(void);
void I2C1_read(uint8_t slave_addr,uint8_t Memory_addr, uint8_t * data) ;
void I2C1_Burst_read(uint8_t slave_addr,uint8_t Memory_addr, int n, uint8_t * data);
void I2C1_Burst_write(uint8_t slave_addr,uint8_t Memory_addr, int n, uint8_t * data);


#endif /* I2C_H_ */
