//	written by : Doug Reuter
//	All Motors - 8 motor array each with timing to confirm correct PWM ( due or zero)
//	http://cheapogadget.com   https://github.com/cheapogadget/Taskmanager32
//	Copyright (C) 2016 Doug Reuter   dougreuter@gmail.com

//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
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
Motor32  motors[8] = {
	{6},{7},{8},{9},{34},{36},{38},{40}
};
uint32_t onRise[8];
uint32_t pulseWidth[8];
#define MOTORT(X,Y) { if (digitalRead(X)) onRise[Y] = micros(); else pulseWidth[Y] = micros() - onRise[Y]; }
void motorTime0() MOTORT(A0, 0)
void motorTime1() MOTORT(A1, 1)
void motorTime2() MOTORT(A2, 2)
void motorTime3() MOTORT(A3, 3)
void motorTime4() MOTORT(A4, 4)
void motorTime5() MOTORT(A5, 5)
void motorTime6() MOTORT(A6, 6)
void motorTime7() MOTORT(A7, 7)

void setup()
{
	Serial.begin(115200);
	pinMode(A0, INPUT_PULLUP); attachInterrupt(A0, motorTime0, CHANGE);
	pinMode(A1, INPUT_PULLUP); attachInterrupt(A1, motorTime1, CHANGE);
	pinMode(A2, INPUT_PULLUP); attachInterrupt(A2, motorTime2, CHANGE);
	pinMode(A3, INPUT_PULLUP); attachInterrupt(A3, motorTime3, CHANGE);
	pinMode(A4, INPUT_PULLUP); attachInterrupt(A4, motorTime4, CHANGE);
	pinMode(A5, INPUT_PULLUP); attachInterrupt(A5, motorTime5, CHANGE);
	pinMode(A6, INPUT_PULLUP); attachInterrupt(A6, motorTime6, CHANGE);
	pinMode(A7, INPUT_PULLUP); attachInterrupt(A7, motorTime7, CHANGE);

	for (int i = 0; i < 8; i++)
		motors[i].set(1000 + (i * 100));
}

void loop()
{
	for (int i = 0; i < 8; i++)
	{
		Serial.print(" A"); Serial.print(i); Serial.print(" "); Serial.print(pulseWidth[i]);
	}
	Serial.println();
	delay(500);
}
