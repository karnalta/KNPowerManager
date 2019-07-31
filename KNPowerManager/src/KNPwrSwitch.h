#ifndef KNPwrSwitch_H
#define KNPwrSwitch_H

#include "Declarations.h"

/**
 * @brief KN Power Module.
 * 
 */
class KNPwrSwitch
{
private:
	String _name;
	uint8_t _dPin = 0;
	uint8_t _aPin = 0;
	uint8_t _pwrPin = 0;
	uint8_t _rstPin = 0;
	byte _moduleType = 0;

	long _quickSwitchStart = 0;
	bool _isQuickSwitching = false;

	long _pwrSwitchStart = 0;
	bool _isPwrSwitching = false;

	long _rstSwitchStart = 0;
	bool _isRstSwitching = false;
	
	float _realPower = 0;
	float _appaPower = 0;
	float _powerFactor = 0;
	bool _powerState = false;

public:
	inline const char* GetName() { return _name.c_str(); }
	inline bool GetQuickSwitchState() { return _isQuickSwitching; }
	inline float GetRealPower() { return _realPower; }
	inline float GetApparentPower() { return _appaPower; }
	inline float GetPowerFactor() { return _powerFactor; }
	inline bool GetPowerState() { return _powerState; }
	String GetPowerStateDesc();

	/**
	 * @brief Construct a new KNPwrSwitch object.
	 * 
	 * @param name Debug name
	 * @param dPin Relay pin
	 * @param aPin Power consumption pin
	 * @param pwrPin PowerSwitch pin
	 * @param rstPin ResetSwitch pin
	 * @param moduleType Module type (Not implemented)
	 */
	KNPwrSwitch(String name, uint8_t dPin, uint8_t aPin, uint8_t pwrPin, uint8_t rstPin, byte moduleType = 0);

	/**
	 * @brief Permanently switch relay on or off.
	 * 
	 * @param on True if switch on
	 */
	void SwitchPower(bool on = true);

	/**
	 * @brief Press the power switch button.
	 * 
	 * @param duration Duration in seconds
	 */
	void PressPowerSwitch(uint8_t duration);

	/**
	 * @brief Press the reset switch button.
	 * 
	 * @param duration Duration in seconds
	 */
	void PressResetSwitch(uint8_t duration);

	/**
	 * @brief Switch off power relay for 5 seconds.
	 * 
	 */
	void QuickSwitchPower();

	/**
	 * @brief Refresh the power consumption data.
	 * 
	 */
	void RefreshPowerConsumption();

	/**
	 * @brief Process pending operation.
	 * 
	 */
	void Process();
};

#endif
