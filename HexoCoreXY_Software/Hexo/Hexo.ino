#include <Servo.h>

/* Servo for Pen Actuation */
Servo servopen;

/* Pinout is based on the RAMPS 1.4 configuration */

/* Stepper X control */
int Pin_X_Step = 54;	//Pin A0
int Pin_X_Dir = 55;		//Pin A1
int Pin_X_En = 38;		//Pin D38

/* Stepper Y control */
int Pin_Y_Step = 60;	//Pin A6
int Pin_Y_Dir = 61;		//Pin A7
int Pin_Y_En = 56;		//Pin A2

/* Stepper movement precision */
float stepPrecision = 0.005; // 0.005mm movement per step see StepX/StepY for details

/* Serial related */
#define MAX_BUFFER 64
char buffer[MAX_BUFFER];

void setup()
{
	/* Serial command line begin */
	Serial.begin(9600);
  Serial.println("Begin !");

	/* Pin config for Stepper X */
	pinMode(Pin_X_Step, OUTPUT);
	pinMode(Pin_X_Dir, OUTPUT);
	pinMode(Pin_X_En, OUTPUT);

	/* Pin config for Stepper Y */
	pinMode(Pin_Y_Step, OUTPUT);
	pinMode(Pin_Y_Dir, OUTPUT);
	pinMode(Pin_Y_En, OUTPUT);

	/* Start Servo module on pin 4 */
	servopen.attach(4);

	/* Enable Stepper drivers */
	digitalWrite(Pin_X_En, LOW);
	digitalWrite(Pin_Y_En, LOW);

}

void loop()
{
	long sofar = 0;
	char dummy[15] = "G00 X1.75 Y1.75";
	char * dummypointer;
	dummypointer = dummy;
	processCommand(dummypointer); // do something with the command
  delay(1000);
}

bool processCommand(char * codePointer)
{
	/* Start reading the first letter to determine the command */
	switch (codePointer[0]) {
	case 'G':	/* Motor Command */
    Serial.println(codePointer[0]);
		PositionCommand(codePointer);
		break;
	default:	/* Unsupported Command */
		Serial.println("Command not supported !");
		break;
	}
}

bool PositionCommand(char * codePointer)
{
	char BufferArray[MAX_BUFFER];
	int i = 2;
	int j = 0;
	float moveValue;
	float moveValueX, moveValueY;
  Serial.println(codePointer[2]);
	switch (codePointer[2]) {
	case '0': /* Rapid Motion G00 */
    Serial.println("CASE 0");
		while (codePointer[i] != 'X')
		{
			i++;
		}

		j = 0;
		while (codePointer[i] != ' ')
		{
			i++;
			BufferArray[j] = codePointer[i];
      j++;
		}
		moveValueX = (float)atof(BufferArray);

		while (codePointer[i] != 'Y')
		{
			i++;
		}

		j = 0;
		while (codePointer[i] != ' ')
		{
			i++;
			BufferArray[j] = codePointer[i];
      j++;
		}
		moveValueY = (float)atof(BufferArray);
    
		MoveLine(moveValueX, moveValueY);

    ActivatePen();
    DeactivatePen();

		break;

	case '1': /* Controlled Rapid Motion G01 */
    Serial.println("CASE 1");
		while (codePointer[i] != 'X')
		{
			i++;
		}

		j = 0;
		while (codePointer[i] != ' ')
		{
			i++;
			BufferArray[j] = codePointer[i];
		}
		moveValueX = (float)atof(BufferArray);

		while (codePointer[i] != 'Y')
		{
			i++;
		}

		j = 0;
		while (codePointer[i] != ' ')
		{
			i++;
			BufferArray[j] = codePointer[i];
		}
		moveValueY = (float)atof(BufferArray);
		MoveLine(moveValueX, moveValueY);

		break;

	case '2': /* Clockwise Motion */

		break;

	case '3': /* Counter Clockwise repositionning */

		break;

	case 28: /* Return to home */

		break;

  default:
    Serial.println("INVALID CASE");
    break;
  
	}
}

void MoveLine(float newx,float newy)
{
	int Px,Py = 0;
	int dx = Px - newx;
	int dy = Py - newy;
	bool directionX = 0;
	bool directionY = 0;
	(dx < 0) ? directionX = 1 : directionX = 0;	// 1 = Reverse : 0 = Forward
	(dy < 0) ? directionY = 1 : directionY = 0;	// 1 = Reverse : 0 = Forward
	dx = abs(dx);
	dy = abs(dy);

	int over = 0;

  Serial.println("MovingLine");

	if (dx>dy) {
		over = dx / 2;
		for (int i = 0; i<dx; ++i) {
			StepX(dx, directionX);
			over += dy;
			if (over >= dx) {
				over -= dx;
				StepY(dy, directionY);
			}
		}
	}
	else {
		over = dy / 2;
		for (int i = 0; i<dy; ++i) {
			StepY(dy, directionY);
			over += dx;
			if (over >= dy) {
				over -= dy;
				StepX(dx, directionX);
			}
		}
	}

	Px = newx;
	Py = newy;
}

void StepX(float move,bool direction)
{
	/* GT2 Pulley OD : 16mm (Spool3d GT2-1605) */
	/* Stepper precision : 1.8deg per step + A4988 1/16 step -> 0.1125 deg -> 3200 step for 1 turn */
	/* Movement : 3200 step for 16mm -> 0.005mm for 1 step */
	if (direction == 0)
	{
		digitalWrite(Pin_X_Dir, HIGH);
    Serial.println("Dir X HIGH");
	}
	else if (direction == 1)
	{
		digitalWrite(Pin_X_Dir, LOW );
    Serial.println("Dir X LOW");
	}
	float distance = move / stepPrecision;

	for (int i = 0; i < distance; i++)
	{
		digitalWrite(Pin_X_Step, HIGH);
    delay(1);
		digitalWrite(Pin_X_Step, LOW);
	}
}

void StepY(float move, bool direction)
{
	/* GT2 Pulley OD : 16mm (Spool3d GT2-1605) */
	/* Stepper precision : 1.8deg per step + A4988 1/16 step -> 0.1125 deg -> 3200 step for 1 turn */
	/* Movement : 3200 step for 16mm -> 0.005mm for 1 step */
	if (direction == 0)
	{
		digitalWrite(Pin_Y_Dir, HIGH);
    Serial.println("Dir Y HIGH");
	}
	else if (direction == 1)
	{
		digitalWrite(Pin_Y_Dir, LOW);
    Serial.println("Dir Y LOW");
	}
	float distance = move / stepPrecision;

	for (int i = 0; i < distance; i++)
	{
		digitalWrite(Pin_Y_Step, HIGH);
    delay(1);
		digitalWrite(Pin_Y_Step, LOW);
	}
}

void ActivatePen()		/* Activate Servo for pen control */
{
	servopen.write(90);
}

void DeactivatePen()	/* Deactivate Servo for pen control */
{
	servopen.write(10);
}
/*
MoveLineSpeed()
{

}
*/

/*
void arc(float cx, float cy, float x, float y, float dir) {
	// get radius
	float dx = px - cx;
	float dy = py - cy;
	float radius = sqrt(dx*dx + dy * dy);

	// find angle of arc (sweep)
	float angle1 = atan3(dy, dx);
	float angle2 = atan3(y - cy, x - cx);
	float theta = angle2 - angle1;

	if (dir>0 && theta<0) angle2 += 2 * PI;
	else if (dir<0 && theta>0) angle1 += 2 * PI;

	theta = angle2 - angle1;

	// get length of arc
	// float circ=PI*2.0*radius;
	// float len=theta*circ/(PI*2.0);
	// simplifies to
	float len = abs(theta) * radius;

	int i, segments = ceil(len * MM_PER_SEGMENT);

	float nx, ny, angle3, scale;

	for (i = 0; i<segments; ++i) {
		// interpolate around the arc
		scale = ((float)i) / ((float)segments);

		angle3 = (theta * scale) + angle1;
		nx = cx + cos(angle3) * radius;
		ny = cy + sin(angle3) * radius;
		// send it to the planner
		line(nx, ny);
	}

	line(x, y);
}
*/
