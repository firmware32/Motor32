//	written by : Doug Reuter
//	All Motors - 8 motor array each with timing to confirm correct PWM ( due )
//	http://www.firmware32.com http://cheapogadget.com   https://github.com/firmware32/motor32
//	Copyright (C) 2016 Doug Reuter   dougreuter@gmail.com

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published byS
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.

//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.

//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.

// INSTRUCTIONS: Hook jumper betwee A0-A7 to D9,D8,D7,D6,D40,D38,D36,D34
// Works with Arduino DUE 
#include <Motor32.h>

// 8 motors
#define MOTOR_PIN 9 // 9,8,7,6,40,38,36,34
Motor32  motor(MOTOR_PIN);
uint32_t onRise;
uint32_t pulseWidth;
#define MEASURE_PIN A0

volatile uint32_t cycleCount;
void motorTime()
{
	if (digitalRead(MEASURE_PIN))
		onRise = micros();
	else
		pulseWidth = micros() - onRise;
	cycleCount++;
}

void setup()
{
	Serial.begin(115200);
	pinMode(MEASURE_PIN, INPUT_PULLUP); attachInterrupt(MEASURE_PIN, motorTime, CHANGE);
}
uint32_t v = 1000;
void loop()
{
	v++;
	if (v >2000)
		v = 1000;
	motor.set(v);
	Serial.print("Pin "); Serial.print(MOTOR_PIN); Serial.print(" set to "); Serial.print(v);
	Serial.print(" A0 reads ");   Serial.print(" "); Serial.print(pulseWidth); Serial.println(" us");
	uint32_t freq = cycleCount;
	delay(1000);
	Serial.print(" Frequency: "); Serial.print((cycleCount - freq) >> 1); +Serial.print("hz ");
}
