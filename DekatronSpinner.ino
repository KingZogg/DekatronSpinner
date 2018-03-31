class dekatronStep
{
public:	
	int Guide1;   
	int Guide2;
	int Index;
	int previousGuideState;
	int stepDelay;
	bool clockwise;
	unsigned long previousMillis;
	
public:
	dekatronStep(int pin1, int pin2, int pin3, bool direction, int sDelay)	//Guide1, Guide2, Index, StepDelay, Direction
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		stepDelay = sDelay;
		clockwise = direction;
		
		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);
	}

void updateStep(unsigned long currentMillis)
	{
		//Delay needed if there is not enough delay in the loop when calling.
		// will need adjusting depending on processor speed. This is runing at 16mHz.
	
	delayMicroseconds(40); 

	//unsigned long currentMillis = millis();

	if ((currentMillis - previousMillis >= stepDelay))
	{
		switch (previousGuideState) {
		case 0:
			previousGuideState = 1;
			digitalWrite(Guide1, LOW);
			digitalWrite(Guide2, LOW);
			previousMillis = currentMillis;
			break;

		case 1:
			previousGuideState = 2;
			if (clockwise == true)
			{
				digitalWrite(Guide1, HIGH);
				digitalWrite(Guide2, LOW);
			}
			else
			{
				digitalWrite(Guide1, LOW);
				digitalWrite(Guide2, HIGH);
			}
			previousMillis = currentMillis;
			break;

		case 2:
			previousGuideState = 0;
			if (clockwise == true)
			{
				digitalWrite(Guide1, LOW);
				digitalWrite(Guide2, HIGH);
			}
			else
			{
				digitalWrite(Guide1, HIGH);
				digitalWrite(Guide2, LOW);
			}
			previousMillis = currentMillis;
			break;
		}

	}

	
}

};

dekatronStep Dek1(52, 50, 48,false,0); //setup physical pins here. In this case 52 and 50 are G1 and G2. The index is 48.
dekatronStep Dek2(44, 42, 40,true,10);
dekatronStep Dek3(36, 34, 32,true,50);
dekatronStep Dek4(28, 26, 24,true,100);

void setup()
{
	// TIMER 1 for interrupt frequency 2000 Hz:
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0; // initialize counter value to 0
			   // set compare match register for 2000 Hz increments
	OCR1A = 7999; // = 16000000 / (1 * 2000) - 1 (must be <65536)
				  // turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei(); // allow interrupts

	pinMode(LED_BUILTIN, OUTPUT);

}

// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{
	unsigned long currentMillis = millis();

	Dek1.updateStep(currentMillis);
	Dek2.updateStep(currentMillis);
	Dek3.updateStep(currentMillis);
	Dek4.updateStep(currentMillis);

}

// the loop function runs over and over again forever
void loop() {

	if (digitalRead(Dek1.Index)) digitalWrite(LED_BUILTIN, HIGH);
	else digitalWrite(LED_BUILTIN, LOW);

}

