// 
// 
// 
#include "Arduino.h"
#include "Motor32.h"
 
motor32_t motor32_config[8] = { 
#if defined( ARDUINO_SAMD_ZERO )
{ MOTOR_D2_CH0 ,2u,0,1000 }, // Channel 0
{ MOTOR_D3_CH1 ,3u,0,1000 }, // Channel 1
{ MOTOR_D4_CH2 ,4u,0,1000 }, // Channel 2
{ MOTOR_D5_CH3 ,5u,0,1000 }, // Channel 3
{ MOTOR_D6_CH4 , 6u,0,1000 }, // Channel 4
{ MOTOR_D7_CH5 , 7u,0,1000 }, // Channel 5
{ MOTOR_D11_CH6 , 11u,0,1000 }, // Channel 6
{ MOTOR_D13_CH7 , 13u,0,1000 }, // Channel 7
#elif defined( ARDUINO_SAM_DUE )
{ MOTOR_D34_CH0 ,34u,0,1000 }, // Channel 0
{ MOTOR_D36_CH1 ,36u,0,1000 }, // Channel 1
{ MOTOR_D38_CH2 ,38u,0,1000 }, // Channel 2
{ MOTOR_D40_CH3 ,40u,0,1000 }, // Channel 3
{ MOTOR_D9_CH4 , 9u,0,1000 }, // Channel 4
{ MOTOR_D8_CH5 , 8u,0,1000 }, // Channel 5
{ MOTOR_D7_CH6 , 7u,0,1000 }, // Channel 6
{ MOTOR_D6_CH7 , 6u,0,1000 }, // Channel 7
#endif
};
 
bool motor32_clock_enabled = false;

void motor32SetPeriod(uint32_t chan, uint16_t period)
{
	//if ((PWM->PWM_SR & bit(chan)) == 0)
	//	PWM->PWM_CH_NUM[chan].PWM_CPRD = period;
	//else
	//	PWM->PWM_CH_NUM[chan].PWM_CPRDUPD = period;
}
void motor32SetDuty(uint32_t chan, uint16_t duty)
{
	//if ((PWM->PWM_SR & bit(chan)) == 0)
	//	PWM->PWM_CH_NUM[chan].PWM_CDTY = duty;
	//else
	//	PWM->PWM_CH_NUM[chan].PWM_CDTYUPD = duty;
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

#if defined( ARDUINO_SAMD_ZERO)
		// Feed GCLK4 to TCC2 (and TC3)
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC2 (and TC3)
			GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
			GCLK_CLKCTRL_ID_TCC2_TC3;    // Feed GCLK4 to TCC2 (and TC3)
		while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TCC0(and TC1)
			GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
			GCLK_CLKCTRL_ID_TCC0_TCC1;    // Feed GCLK4 to TCC0 (and TCC1)
		while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

#elif defined (ARDUINO_SAM_DUE)

		PMC->PMC_PCER1 = bit((ID_PWM - 32));

		//Configure clock
		//Calculating the frequency MCK / (PREA x DIVA)
		// freq = 84000000 / (256 X 1) = 328125Hz
		PWM->PWM_CLK = 1 | (8 << 8);
#endif
		motor32_clock_enabled = true;
	}
 
	if(!motorP->enabled)
	{
	//	const uint8_t CHANNELS = 8;
	//	const uint8_t pwmPins[] = { 2, 3, 4, 5, 6, 7, 11, 13 };
	//	for (uint8_t i = 0; i < CHANNELS; i++)
	//	{
	//		PORT->Group[g_APinDescription[pwmPins[i]].ulPort].PINCFG[g_APinDescription[pwmPins[i]].ulPin].bit.PMUXEN = 1;
	//}

		//// Connect the TCC timers to the port outputs - port pins are paired odd PMUO and even PMUXE
		//// F & E specify the timers: TCC0, TCC1 and TCC2
		//PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
		//PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
		//PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
		//PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg = PORT_PMUX_PMUXO_E | PORT_PMUX_PMUXE_E;
#if defined( ARDUINO_SAMD_ZERO)
		//PORT->Group[g_APinDescription[ motorP->digitalPin ].ulPort].PINCFG[g_APinDescription[motorP->digitalPin].ulPin].bit.PMUXEN = 1;

		switch (motorP->digitalPin)
		{
		case   2:
		case   3:
			PORT->Group[g_APinDescription[2].ulPort].PINCFG[g_APinDescription[2].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[3].ulPort].PINCFG[g_APinDescription[3].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
			motorP->enabled = 1;
			(motorP+1)->enabled = 1;
			break;
		case 4:
		case 5:
			PORT->Group[g_APinDescription[4].ulPort].PINCFG[g_APinDescription[4].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[5].ulPort].PINCFG[g_APinDescription[5].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
			motorP->enabled = 1;
			(motorP + 1)->enabled = 1;
			break;
		case 6:
		case 7:
			PORT->Group[g_APinDescription[6].ulPort].PINCFG[g_APinDescription[6].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[7].ulPort].PINCFG[g_APinDescription[7].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg = PORT_PMUX_PMUXO_F | PORT_PMUX_PMUXE_F;
			motorP->enabled = 1;
			(motorP + 1)->enabled = 1;
			break;
		case 11:
		case 13:
			PORT->Group[g_APinDescription[11].ulPort].PINCFG[g_APinDescription[11].ulPin].bit.PMUXEN = 1;
			PORT->Group[g_APinDescription[13].ulPort].PINCFG[g_APinDescription[13].ulPin].bit.PMUXEN = 1;
			 PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg = PORT_PMUX_PMUXO_E | PORT_PMUX_PMUXE_E;
			motorP->enabled = 1;
			(motorP + 1)->enabled = 1;
			break;
		}


#elif defined (ARDUINO_SAM_DUE)

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
#endif
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

