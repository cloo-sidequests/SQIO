#include <Arduino.h>
#include <sqio.h>

SQIO::SQIO() {
	// Initialize class variables
	for (int i = 0; i <= SQIO_MAX_D; i++) {
		this->isOutput[i] = false;
		this->lastValD[i] = false;
	}
	for (int i = 0; i <= SQIO_MAX_A - SQIO_MIN_A; i++) {
		this->lastValA[i] = false;
	}
	this->analogThreshold = 512;
	this->debounceTimer = 50;
	this->nBuf = 0;
	this->timerActive = false;
}

// Sets the pinMode.  Also allows for some later error catching (inadvertent reading of an OUTPUT pin).
boolean SQIO::setMode(int pin, int mode) {
	if (pin < SQIO_MIN_D || pin > SQIO_MAX_D) {
		return false;
	}
	this->isOutput[pin] = (mode == OUTPUT) ? true : false;
	pinMode(pin, mode);
	return true;
}

// Sets the analog threshold for analogs masquerading as digitals.
void SQIO::setAnalogThreshold(int threshold) {
	if (threshold >= 0 && threshold <= SQIO_MAX_A_VAL) {
		this->analogThreshold = threshold;
	}
}

// Set a new debounce timer
void SQIO::setDebounce(int newTimer) {
    this->debounceTimer = newTimer;
}

// Convenience function to read a pin (analogs will be returned as analog values).
int SQIO::read(int pin) {
	return this->read(pin, false, false);
}

// Read the value of the given pin.  forceDigital = true will return analog pins as HIGH or LOW depending on if they exceed threshold.
// forceDigital has no effect on digital pins.
int SQIO::read(int pin, boolean forceDigital, boolean debounce) {
	if (pin >= SQIO_MIN_D && pin <= SQIO_MAX_D && !this->isOutput[pin]) {
		int v1 = digitalRead(pin);
		int v2 = v1;
		int ret = this->lastValD[pin];
		if (debounce) {
		    delay(this->debounceTimer);
		    v2 = digitalRead(pin);
		}
		if (v1 == v2) {
		    ret = v1;
		    this->lastValD[pin] = (v1 == HIGH);
		}
		/*int v = digitalRead(pin);
		this->lastValD[pin] = (v == HIGH);*/
		return ret;
	}
	else if (pin >= SQIO_MIN_A && pin <= SQIO_MAX_A) {
		pin = pin - 100;
		if (forceDigital) {
		    boolean v1 = (analogRead(pin) >= this->analogThreshold);
		    boolean v2 = v1;
		    int ret = this->lastValA[pin] ? HIGH : LOW;
		    if (debounce) {
		        delay(this->debounceTimer);
		        v2 = (analogRead(pin) >= this->analogThreshold);
		    }
		    if (v1 == v2) {
		        ret = v1 ? HIGH : LOW;
		        this->lastValA[pin] = v1;
		    }
			/*int v = analogRead(pin - 100);
			this->lastValA[pin] = (v >= this->analogThreshold);
			return v >= this->analogThreshold ? HIGH : LOW;*/
			return ret;
		}
		else {
			return analogRead(pin);
		}
	}
	else {
		return -1;
	}
}

// Check if the pin has gone HIGH since the last time it was read.
boolean SQIO::rise(int pin) {
    return this->rise(pin, false);
}
boolean SQIO::rise(int pin, boolean debounce) {
	if (pin >= SQIO_MIN_D && pin <= SQIO_MAX_D && !this->isOutput[pin]) {
	    int v1 = digitalRead(pin);
	    int v2 = v1;
	    boolean ret = false;
	    if (debounce) {
		    delay(this->debounceTimer);
		    v2 = digitalRead(pin);
	    }
	    if (v1 == v2) {
		    ret = (v1 == HIGH && !this->lastValD[pin]);
		    this->lastValD[pin] = (v1 == HIGH);
		}
		/*int v = digitalRead(pin);
		boolean ret = (v == HIGH && !this->lastValD[pin]);
		this->lastValD[pin] = (v == HIGH);*/
		return ret;
	}
	else if (pin >= SQIO_MIN_A && pin <= SQIO_MAX_A) {
		pin = pin - 100;
		boolean v1 = (analogRead(pin) >= this->analogThreshold);;
		boolean v2 = v1;
		boolean ret = false;
		if (debounce) {
		    delay(this->debounceTimer);
		    v2 = (analogRead(pin) >= this->analogThreshold);
		}
		if (v1 == v2) {
		    ret = (v1 && !this->lastValA[pin]);
		    this->lastValA[pin] = v1;
		}
		/*int v = analogRead(pin);
		boolean ret = (v >= this->analogThreshold && !this->lastValA[pin]);
		this->lastValA[pin] = (v >= this->analogThreshold);*/
		return ret;
	}
	else {
		return false;
	}
}

// Check if the pin has gone LOW since the last time it was read.
boolean SQIO::fall(int pin) {
    return this->fall(pin, false);
}
boolean SQIO::fall(int pin, boolean debounce) {
	if (pin >= SQIO_MIN_D && pin <= SQIO_MAX_D && !this->isOutput[pin]) {
		int v1 = digitalRead(pin);
	    int v2 = v1;
	    boolean ret = false;
	    if (debounce) {
		    delay(this->debounceTimer);
		    v2 = digitalRead(pin);
	    }
	    if (v1 == v2) {
		    ret = (v1 == LOW && this->lastValD[pin]);
		    this->lastValD[pin] = (v1 == HIGH);
		}
		return ret;
	}
	else if (pin >= SQIO_MIN_A && pin <= SQIO_MAX_A) {
		pin = pin - 100;
		boolean v1 = (analogRead(pin) >= this->analogThreshold);;
		boolean v2 = v1;
		boolean ret = false;
		if (debounce) {
		    delay(this->debounceTimer);
		    v2 = (analogRead(pin) >= this->analogThreshold);
		}
		if (v1 == v2) {
		    ret = (!v1 && this->lastValA[pin]);
		    this->lastValA[pin] = v1;
		}
		return ret;
	}
	else {
		return false;
	}
}

// Check if the pin has changed state since the last time it was read.
boolean SQIO::changed(int pin) {
    return this->changed(pin, false);
}
boolean SQIO::changed(int pin, boolean debounce) {
	if (pin >= SQIO_MIN_D && pin <= SQIO_MAX_D && !this->isOutput[pin]) {
	    int v1 = digitalRead(pin);
	    int v2 = v1;
	    boolean ret = false;
	    if (debounce) {
	        delay(this->debounceTimer);
	        v2 = digitalRead(pin);
	    }
	    if (v1 == v2) {
		    ret = ((v1 == LOW && this->lastValD[pin]) || (v1 == HIGH && !this->lastValD[pin]));
		    this->lastValD[pin] = (v1 == HIGH);
	    }
		/*int v = digitalRead(pin);
		boolean ret = ((v == LOW && this->lastValD[pin]) || (v == HIGH && !this->lastValD[pin]));
		this->lastValD[pin] = (v == HIGH);*/
		return ret;
	}
	else if (pin >= SQIO_MIN_A && pin <= SQIO_MAX_A) {
		pin = pin - 100;
		boolean v1 = (analogRead(pin) >= this->analogThreshold);
		boolean v2 = v1;
		boolean ret = false;
		if (debounce) {
		    delay(this->debounceTimer);
		    v2 = (analogRead(pin) >= this->analogThreshold);
		}
		if (v1 == v2) {
		    ret = ((!v1 && this->lastValA[pin]) || (v1 && !this->lastValA[pin]));
		    this->lastValA[pin] = v1;
		}
		/*int v = analogRead(pin);
		boolean ret = ((v < this->analogThreshold && this->lastValA[pin]) || (v >= this->analogThreshold && !this->lastValA[pin]));
		this->lastValA[pin] = (v >= this->analogThreshold);*/
		return ret;
	}
	else {
		return false;
	}
}

// Convenience function to set a digital output pin without a buffered timer
boolean SQIO::set(int pin, int val) {
	return this->set(pin, val, 0);
}

// Sets the given digital output pin to the given value.  If time > 0, then also subjects the pin to max buffer size,
// scheduling a future activation if necessary, and schedules the pin to revert after the given time.
// Use time = 0 to disable timer.
boolean SQIO::set(int pin, int val, unsigned long time) {
	/*Serial.print("pin=");
	Serial.print(pin);
	Serial.print(", val=");
	Serial.print(val);
	Serial.print(", time=");
	Serial.println(time);*/
	if (pin < SQIO_MIN_D || pin > SQIO_MAX_D || !this->isOutput[pin]) {
		return false;
	}
	
	// Check if a time applies.
	// If max number of active pins has been reached, then schedule a future activation if space allows.
	if (time > 0) {
		// If there are no timers active, then activate it immediately and schedule a de-activation.  Otherwise, schedule a future activation.
		if (this->timerActive) {
			// If out of buffer space, then just fail.
			if (this->nBuf >= SQIO_MAX_BUF) {
				return false;
			}
			this->buf[this->nBuf].pin = pin;
			this->buf[this->nBuf].val = val;
			this->buf[this->nBuf].time = time;
			this->nBuf++;
			return true;
		}
		this->timerActive = true;
		this->timer.pin = pin;
		this->timer.val = (val == HIGH) ? LOW : HIGH;
		this->timer.time = millis() + time;
	}
	
	digitalWrite(pin, val);
	return true;
}

// Should be called every time around loop().
// Checks if there are any active timered pins that have timed out, and if so, deactivates them.
// Checks if there are any timered pins in the buffer, and space for additional timered pins.  If so, activates them and schedules a future deactivation.
void SQIO::update() {
	unsigned long now = millis();
	
	// Check to deactivate timered pins.
	if (this->timerActive && this->timer.time <= now) {
		this->set(this->timer.pin, this->timer.val);
		this->timerActive = false;
	}
	
	// Check if there are timered pins awaiting activation (and there is space to activate them)
	if (this->nBuf > 0 && !this->timerActive) {
		this->nBuf--;
		this->set(this->buf[this->nBuf].pin, this->buf[this->nBuf].val, this->buf[this->nBuf].time);
	}
}

// Clear all timered pins
void SQIO::clearTimers() {
	if (this->timerActive) {
		this->set(this->timer.pin, this->timer.val);
		this->timerActive = false;
	}
	this->nBuf = 0;
}