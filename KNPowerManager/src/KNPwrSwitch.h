#ifndef KNPwrSwitch_H
#define KNPwrSwitch_H

#include "Declarations.h"
#include "KNSwitchEvent.h"

/// <summary>
/// KNPowerManager's module controller.
/// </summary>
class KNPwrSwitch
{
private:
	String _name;
	uint8_t _dPin = 0;
	uint8_t _aPin = 0;
	uint8_t _pwrPin = 0;
	uint8_t _rstPin = 0;
	byte _moduleType = 0;
	
	float _realPower = 0;
	float _appaPower = 0;
	float _powerFactor = 0;
	bool _powerState = false;
	String _powerStateDesc;

	KNSwitchEvent* _switchingEvents[3];


public:
	inline const char* GetName() { return _name.c_str(); }
	inline float GetRealPower() { return _realPower; }
	inline float GetApparentPower() { return _appaPower; }
	inline float GetPowerFactor() { return _powerFactor; }
	inline bool GetPowerState() { return _powerState; }
	inline const char* GetPowerStateDesc() { return _powerStateDesc.c_str(); };
	
	/// <summary>
	/// Initializes a new instance of the <see cref="KNPwrSwitch"/> class.
	/// </summary>
	/// <param name="name">The name.</param>
	/// <param name="dPin">The d pin.</param>
	/// <param name="aPin">a pin.</param>
	/// <param name="pwrPin">The PWR pin.</param>
	/// <param name="rstPin">The RST pin.</param>
	/// <param name="moduleType">Type of the module.</param>
	KNPwrSwitch(String name, uint8_t dPin, uint8_t aPin, uint8_t pwrPin, uint8_t rstPin, byte moduleType = 0);
	
	/// <summary>
	/// Switches the power.
	/// </summary>
	/// <param name="on">if set to <c>true</c> [on].</param>
	void SwitchPower(bool on = true);
	
	/// <summary>
	/// Presses the power switch.
	/// </summary>
	/// <param name="duration">The duration.</param>
	void PressPowerSwitch(int duration);
	
	/// <summary>
	/// Presses the reset switch.
	/// </summary>
	/// <param name="duration">The duration.</param>
	void PressResetSwitch(int duration);
	
	/// <summary>
	/// Quicks the switch power.
	/// </summary>
	void QuickSwitchPower();
	
	/// <summary>
	/// Refreshes the power consumption.
	/// </summary>
	void RefreshPowerConsumption();
	
	/// <summary>
	/// Process (in running loop).
	/// </summary>
	void Process();
};

#endif
