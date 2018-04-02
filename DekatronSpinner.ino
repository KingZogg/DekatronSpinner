class dekatronStep
{
// Class Member Variables
// These are initialized at startup
public:	
	byte Guide1;   
	byte Guide2;
	byte Index;
	byte previousGuideState;
	int stepDelay;
	bool clockwise;
	int atTheIndexPin;
	
	unsigned long previousMillis;

	//index ignore timout settings.
	byte oldIndexState = HIGH;
	unsigned long ignoreTime = 5;  // milliseconds
	unsigned long indexHighTime;	// when the index last changed state

	


// Constructor - creates a dekatronStep 
// and initializes the member variables and state
public:
	dekatronStep(byte pin1, byte pin2, byte pin3, bool direction,int indexOn,unsigned long timeIgnore, int sDelay)	//Guide1, Guide2, Index, StepDelay, Direction
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		stepDelay = sDelay;
		clockwise = direction;
		atTheIndexPin = indexOn;
		ignoreTime = timeIgnore;

		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);

		
	}
	
//Member function.
int updateStep(bool clockwise,int atTheIndexPin,int stepDelay)		
	{
		//Delay needed if there is not enough delay in the loop when calling.
		// will need adjusting depending on processor speed. This is runing at 16mHz.
	//int atTheIndexPin;
	
	delayMicroseconds(40); 

	//unsigned long currentMillis = millis();

	if (millis() - previousMillis >= stepDelay)
	{
		switch (previousGuideState) {
		case 0:
			previousGuideState = 1;
			digitalWrite(Guide1, LOW);
			digitalWrite(Guide2, LOW);
			previousMillis = millis();
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
			previousMillis = millis();
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
			previousMillis = millis();
			break;
		} // end of switch case

		  // see if Index is High or Low
		atTheIndexPin = digitalRead(Index);

		/*
		// see if Index is High or Low
		byte indexState = digitalRead(Index);

		

		// has index state changed since last time?
		if (indexState != oldIndexState)
		{
			// ignore time.
			if (millis() - indexHighTime >= ignoreTime)
			{
				indexHighTime = millis();  // when index was high
				oldIndexState = indexState;  // remember for next time 

				if ((indexState == HIGH) && (clockwise == false))
				{
					clockwise = true;
					//Serial.println("Clockwise");
					//Serial.println("index high");

				}
				else if (((indexState == HIGH)) && (clockwise == true))
				{
					clockwise = false;
					//Serial.println("Counter Clockwise");
					//Serial.println("index low");

				}

			}  // end if ignore time up


		}  // end of state change

		*/
		
		return atTheIndexPin;
	}

	
	
}

};

//setup physical pins here. In this case 52 and 50 are G1 and G2. The index is 48.
dekatronStep Dek1(52, 50, 48,true,false,0,0);
dekatronStep Dek2(44, 42, 40,true,false,0,0);
dekatronStep Dek3(36, 34, 32,true,false,0,0);
dekatronStep Dek4(28, 26, 24,true,false,0,0);






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

	//Serial.begin(2000000);

}

// Interrupt is called once a millisecond
ISR(TIMER1_COMPA_vect)
{
//	unsigned long currentMillis = millis();

	Dek1.updateStep(false,0,20);

	

	if (Dek1.atTheIndexPin == 1) {
		Dek1.updateStep(true,0, 1000);
		digitalWrite(LED_BUILTIN, HIGH);
	}
	
	//Dek2.updateStep(true , 100);
	//Dek3.updateStep(true , 500);
	//Dek4.updateStep(false, 1000);
	


	//Dek2.updateStep.ignoreTime(currentMillis);
	//Dek3.updateStep.ignoreTime(currentMillis);
	//Dek4.updateStep.ignoreTime(currentMillis);

	//updateIndex(currentMillis);
	//Dek1.atTheIndexPin(currentMillis);
	//Dek1.atTheIndexPin;
}

/*
void updateIndex(unsigned long currentMillis) {

	// see if Index is High or Low
	byte indexState = digitalRead(Dek4.Index);

	// has index state changed since last time?
	if (indexState != oldIndexState)
	{
		// ignore time.
		if (millis() - indexHighTime >= ignoreTime)
		{
			indexHighTime = currentMillis;  // when index was high
			oldIndexState = indexState;  // remember for next time 
			
				if ((indexState == HIGH) && (Dek3.clockwise == false))
				{
					Dek3.clockwise = true;
					Serial.println("Clockwise");
					//Serial.println("index high");

				}
				else if (((indexState == HIGH)) && (Dek3.clockwise == true))
				{
					Dek3.clockwise = false;
					Serial.println("Counter Clockwise");
					//Serial.println("index low");

				}

		}  // end if ignore time up


	}  // end of state change

}


*/
void loop() {

	
}
