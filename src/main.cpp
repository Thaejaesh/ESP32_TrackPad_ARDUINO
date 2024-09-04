// External Imports
#include <Arduino.h>
#include "SPI.h"

#include "USB.h"
#include "USBHIDMouse.h"
// Internal Imports
#include "TM040040Library.h"

// Contants
SPIClass *hspi = NULL;
static const uint32_t spi_speed = 1000000;

USBHIDMouse Mouse;

// Pin Assignments
gpio_num_t backlight_led = GPIO_NUM_38;
gpio_num_t spi_cs = GPIO_NUM_10;

// Function Prototypes
static void write_register(E_TRACKPAD_REGISTERS reg_idx, uint8_t value);
static uint8_t read_register(E_TRACKPAD_REGISTERS reg_idx);


void setup() {

  // Initialize mouse control
  Mouse.begin();
  USB.begin();

  Serial.begin(115200);

  // Initialize SPI Bus
  hspi = new SPIClass(HSPI);
  hspi->begin(SCK, MISO, MOSI, SS);
  gpio_set_direction(spi_cs, GPIO_MODE_OUTPUT);

  //Use backlight for LCD as onboard LED
  gpio_set_direction(backlight_led, GPIO_MODE_OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  gpio_set_level(backlight_led, HIGH);
  Mouse.move(5000, 0, 0);
  delay(500);
  gpio_set_level(backlight_led, LOW);
  Mouse.move(-5000, 0, 0);
  delay(500);
  Serial.println("Loop");

  read_register(E_TM040040_REGx0A_Z_IDLE);
}

/**
 * @brief  Write Register to the TM040040
 * @param  reg_idx - Index of register to write to
 * @param  value   - Byte value to be written to the register 
 * @retval None
 */
static void write_register(E_TRACKPAD_REGISTERS reg_idx, uint8_t value){
  // Assert Chip Select
  gpio_set_level(spi_cs, LOW);

  hspi->beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE1));
  hspi->transfer(TM040040_SPI_WRITE_OPCODE | (uint8_t)reg_idx);
  hspi->transfer(value);

  hspi->endTransaction();

  // De-Assert Chip Select
  gpio_set_level(spi_cs, HIGH);
}

/**
 * @brief  Write Register to the TM040040
 * @param  reg_idx - Index of register to write to
 * @param  value   - Byte value to be written to the register 
 * @retval uint8_t - The Byte value read from the register
 */
static uint8_t read_register(E_TRACKPAD_REGISTERS reg_idx){
  uint8_t read_byte = 0;

  // Assert Chip Select
  gpio_set_level(spi_cs, LOW);

  // Begin SPI transaction
  hspi->beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE1));

  // Transfer Read Address to IC
  hspi->transfer(TM040040_SPI_READ_OPCODE| (uint8_t)reg_idx);
  hspi->transfer(0xFB); // Filler Byte
  hspi->transfer(0xFB); // Filler Byte
  read_byte = hspi->transfer(0xFB); // Contents are received on 3rd filler byte as per DS

  //End SPI transaction
  hspi->endTransaction();

  // De-Assert Chip Select
  gpio_set_level(spi_cs, HIGH);

  // Print out read values
  Serial.println("Read from Register:");
  Serial.println(read_byte);

  return read_byte;
}