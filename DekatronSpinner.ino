/*
Dekatron Spinner - Variable Rate

modified March 25, 2017
by Michael Moorrees
*/

int D_count = 0;
int i_rate = 15;
int i_delay = 100;
int LED = 13;         // Test LED
int Guide1 = 52;       // Guide 1 - G1 pin of 2-guide Dekatron
int Guide2 = 50;       // Guide 2 - G2 pin of 2-guide Dekatron
int Index = 48;       // Index   - NDX input pin. High when glow at K0
int Ndx = 0;          // K0 index indicator variable

void G_step(int CINT) // Dekatron Stepper
{
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
	pinMode(LED, OUTPUT);

	D_count = 0;
	i_rate = 15;
	i_delay = 100;
	Ndx = 0;
	G_step(D_count);
	digitalWrite(LED, LOW);
}

// the loop function runs over and over again forever
void loop() {
	Ndx = digitalRead(Index); // Sample for glow at K0
	if (Ndx) {
		i_rate = 16;
		i_delay = (3 * i_rate) + 8;
		digitalWrite(LED, HIGH);
	}
	i_rate--;
	i_delay = (3 * abs(i_rate)) + 8;
	D_count++;
	delay(i_delay);            // wait per i_rate
	if (D_count > 2) D_count = 0;
	if (D_count < 0) D_count = 2;
	G_step(D_count);          // Step Dekatron
	digitalWrite(LED, LOW);
}
