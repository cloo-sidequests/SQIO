#ifndef SQIO_H
#define SQIO_H

// Aliases for the IO pins (Uno only does up to D13/A5; Mega does up to D53/A15)
#define SQD0 0
#define SQD1 1
#define SQD2 2
#define SQD3 3
#define SQD4 4
#define SQD5 5
#define SQD6 6
#define SQD7 7
#define SQD8 8
#define SQD9 9
#define SQD10 10
#define SQD11 11
#define SQD12 12
#define SQD13 13
#define SQD14 14
#define SQD15 15
#define SQD16 16
#define SQD17 17
#define SQD18 18
#define SQD19 19
#define SQD20 20
#define SQD21 21
#define SQD22 22
#define SQD23 23
#define SQD24 24
#define SQD25 25
#define SQD26 26
#define SQD27 27
#define SQD28 28
#define SQD29 29
#define SQD30 30
#define SQD31 31
#define SQD32 32
#define SQD33 33
#define SQD34 34
#define SQD35 35
#define SQD36 36
#define SQD37 37
#define SQD38 38
#define SQD39 39
#define SQD40 40
#define SQD41 41
#define SQD42 42
#define SQD43 43
#define SQD44 44
#define SQD45 45
#define SQD46 46
#define SQD47 47
#define SQD48 48
#define SQD49 49
#define SQD50 50
#define SQD51 51
#define SQD52 52
#define SQD53 53
#define SQA0 100
#define SQA1 101
#define SQA2 102
#define SQA3 103
#define SQA4 104
#define SQA5 105
#define SQA6 106
#define SQA7 107
#define SQA8 108
#define SQA9 109
#define SQA10 110
#define SQA11 111
#define SQA12 112
#define SQA13 113
#define SQA14 114
#define SQA15 115

#define SQIO_MIN_D 0
#define SQIO_MAX_D 53
#define SQIO_MIN_A 100
#define SQIO_MAX_A 115
#define SQIO_MAX_BUF 8
#define SQIO_MAX_A_VAL 1023

class SQIO {
public:
	SQIO();
	boolean setMode(int pin, int mode);				// Set the mode (INPUT/OUTPUT) of the given pin.  Return false if not a D pin
	void setAnalogThreshold(int threshold);			// Set the analog threshold for use as a simulated digital pin
	void setDebounce(int newTimer);                 // Set a new debounce timer
	int read(int pin);								// Read the value of the given pin.  Digital returns 0/1 (-1 if err); analog returns 0-1023 (-1 if err)
	int read(int pin, boolean forceDigital, boolean debounce);		// Read the value of the given pin.  forceDigital = analog values are returned as if they were digital.  Debounce if desired.
	boolean rise(int pin);							// Check if the pin has gone HIGH since the last time it was read.
	boolean rise(int pin, boolean debounce);
	boolean fall(int pin);							// Check if the pin has gone LOW since the last time it was read.
	boolean fall(int pin, boolean debounce);
	boolean changed(int pin);						// Check if the pin has changed state since the last time it was read.
	boolean changed(int pin, boolean debounce);
	boolean set(int pin, int val);					// Set the given pin to the given value (HIGH/LOW).  Return false if error (not an output pin)
	boolean set(int pin, int val, unsigned long time);		// Set the given pin to the given value (HIGH/LOW); will revert to the other value after time milliseconds.  Return false if fail or buffer full.
	void update();									// Do an update on timered pins.  Run this once every loop.
	void clearTimers();								// Deactivate all timered pins
	
private:
	typedef struct {
		int pin;
		int val;
		unsigned long time;
	} SqioBuf;
	boolean isOutput[SQIO_MAX_D + 1];
	int analogThreshold;
	int debounceTimer;
	int nBuf;
	SqioBuf buf[SQIO_MAX_BUF];
	boolean timerActive;
	SqioBuf timer;
	boolean lastValD[SQIO_MAX_D + 1];
	boolean lastValA[SQIO_MAX_A - SQIO_MIN_A + 1];
};

#endif