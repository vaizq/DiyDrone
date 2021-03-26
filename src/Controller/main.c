#include <avr/io.h>
#include <util/delay.h>

#include "SPI_Commands.h"
#include "USART.h"

/* _______________ Definitions _________________ */

#define JOY_A_X PC0
#define JOY_A_Y PC1
#define JOY_B_X PC2
#define JOY_B_Y PC3

#define DDR_SPI DDRB
#define PORT_SPI PORTB

#define SS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

/* ________________ Globals _____________________ */

struct joystick
{
  uint8_t x, y;
};

struct joystick joy_A = {.x = 0, .y = 0};
struct joystick joy_B = {.x = 0, .y = 0};


/* ________________ Functions ___________________ */

void read_joystick(struct joystick *joy, uint8_t port_x, uint8_t port_y);
void adc_init();
uint8_t adc_read(uint8_t port);

void SPI_init();
void SPI_transmit(uint8_t msg);
uint8_t SPI_read();

/* _________________ Main _________________________ */

int main(void)
{
  adc_init();
  SPI_init();
  
  uint8_t i = 0;
  while(1)
  {
    read_joystick(&joy_A, JOY_A_X, JOY_A_Y);
    read_joystick(&joy_B, JOY_B_X, JOY_B_Y);

    SPI_transmit(JOYSTICKS);
    SPI_transmit(joy_A.x);
    SPI_transmit(joy_A.y);
    SPI_transmit(joy_B.x);
    SPI_transmit(joy_B.y);
  }
  return 0;
}


/* _______________ Function definitions ___________ */

void read_joystick(struct joystick* joy,
                    uint8_t port_x, uint8_t port_y)
{
  joy->x = adc_read(port_x);
  joy->y = adc_read(port_y);
}

void adc_init()
{
  /* Reference voltage */
  ADMUX |= (1 << REFS0);
  /* Set ADC clock speed, clk/32 */
  ADCSRA |= (1 << ADPS2);
  /* Left justify result to read 8bit value */
  ADMUX |= (1 << ADLAR);
  /* Enable ADC */
  ADCSRA |= (1 << ADEN); 
}

uint8_t adc_read(uint8_t port)
{
  /* Clear port select bits */
  ADMUX &= 0xf0;
  ADMUX |= port;
  /* Start reading */
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC))
  {}
    
  return ADCH;
}

/* Initialize Spi in master mode */
void SPI_init()
{ 
  DDR_SPI |= (1 << SS);
  PORT_SPI |= (1 << SS);

  DDR_SPI |= (1 << MOSI);
  PORT_SPI |= (1 << MISO);
  DDR_SPI |= (1 << SCK);

  /* clk / 128. Can be made faster. */
  SPCR |= (1 << SPR1) | (1 << SPR0);
  SPCR |= (1 << MSTR);
  SPCR |= (1 << SPE);
}

void SPI_transmit(uint8_t msg)
{
  PORT_SPI &= ~(1 << SS);
  /* Write to Data Register */
  SPDR = msg;
  /* Wait for transfer to complete */
  while(!(SPSR & (1 << SPIF)))
    ;

  PORT_SPI |= (1 << SS);
}

uint8_t SPI_read()
{
  /* Write to spi to get byte back */
  SPI_transmit(0);
  return SPDR;
} 





