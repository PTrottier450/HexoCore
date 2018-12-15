
/* Stepper X control */
int Pin_X_Step = 23;
int Pin_X_Dir = 25;

/* Stepper Y control */
int Pin_Y_Step = 23;
int Pin_Y_Dir = 25;

/* Serial related */
#define MAX_BUFFER 64
char Buffer[MAX_BUFFER];

void setup()
{
	/* Serial command line begin */
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
	// listen for commands
	if (Serial.available())
	{
		char c = Serial.read(); 
		Serial.print(c); // optional: repeat back what I got for debugging

						 // store the byte as long as there's room in the buffer.
						 // if the buffer is full some data might get lost
		if (sofar < MAX_BUF) buffer[sofar++] = c;

		// if we got a return character (\n) the message is done.
		if (c == '\n')
		{
			processCommand(); // do something with the command
			ready();
		}
	}
}

bool processCommand(char * codePointer)
{
	/* Start reading the first letter to determine the command */
	switch (codePointer[0])
		case 'G':	/* Motor Command */
			PositionCommand(codePointer);
			break;
		default:	/* Unsupported Command */
			Serial.println("Command not supported !");
			break;
}

bool PositionCommand(char * codePointer)
{
	char BufferArray[MAX_BUFFER];
	int i = 2;
	int j = 0;
	float moveValue;

	switch (codePointer[2])
		case 0: /* Rapid Motion */
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
			moveValue = (float)atof(BufferArray);
			MoveX(moveValue);

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
			moveValue = (float)atof(BufferArray);
			MoveY(moveValue);

			break;

		case 1: /* Controlled Rapid Motion */
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
			moveValue = (float)atof(BufferArray);
			MoveX(moveValue);

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
			moveValue = (float)atof(BufferArray);
			MoveY(moveValue);

			break;

		case 2: /* Clockwise Motion */

			break;

		case 3: /* Counter Clockwise repositionning */

			break;

		case 28 : /* Return to home */

			break;

}

MoveLine(int16 Px,int16 Py)
{
	int dx = Px - newx;
	int dy = Py - newy;
	bool direction = if (dx < 0) ? 1 : 0;	// 1 = Reverse : 0 = Forward
	bool direction = if (dy < 0) ? 1 : 0;	// 1 = Reverse : 0 = Forward
	dx = abs(dx);
	dy = abs(dy);

	int over = 0;

	if (dx>dy) {
		over = dx / 2;
		for (i = 0; i<dx; ++i) {
			StepX(dirx);
			over += dy;
			if (over >= dx) {
				over -= dx;
				StepY(diry);
			}
			delay(10);
		}
	}
	else {
		over = dy / 2;
		for (i = 0; i<dy; ++i) {
			StepY(diry);
			over += dx;
			if (over >= dy) {
				over -= dy;
				StepX(dirx);
			}
			delay(10);
		}
	}

	Px = newx;
	Py = newy;
}

MoveLineSpeed()
{

}

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