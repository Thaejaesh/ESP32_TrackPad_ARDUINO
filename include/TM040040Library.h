#ifndef __TM040040_LIBRARY
#define __TM040040_LIBRARY

#include <Arduino.h>

/*
- The TM040040 Device SPI ADDRESSES are 8bits/1Byte
- The Top 3 bits are opcodes defining Read or Write 
- The Bottom 5 bits are the actual address 0x00 -> 0x1F
*/
#define TM040040_SPI_READ_OPCODE  0xA0
#define TM040040_SPI_WRITE_OPCODE 0x80
// Macro to generate Read Addresss, addresses are 5 bits with opcodes being top 3 bits
#define TM040040_GENERATE_SPI_READ_ADDRESS(ADDR)  {TM040040_SPI_READ_OPCODE  | (0x1F) * ADDR}
#define TM040040_GENERATE_SPI_WRITE_ADDRESS(ADDR) (TM040040_SPI_WRITE_OPCODE | (0x1F) * ADDR)

typedef enum{
    E_TM040040_REGx00_FIRMWARE_ID        = 0x00,
    E_TM040040_REGx01_FIRMWARE_VERSION   = 0x01,
    E_TM040040_REGx02_STATUS1            = 0x02,
    E_TM040040_REGx03_SYSnCONFIG1        = 0x03,
    E_TM040040_REGx04_FEED_CONFIG1       = 0x04,
    E_TM040040_REGx05_FEED_CONFIG2       = 0x05,
    E_TM040040_REGx06_RESERVED           = 0x06,
    E_TM040040_REGx07_CAL_CONFIG1        = 0x07,
    E_TM040040_REGx08_PS2AUX_CONTROL     = 0x08,
    E_TM040040_REGx09_SAMPLE_RATE        = 0x09,
    E_TM040040_REGx0A_Z_IDLE             = 0x0A,
    E_TM040040_REGx0B_Z_SCALER           = 0x0B,
    E_TM040040_REGx0C_SLEEP_INTERVAL     = 0x0C,
    E_TM040040_REGx0D_SLEEP_TIMER        = 0x0D,
    E_TM040040_REGx0E_RESERVED           = 0x0E,
    E_TM040040_REGx0F_RESERVED           = 0x0F,
    E_TM040040_REGx10_RESERVED           = 0x10,
    E_TM040040_REGx11_RESERVED           = 0x11,
    E_TM040040_REGx12_PACKET_BYTE_0      = 0x12,
    E_TM040040_REGx13_PACKET_BYTE_1      = 0x13,
    E_TM040040_REGx14_PACKET_BYTE_2      = 0x14,
    E_TM040040_REGx15_PACKET_BYTE_3      = 0x15,
    E_TM040040_REGx16_PACKET_BYTE_4      = 0x16,
    E_TM040040_REGx17_PACKET_BYTE_5      = 0x17,
    //Rest of the registers to 0x1F are reserved
}E_TRACKPAD_REGISTERS;


typedef struct __attribute__((packed)){
    uint32_t x_position;
    uint32_t y_position;
}xy_position;

uint32_t get_x_position(void);
uint32_t get_y_position(void);

bool set_feed_enable(void); // Set Bit0 in Regx04
bool unset_feed_enable(void);



#endif /* __TM040040_LIBRARY */