#include "motors.h"
#include <avr/io.h>

void motors_init()
{
  /* Set motor pins as output */
  DDRD |= (1 << MOTOR_FL) | (1 << MOTOR_FR) |
          (1 << MOTOR_RR);  
  DDRB |= (1 << MOTOR_RL);

  /* Init timer to put pwm */
  timer0_pwm_init();
  timer1_pwm_init();
  timer2_pwm_init();
}

void timer0_pwm_init()
{
  /* Fast pwm, mode 3 */
  TCCR0A |= (1 << WGM01) | (1 << WGM00);

  /* Clear OCR0A and OCR0B on compare match,
   * Set on bottom */ 
  TCCR0A |= (1 << COM0A1);
  TCCR0A |= (1 << COM0B1);
  
  /* Set clock speed clk / 64
   * 125khz                   */
  TCCR0B |= (1 << CS01) | (1 << CS00);
}

void timer1_pwm_init()
{ 
  /* Fast pwm 8 bit, mode 5 */
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM12);

  /* Clear OCR1A on compare match. Set on bottom */ 
  TCCR1A |= (1 << COM1A1);
  
  /* Set clock speed clk / 64
   * 125khz                   */
  TCCR1B |= (1 << CS11) | (1 << CS10); 
}

void timer2_pwm_init()
{
  /* Fast pwm mode.
   * Clear OC2B on compare match, set on bottom.
   * clk / 64                                   */
  TCCR2A |= (1 << WGM21) | (1 << WGM20);
  TCCR2A |= (1 << COM2B1);
  TCCR2B |= (1 << CS01) | (1 << CS00);
}

void drive_motors(uint8_t FL_speed, uint8_t FR_speed,
                  uint8_t RL_speed, uint8_t RR_speed)
{
  FL_COMP_REG = FL_speed;
  FR_COMP_REG = FR_speed;
  RL_COMP_REG = RL_speed;
  RR_COMP_REG = RR_speed;
}










