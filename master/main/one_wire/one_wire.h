#ifndef ONE_WIRE
#define ONE_WIRE

#include "hal/gpio_types.h"

#define ONE_WIRE_WRITE_SIGNAL_TIME (8)
#define ONE_WIRE_READ_SIGNAL_TIME (5)
#define ONE_WIRE_READ_SAMPLE_TIME (10)
#define ONE_WIRE_WRITE_TIME (52)
#define ONE_WIRE_READ_TIME (45)
#define ONE_WIRE_RECOVER_TIME (10)
#define ONE_WIRE_RESET_SIGNAL_TIME (480)
#define ONE_WIRE_RESET_SAMPLE_TIME (70)
#define ONE_WIRE_RESET_STABILIZATION_TIME (410)

#define HIGH_LEVEL 1
#define LOW_LEVEL 0

#define EXTRACT_BIT_TO_LESSER(data, bit) ((data & (1 << bit)) >> bit)

enum one_wire_bits {
  LOW = 0x0,
  HIGH = 0x1,
};

typedef enum one_wire_bits one_wire_bits;

// must be called before everything
void one_wire_initialize(gpio_num_t serial_pin, gpio_num_t parallel_pin);

void one_wire_write_dword(gpio_num_t gpio_pin, uint32_t data);
void one_wire_write_word(gpio_num_t gpio_pin, uint16_t data);
void one_wire_write_byte(gpio_num_t gpio_pin, uint8_t data);

uint32_t one_wire_read_dword(gpio_num_t gpio_pin);
uint16_t one_wire_read_word(gpio_num_t gpio_pin);
uint8_t one_wire_read_byte(gpio_num_t gpio_pin);

uint8_t one_wire_reset(gpio_num_t gpio_pin);
void one_wire_write_bit(gpio_num_t gpio_pin, one_wire_bits data);
uint8_t one_wire_read_bit(gpio_num_t gpio_pin);

#endif // ONE_WIRE
