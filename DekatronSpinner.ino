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
		pinMode(Index, INPUT_PULLUP);
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
		} // end of switch case

	}

	
}

};

dekatronStep Dek1(52, 50, 48,true,0); //setup physical pins here. In this case 52 and 50 are G1 and G2. The index is 48.
dekatronStep Dek2(44, 42, 40,true,3);
dekatronStep Dek3(36, 34, 32,true,50);
dekatronStep Dek4(28, 26, 24,true,10);

int IndexCount = 0;

byte oldIndexState = HIGH;  // assume index open because of pull-up resistor
const unsigned long ignoreTime = 5;  // milliseconds
unsigned long indexHighTime;  // when the index last changed state




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
	unsigned long currentMillis = millis();

	Dek1.updateStep(currentMillis);
	Dek2.updateStep(currentMillis);
	Dek3.updateStep(currentMillis);
	Dek4.updateStep(currentMillis);

}

// the loop function runs over and over again forever
void loop() {

//	if (digitalRead(Dek4.Index)) 
//	{
//		digitalWrite(LED_BUILTIN, HIGH);
//		Serial.println(" Index");
//		Serial.print(IndexCount);
//		IndexCount++;
//	}
//	else digitalWrite(LED_BUILTIN, LOW);


	// see if Index is High or Low
	byte indexState = digitalRead(Dek4.Index);

	// has index state changed since last time?
	if (indexState != oldIndexState)
	{
		// ignore time.
		if (millis() - indexHighTime >= ignoreTime)
		{
			indexHighTime = millis();  // when index was high
			oldIndexState = indexState;  // remember for next time 
			if (indexState == LOW)
			{
				Serial.println("indexState low.");
				digitalWrite(LED_BUILTIN, HIGH);
			}  // end if indexState is LOW
			else
			{
				Serial.println("indexState high.");
				digitalWrite(LED_BUILTIN, LOW);
			}  // end if indexState is HIGH

		}  // end if ignore time up

		Serial.println(IndexCount);
		IndexCount++;

	}  // end of state change

		
	//Serial.println(digitalRead(Dek4.Index));

	//Dek4.clockwise = false;

	//if (digitalRead(Dek4.Index)) Serial.println("Dek4 Index");
	
	//if (Dek4.clockwise = true);
	//Serial.println("Dek4 Clockwise");


	//else if Dek3.clockwise = true;
	



}


