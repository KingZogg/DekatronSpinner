/*
Dekatron Spinner - Variable Rate

modified March 25, 2017
by Michael Moorrees
*/

int D_count = 0;
int Guide1 = 52;       // Guide 1 - G1 pin of 2-guide Dekatron
int Guide2 = 50;       // Guide 2 - G2 pin of 2-guide Dekatron
int Index = 48;		// Index   - NDX input pin. High when glow at K0


void G_step(int CINT)		// Dekatron Stepper
{
	delayMicroseconds(80); // minimum pulse width of a dekatron seems to be about 80uS.

	if (CINT > 2) CINT = 0;
	if (CINT == 0)
	{
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, LOW);
	}
	if (CINT == 1)
	{
		digitalWrite(Guide1, HIGH);
		digitalWrite(Guide2, LOW);
	}
	if (CINT == 2)
	{
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, HIGH);
	}
}

// setup() runs once, at reset, to initialize system
void setup() {
	pinMode(Guide1, OUTPUT);
	pinMode(Guide2, OUTPUT);
	pinMode(Index, INPUT);
	D_count = 0;
	G_step(D_count);
}

// the loop function runs over and over again forever
void loop() {

	D_count++;

	if (D_count > 2) D_count = 0;     // sends only 0,1 or 2 to the step loop.
	if (D_count < 0) D_count = 2;		// sends only 0,1 or 2 to the step loop.
	G_step(D_count);					// Step Dekatron
}
