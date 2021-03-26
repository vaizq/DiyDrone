#include <avr/io.h>

/* Use timers 0 and 1 for pwm */

/* Pin definitions */
#define MOTOR_FL PD6 // Motor front left OC0A
#define MOTOR_FR PD5 // Motor Front right OC0B
#define MOTOR_RL PB1 // Motor rear Left OC1A
#define MOTOR_RR PD3 // Motor rear right OC2B

/* Timers compare registers for motors
 * Write to set speeds for motors */

#define FL_COMP_REG OCR0A
#define FR_COMP_REG OCR0B
#define RL_COMP_REG OCR1A
#define RR_COMP_REG OCR2B


/* Becouse SS pin must be used for SPI,
 * We need to use OC1A with timer1
 * and OC2A with timer2..             */

void motors_init();
void timer0_pwm_init();
void timer1_pwm_init();
void timer2_pwm_init();

void drive_motors(uint8_t FL_speed, uint8_t FR_speed,
                  uint8_t RL_speed, uint8_t RR_speed);
