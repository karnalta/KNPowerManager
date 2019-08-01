#include "KNPwrSwitch.h"

KNPwrSwitch::KNPwrSwitch(String name, uint8_t dPin, uint8_t aPin, uint8_t pwrPin, uint8_t rstPin, byte moduleType = 0)
{
	_name = name;
	_dPin = dPin;
	_aPin = aPin;
	_pwrPin = pwrPin;
	_rstPin = rstPin;
	_moduleType = moduleType;

	// Init switching events
	_switchingEvents[0] = new KNSwitchEvent(_dPin);
	_switchingEvents[1] = new KNSwitchEvent(_pwrPin);
	_switchingEvents[2] = new KNSwitchEvent(_rstPin);
}

void KNPwrSwitch::SwitchPower(bool on)
{
	// Not Implemented
}

void KNPwrSwitch::PressPowerSwitch(uint8_t duration)
{
	// Start switch event
	_switchingEvents[1]->SetDuration(duration);
	_switchingEvents[1]->Start();

	KNLog::LogEvent(&(knpwrswitch_table[2]));
}

void KNPwrSwitch::PressResetSwitch(uint8_t duration)
{
	// Start switch event
	_switchingEvents[2]->SetDuration(duration);
	_switchingEvents[2]->Start();

	KNLog::LogEvent(&(knpwrswitch_table[4]));
}

void KNPwrSwitch::QuickSwitchPower()
{
	// Start switch event
	_switchingEvents[0]->SetDuration(5000);
	_switchingEvents[0]->Start();

	KNLog::LogEvent(&(knpwrswitch_table[0]));
}

void KNPwrSwitch::RefreshPowerConsumption()
{
	uint32_t startTime = millis();

	int sampleCnt = 0;
	int sampleTotal = 0;

	// Pre-check to avoid sampling during 0.5 sec for nothing
	if (analogRead(_aPin) <= 5)
	{
		_realPower = 0;
		_appaPower = 0;
		_powerFactor = 0;

		_powerState = false;
	}
	else
	{
		// Sampling
		while (millis() - startTime < 500)
		{
			float value = analogRead(_aPin);

			sampleCnt++;
			sampleTotal += abs(value - (512 + PWR_5V_OFFSET));

			delay(1);
		}

		// Base calculation
		float voltAvg = (((float)sampleTotal / sampleCnt) * 5.0) / 1024.0;
		float currAvg = voltAvg / 1000; // (1000 = Burden Resistor (Ohms))

		// Compute REAL POWER AVERAGE (2000 = TA12-200 ratio)
		_realPower = ((currAvg * 2000) * 230) * PWR_FACTOR;

		// Compute RMS CURRENT AVERAGE
		float rmsCurrAvg = currAvg * 0.707;

		// Compute APPARENT POWER (2000 = TA12-200 ratio)
		_appaPower = ((rmsCurrAvg * 2000) * 230) * PWR_FACTOR;

		// Compute POWER FACTOR
		_powerFactor = _realPower / _appaPower;

		// Set power state
		_powerState = _realPower <= PWR_THRESHOLD ? false : true;

		// Update state description
		if (_powerState)
			strcpy_P(StringBuffer, (char*)pgm_read_word(&(knpwrswitch_table[3])));
		else
			strcpy_P(StringBuffer, (char*)pgm_read_word(&(knpwrswitch_table[4])));

		_powerStateDesc = StringBuffer;

		// Log measure
		if (DEBUG_MODE)
		{
			char message[256];
			strcpy_P(message, (char*)pgm_read_word(&(knpwrswitch_table[6])));
			sprintf(StringBuffer, message, _name.c_str(), ((String)_realPower).c_str(), ((String)_appaPower).c_str(), ((String)_powerFactor).c_str());
			KNLog::LogEvent(StringBuffer);
		}
	}
}

void KNPwrSwitch::Process()
{
	// Check all switching events
	for (int i = 0; i < 3; i++)
	{
		if (_switchingEvents[i]->Check())
		{
			_switchingEvents[i]->Reset();
			KNLog::LogEvent(&(knpwrswitch_table[(i * 2) + 1]));
		}
	}
}
