class dekatronStep
{
	int Guide1;   
	int Guide2;
	int Index;
	int previousGuideState;
	int stepDelay;
	bool clockwise;
	unsigned long previousMillis;
	
public:
	dekatronStep(int pin1, int pin2, int pin3,int sDelay,bool direction)
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

void updateStep()
	{
		//Delay needed if there is not enough delay in the loop when calling.
		// will need adjusting depending on processor speed. This is runing at 16mHz.
	
	delayMicroseconds(40); 

	unsigned long currentMillis = millis();

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

dekatronStep Dek1(52, 50, 48,0,false); //setup physical pins here. In this case 52 and 50 are G1 and G2. The index is 48.
dekatronStep Dek2(44, 42, 40,5,true);
dekatronStep Dek3(36, 34, 32,10,true);
dekatronStep Dek4(28, 26, 24,1000,true);

// setup() runs once, at reset, to initialize system
void setup() {
}

// the loop function runs over and over again forever
void loop() {
	Dek1.updateStep();
	Dek2.updateStep();
	Dek3.updateStep();
	Dek4.updateStep();
}
