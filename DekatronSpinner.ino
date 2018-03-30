class DekatronStep
{
	int Guide1;   
	int Guide2;
	int Index;
	int previousGuideState;
	int stepDelay;
	
public:
	DekatronStep(int pin1, int pin2, int pin3)
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		
		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);
	}

void Update()
	{
		//Delay needed for there is not enough delay in the loop when calling.
		//if there is a serial print or the like in the loop then the delay is not needed.
		// minimum reliable pulse width of a dekatron seems to be about 40uS (in this code).
		// will need adjusting depending on processor speed. This is runing at 16mHz.
	
	delayMicroseconds(40); 

		switch (previousGuideState) {
		case 0:    
			previousGuideState = 1;
			digitalWrite(Guide1, LOW);
			digitalWrite(Guide2, LOW);
			break;
		case 1:    
			previousGuideState = 2;
			digitalWrite(Guide1, HIGH);
			digitalWrite(Guide2, LOW);
			break;
		case 2:    
			previousGuideState = 0;
			digitalWrite(Guide1, LOW);
			digitalWrite(Guide2, HIGH);
			break;
		}
	}
};


DekatronStep Dek1(52, 50, 48); //setup physical pins here. In this case 52 and 50 are G1 and G2. The index is 48.
DekatronStep Dek2(44, 42, 40);
DekatronStep Dek3(36, 34, 32);
DekatronStep Dek4(28, 26, 24);



// setup() runs once, at reset, to initialize system
void setup() {
}

// the loop function runs over and over again forever
void loop() {
	Dek1.Update();
	Dek2.Update();
	Dek3.Update();
	Dek4.Update();
}
