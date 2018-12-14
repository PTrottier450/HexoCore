
/* Stepper X control */
int Pin_X_Step = 23;
int Pin_X_Dir = 25;

/* Stepper Y control */
int Pin_Y_Step = 23;
int Pin_Y_Dir = 25;

void setup()
{

	Serial.begin(9600);

	/* Pin config for Stepper X */
	pinMode(Pin_X_Step, OUTPUT);
	pinMode(Pin_X_Dir, OUTPUT);

	/* Pin config for Stepper Y */
	pinMode(Pin_Y_Step, OUTPUT);
	pinMode(Pin_Y_Dir, OUTPUT);

}

void loop()
{

  /* add main program code here */

}
