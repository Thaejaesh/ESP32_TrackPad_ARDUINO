#include <Arduino.h>
// #include "HID.h"
#include "SPI.h"

#include "TM040040Library.h"

gpio_num_t backlight_led = GPIO_NUM_38;
gpio_num_t spi_cs = GPIO_NUM_10;

static void write_register(E_TRACKPAD_REGISTERS regval, uint8_t value);
static uint8_t read_register(E_TRACKPAD_REGISTERS regval);

SPIClass *hspi = NULL;
static const uint32_t spi_speed = 1000000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  hspi = new SPIClass(HSPI);
  hspi->begin(SCK, MISO, MOSI, SS);
  gpio_set_direction(spi_cs, GPIO_MODE_OUTPUT);

  //Use backlight for LCD as onboard LED
  gpio_set_direction(backlight_led, GPIO_MODE_OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  gpio_set_level(backlight_led, HIGH);
  delay(500);
  gpio_set_level(backlight_led, LOW);
  delay(500);
  Serial.println("Hello");

  read_register(E_TM040040_REGx0A_Z_IDLE);
}

static void write_register(E_TRACKPAD_REGISTERS regval, uint8_t value){
  // Assert Chip Select
  gpio_set_level(spi_cs, LOW);

  hspi->beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE1));
  hspi->transfer(TM040040_SPI_WRITE_OPCODE | (uint8_t)regval);
  hspi->transfer(value);

  hspi->endTransaction();

  // De-Assert Chip Select
  gpio_set_level(spi_cs, HIGH);
}

static uint8_t read_register(E_TRACKPAD_REGISTERS regval){
  uint8_t read_byte = 0;

  // Assert Chip Select
  gpio_set_level(spi_cs, LOW);

  hspi->beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE1));
  hspi->transfer(TM040040_SPI_READ_OPCODE| (uint8_t)regval);
  hspi->transfer(0xFB);
  hspi->transfer(0xFB);
  read_byte = hspi->transfer(0xFB); // Contents are received on 3rd filler byte as per DS

  hspi->endTransaction();

  // Print out read values
  Serial.println("Read from Register:");
  Serial.println(read_byte);

  // De-Assert Chip Select
  gpio_set_level(spi_cs, HIGH);
  return read_byte;
}