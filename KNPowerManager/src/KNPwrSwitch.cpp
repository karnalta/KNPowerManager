#include "KNPwrSwitch.h"

KNPwrSwitch::KNPwrSwitch(String name, uint8_t dPin, uint8_t aPin, uint8_t pwrPin, uint8_t rstPin, byte moduleType = 0)
{
	_name = name;
	_dPin = dPin;
	_aPin = aPin;
	_pwrPin = pwrPin;
	_rstPin = rstPin;
	_moduleType = moduleType;
}

void KNPwrSwitch::SwitchPower(bool on)
{
	// Not Implemented
}

void KNPwrSwitch::QuickSwitchPower()
{
	// Switch module's relay
	if (!_isQuickSwitching)
	{
		KNLog::LogEvent(&(knpwrswitch_table[0]));

		_isQuickSwitching = true;
		_quickSwitchStart = millis();

		digitalWrite(_dPin, HIGH);
	}
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

		// Log measure
		if (DEBUG_MODE)
		{
			char message[256];
			strcpy_P(message, (char*)pgm_read_word(&(knpwrswitch_table[2])));
			sprintf(StringBuffer, message, _name.c_str(), ((String)_realPower).c_str(), ((String)_appaPower).c_str(), ((String)_powerFactor).c_str());
			KNLog::LogEvent(StringBuffer);
		}
	}
}

String KNPwrSwitch::GetPowerStateDesc()
{
	if (_powerState)
		strcpy_P(StringBuffer, (char*)pgm_read_word(&(knpwrswitch_table[3])));
	else
		strcpy_P(StringBuffer, (char*)pgm_read_word(&(knpwrswitch_table[4])));

	return StringBuffer;
}

void KNPwrSwitch::Process()
{
	// Check if quick switching is running
	if (_isQuickSwitching)
	{
		long diff = millis() - _quickSwitchStart;

		if (diff > 5000 || diff < 0)
		{
			_isQuickSwitching = false;
			_quickSwitchStart = 0;

			digitalWrite(_dPin, LOW);

			KNLog::LogEvent(&(knpwrswitch_table[1]));
		}
	}
}
