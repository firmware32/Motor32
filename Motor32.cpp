// 
// 
// 
#include "Arduino.h"
#include "Motor32.h"
 
motor32_t motor32_config[8] = { 
{ MOTOR_D34_CH0 ,34u,0,1000 }, // Channel 0
{ MOTOR_D36_CH1 ,36u,0,1000 }, // Channel 1
{ MOTOR_D38_CH2 ,38u,0,1000 }, // Channel 2
{ MOTOR_D40_CH3 ,40u,0,1000 }, // Channel 3
{ MOTOR_D9_CH4 , 9u,0,1000 }, // Channel 4
{ MOTOR_D8_CH5 , 8u,0,1000 }, // Channel 5
{ MOTOR_D7_CH6 , 7u,0,1000 }, // Channel 6
{ MOTOR_D6_CH7 , 6u,0,1000 }, // Channel 7
};
 
bool motor32_clock_enabled = false;

void motor32SetPeriod(uint32_t chan, uint16_t period)
{
	if ((PWM->PWM_SR & bit(chan)) == 0)
		PWM->PWM_CH_NUM[chan].PWM_CPRD = period;
	else
		PWM->PWM_CH_NUM[chan].PWM_CPRDUPD = period;
}
void motor32SetDuty(uint32_t chan, uint16_t duty)
{
	if ((PWM->PWM_SR & bit(chan)) == 0)
		PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
	else
		PWM->PWM_CH_NUM[chan].PWM_CDTYUPD = duty;
}
#define MOTOR32_MAX_WIDTH 2040
void motor32SetDutyWidth(uint32_t chan, uint16_t widthms, uint32_t period)
{
	motor32SetDuty(chan, map(widthms, 0, MOTOR32_MAX_WIDTH, 0, period));
 
}


void Motor32::init()
{
	if (!motor32_clock_enabled)
	{
		PMC->PMC_PCER1 = bit((ID_PWM - 32));

		//Configure clock
		//Calculating the frequency MCK / (PREA x DIVA)
		// freq = 84000000 / (256 X 1) = 328125Hz
		PWM->PWM_CLK = 1 | (8 << 8);

		motor32_clock_enabled = true;
	}

	if(!motorP->enabled)
	{
		//Because we are using PORTC.PIN23 in peripheral B mode
		//  we need to enable the clock for that line.
		PMC->PMC_PCER0 = bit(ID_PIOC);

		//Disable PIO Control on PC23 and set up for Peripheral B PWML7 
		//PWM Pin 6 on the board
		PIOC->PIO_PDR |= motor32_config[channel].port;
		PIOC->PIO_ABSR |= motor32_config[channel].port;;

		//Enable output on C23
		PIOC->PIO_OER |= motor32_config[channel].port;;

		//Enable pull-up on pin
		PIOC->PIO_PUDR |= motor32_config[channel].port;
		delay(10);
	//Channel Mode Register - Use ClockA as configured in PWM_CLK
	PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA;
	  motorP->period = ceil(328125.0 / 490.0);
	//Final frequency = 164062.5 / 255 = 643.38Hz
	motor32SetPeriod(channel, motorP->period);
	
	//Duty cycle = ratio of Period to Duty (Duty / Period)  
	motor32SetDuty(channel, map(motorP->defaultMs, 0, MOTOR32_MAX_WIDTH, 0, motorP->period));

	//Enable channel
	PWM->PWM_ENA = bit(channel);
	 motorP->enabled = 1;
	}
 
}
uint16_t Motor32::get()
{
	return pulseWidth;

}
void Motor32::set( uint16_t ms)
{
	pulseWidth = constrain(ms, 1000, 2000);
	motor32SetDutyWidth(channel, pulseWidth, motorP->period);

}
  Motor32::Motor32( uint8_t pin)
{

	if (PinAvailable(pin))
		SetPin(pin);
	 
	

}
  bool Motor32::PinAvailable(uint8_t pin)
  {
	  for (uint8_t i = 0; i < 8; i++)
		  if (motor32_config[i].digitalPin == pin && !motor32_config[i].enabled)
			  return true;
	  return false;
  }
  void  Motor32::SetPin(uint8_t pin)
  {
	  for (uint8_t i = 0; i < 8; i++)
		  if (motor32_config[i].digitalPin == pin)
		  {
			  motorP = &motor32_config[i];
			  pulseWidth = motorP->defaultMs;
			  channel = i;
		  }


	  init();
  }

//Motor32 Motor32;

