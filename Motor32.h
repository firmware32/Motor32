// Motor32.h

#ifndef _MOTOR32_h
#define _MOTOR32_h
#include "Arduino.h"
#define MOTOR_D34_CH0			(1u << 2) 
#define MOTOR_D36_CH1			(1u << 4) 
#define MOTOR_D38_CH2         (1u << 6) 
#define MOTOR_D40_CH3         (1u << 8) 
#define MOTOR_D9_CH4          (1u << 21) /**PIO_PC21*/
#define	MOTOR_D8_CH5			(1u << 22) /**PIO_PC22*/
#define MOTOR_D7_CH6          (1u << 23) /**PIO_PC23 */
#define MOTOR_D6_CH7          (1u << 24) /**PIO_PC24 */
typedef struct {
	uint32_t port;
	uint32_t digitalPin;
	bool enabled;
	uint16_t defaultMs;
	uint32_t period;
} motor32_t;

extern motor32_t motor32_config[8];
extern bool motor32_clock_enabled;
	#include "arduino.h"
 

class Motor32
{
 protected:
	 motor32_t * motorP;
	 uint8_t channel;
	 uint16_t pulseWidth;
	 void init();
	 bool PinAvailable(uint8_t pin);
	 void SetPin(uint8_t pin);
 public:
	 Motor32(uint8_t pin);
	
	 void set(uint16_t ms); //1000 - 2000
	 uint16_t get(); // 
};

  /*Motor32  Motor32;*/

#endif

