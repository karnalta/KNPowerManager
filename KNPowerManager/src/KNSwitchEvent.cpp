#include "KNSwitchEvent.h"

KNSwitchEvent::KNSwitchEvent(uint8_t dPin)
{
	_dPin = dPin;
}

void KNSwitchEvent::Start()
{
	// Start pin digital write
	if (!_started)
	{
		_started = true;
		_completed = false;
		_startTime = millis();

		digitalWrite(_dPin, HIGH);
	}
}

void KNSwitchEvent::Reset()
{
	// Reset to default
	_started = false;
	_completed = false;
	digitalWrite(_dPin, LOW);
}

bool KNSwitchEvent::Check()
{
	// Check duration if pin is on
	if (_started && !_completed)
	{
		long diff = millis() - _startTime;

		if (diff > _duration || diff < 0)
		{
			_completed = true;
			_started = false;

			digitalWrite(_dPin, LOW);
		}
	}

	return _completed;
}
