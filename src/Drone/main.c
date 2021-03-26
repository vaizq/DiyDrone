#include <avr/io.h>
#include <util/delay.h>

#include "motors.h"
#include "SPI_Commands.h"
#include "USART.h"


/* ______________ Definitions ______________ */

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

/* ______________ Functions ________________ */

void SPI_init();
uint8_t SPI_receive();

void read_joysticks();
void calculate_motor_speeds(uint8_t turn, uint8_t throttle,
                            uint8_t dir_x, uint8_t dir_y);

uint8_t speed = 255;

int main(void)
{
  initUSART();
  motors_init();
  SPI_init();

  char msg;
  uint8_t turn, throttle, dir_x, dir_y;

  while(1)
  {
    msg = SPI_receive();
    if(msg == JOYSTICKS)
    {
      read_joysticks(&dir_x, &dir_y, &turn, &throttle);
      calculate_motor_speeds(turn, throttle, dir_x, dir_y); 
    }
  }
  return 0;
}


/* _____________ Function definitons ________ */

void SPI_init()
{
  SPI_DDR |= (1 << MISO);
  SPCR |= (1 << SPE);
}

uint8_t SPI_receive()
{
  while(!(SPSR & (1 << SPIF)))
    ; 
    
  return SPDR;
}


void read_joysticks(uint8_t *a_x, uint8_t *a_y,
                    uint8_t *b_x, uint8_t *b_y)
{
  *a_x = SPI_receive(); // turn
  *a_y = SPI_receive(); // Throttle
  *b_x = SPI_receive(); // Forwards or backwards
  *b_y = SPI_receive(); // Left or right
}

uint8_t balance(uint8_t num, uint8_t x)
{
  if(x > 127)
  {
    x -= 127;
    if(num + x <= 255)
    {
      num += x;
    }
    else
    {
      num = 255; 
    }
  }
  else
  {
    x = 127 - x;
    if(num - x >= 0)
    {
      num -= x;
    }
    else
    {
      num = 0;
    }
  }
  return num;
}

void calculate_motor_speeds(uint8_t turn, uint8_t throttle,
                            uint8_t dir_x, uint8_t dir_y)
{
  uint8_t fl=throttle, fr=throttle,
           rl=throttle, rr=throttle;
  
  fl = balance(fl, 255 - dir_x);
  fr = balance(fr, 255 - dir_x);
  rl = balance(rl, dir_x);
  rr = balance(rr, dir_x); 

  fl = balance(fl, dir_y);
  fr = balance(fr, 255 - dir_y);
  rl = balance(rl, dir_y);
  rr = balance(rr, 255 - dir_y); 
  
  drive_motors(fl, fr,
               rl, rr);
}











