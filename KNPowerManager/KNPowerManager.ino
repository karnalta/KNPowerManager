/*

KNPowerManager Project.

Designed for Arduino MEGA + Ethernet Shield + RTC module.

By MATHIEU Dimitri.

*/

#include <Arduino.h>

#include "src/Declarations.h"
#include "src/KNRest.h"
#include "src/KNTaskScheduler.h"
#include "src/KNWebServer.h"
#include "src/KNPwrSwitch.h"
#include "src/KNClock.h"
#include "src/KNCardStorage.h"

// TCP/IP Settings
byte TCP_MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte TCP_IP[] = { 192, 168, 1, 240 };
byte TCP_GATEWAY[] = { 192, 168, 1, 1 };
byte TCP_SUBNET[] = { 255, 255, 255, 0 };

// Controllers
KNRest* _knRest = NULL;
KNTaskScheduler* _knTaskScheduler = NULL;
KNWebServer* _knWebServer = NULL;
KNPwrSwitch** _knPowerSwitches = NULL;
KNClock* _knClock = NULL;
KNCardStorage* _knCardStorage = NULL;

// Callback functions predeclarations
/*void TaskCallBack_UpdateNTP();
void TaskCallBack_RefreshModules();
void RESTCallBack_ResetPSU(Vector<FuncParam*>);
void RESTCallBack_PressPwr(Vector<FuncParam*>);
void RESTCallBack_PressRst(Vector<FuncParam*>);*/

/// <summary>
/// Try to parse requiered parameters from params vector.
/// </summary>
/// <param name="params">The parameters.</param>
/// <param name="id">The identifier.</param>
/// <param name="duration">The duration.</param>
void ParseParams(Vector<FuncParam*>* params, int* id, int* duration)
{
	for (int i = 0; i < params->Size(); i++)
	{
		(*params)[i]->Name.toLowerCase();
		if ((*params)[i]->Name == "id")
			*id = (*params)[i]->Value.toInt();
		else if ((*params)[i]->Name == "duration")
			*duration = (*params)[i]->Value.toInt();
	}
}

/// <summary>
/// Refresh modules data.
/// </summary>
void TaskCallBack_RefreshModules()
{
	// Check consumption
	for (int i = 0; i < PWR_SWITCH_CNT; i++)
		_knPowerSwitches[i]->RefreshPowerConsumption();
}

/// <summary>
/// Update datetime from NTP server.
/// </summary>
void TaskCallBack_UpdateNTP()
{
	_knClock->UpdateFromNTPServer();
}

/// <summary>
/// Reset the PSU controlled by a specific module.
/// </summary>
/// <param name="params">The parameters.</param>
void RESTCallBack_ResetPSU(Vector<FuncParam*> params)
{
	int moduleId = -1;
	int duration = -1;
	ParseParams(&params, &moduleId, &duration);

	// Start operation
	if (moduleId > -1 && moduleId < (PWR_SWITCH_CNT - 1))
		_knPowerSwitches[moduleId]->QuickSwitchPower();
	else
		KNLog::LogEvent(&(global_table[1]));
}

/// <summary>
/// Press power button for x seconds from a specific module.
/// </summary>
/// <param name="params">The parameters.</param>
void RESTCallBack_PressPwr(Vector<FuncParam*> params)
{
	int moduleId = -1;
	int duration = -1;
	ParseParams(&params, &moduleId, &duration);

	// Start operation
	if (moduleId > -1 && moduleId < (PWR_SWITCH_CNT - 1) && duration > 0)
		_knPowerSwitches[moduleId]->PressPowerSwitch(duration);
	else
		KNLog::LogEvent(&(global_table[1]));
}

/// <summary>
/// Press reset button for x seconds from a specific module.
/// </summary>
/// <param name="params">The parameters.</param>
void RESTCallBack_PressRst(Vector<FuncParam*> params)
{
	int moduleId = -1;
	int duration = -1;
	ParseParams(&params, &moduleId, &duration);

	// Start operation
	if (moduleId > -1 && moduleId < (PWR_SWITCH_CNT - 1) && duration > 0)
		_knPowerSwitches[moduleId]->PressResetSwitch(duration);
	else
		KNLog::LogEvent(&(global_table[1]));
}

/// <summary>
/// Get power consumption from a specific module.
/// </summary>
/// <param name="params">The parameters.</param>
Vector<FuncParam*> RESTCallback_GetPowerConsumption(Vector<FuncParam*> params)
{
	int moduleId = -1;
	int duration = -1;
	ParseParams(&params, &moduleId, &duration);

	Vector<FuncParam*> result;

	// Start operation
	if (moduleId > -1 && moduleId < (PWR_SWITCH_CNT - 1))
	{
		float pc = _knPowerSwitches[moduleId]->GetApparentPower();
		float pk = _knPowerSwitches[moduleId]->GetRealPower();
		bool po = _knPowerSwitches[moduleId]->GetPowerState();

		FuncParam* pairA = new FuncParam("PowerOn", (String)po);
		result.PushBack(pairA);
		FuncParam* pairB = new FuncParam("Consumption", (String)pc);
		result.PushBack(pairB);
		FuncParam* pairC = new FuncParam("PowerPeak", (String)pk);
		result.PushBack(pairC);

		return result;
	}
	else
		KNLog::LogEvent(&(global_table[1]));

	return result;
}

/// <summary>
/// ATmega2560 setup.
/// </summary>
void setup()
{
  // Serial debug
  if (DEBUG_MODE) { Serial.begin(9600); }

  // Welcome
  KNLog::LogEvent(&(global_table[0]));

  // Init Pins
  pinMode(PIN_RELAY01, OUTPUT);
  pinMode(PIN_RELAY02, OUTPUT);
  pinMode(PIN_RELAY03, OUTPUT);
  pinMode(PIN_RELAY04, OUTPUT);
  pinMode(PIN_RELAY05, OUTPUT);
  pinMode(PIN_RELAY06, OUTPUT);
  pinMode(PIN_RELAY07, OUTPUT);
  pinMode(PIN_RELAY08, OUTPUT);
  pinMode(PIN_PWRSW01, OUTPUT);
  pinMode(PIN_PWRSW02, OUTPUT);
  pinMode(PIN_PWRSW03, OUTPUT);
  pinMode(PIN_PWRSW04, OUTPUT);
  pinMode(PIN_PWRSW05, OUTPUT);
  pinMode(PIN_PWRSW06, OUTPUT);
  pinMode(PIN_PWRSW07, OUTPUT);
  pinMode(PIN_PWRSW08, OUTPUT);
  pinMode(PIN_RSTSW01, OUTPUT);
  pinMode(PIN_RSTSW02, OUTPUT);
  pinMode(PIN_RSTSW03, OUTPUT);
  pinMode(PIN_RSTSW04, OUTPUT);
  pinMode(PIN_RSTSW05, OUTPUT);
  pinMode(PIN_RSTSW06, OUTPUT);
  pinMode(PIN_RSTSW07, OUTPUT);
  pinMode(PIN_RSTSW08, OUTPUT);
  pinMode(PIN_PWRSS01, INPUT);
  pinMode(PIN_PWRSS02, INPUT);
  pinMode(PIN_PWRSS03, INPUT);
  pinMode(PIN_PWRSS04, INPUT);
  pinMode(PIN_PWRSS05, INPUT);
  pinMode(PIN_PWRSS06, INPUT);
  pinMode(PIN_PWRSS07, INPUT);
  pinMode(PIN_PWRSS08, INPUT);
  pinMode(PIN_TMPSS01, INPUT);
  pinMode(PIN_TMPSS02, INPUT);
  pinMode(PIN_TMPSS03, INPUT);

  // Init RESTful API
  _knRest = new KNRest(TCP_IP, TCP_MAC, TCP_GATEWAY, TCP_SUBNET);

  // Init PowerSwitch Modules
  _knPowerSwitches = new KNPwrSwitch*[PWR_SWITCH_CNT]{ new KNPwrSwitch("Module 01", PIN_RELAY01, PIN_PWRSS01, PIN_PWRSW01, PIN_RSTSW01, PWR_MODULE_01),
													   new KNPwrSwitch("Module 02", PIN_RELAY02, PIN_PWRSS02, PIN_PWRSW02, PIN_RSTSW02, PWR_MODULE_02),
													   new KNPwrSwitch("Module 03", PIN_RELAY03, PIN_PWRSS03, PIN_PWRSW03, PIN_RSTSW03, PWR_MODULE_03),
													   new KNPwrSwitch("Module 04", PIN_RELAY04, PIN_PWRSS04, PIN_PWRSW04, PIN_RSTSW04, PWR_MODULE_04),
													   new KNPwrSwitch("Module 05", PIN_RELAY05, PIN_PWRSS05, PIN_PWRSW05, PIN_RSTSW05, PWR_MODULE_05),
													   new KNPwrSwitch("Module 06", PIN_RELAY06, PIN_PWRSS06, PIN_PWRSW06, PIN_RSTSW06, PWR_MODULE_06),
													   new KNPwrSwitch("Module 07", PIN_RELAY07, PIN_PWRSS07, PIN_PWRSW07, PIN_RSTSW07, PWR_MODULE_07),
													   new KNPwrSwitch("Module 08", PIN_RELAY08, PIN_PWRSS08, PIN_PWRSW08, PIN_RSTSW08, PWR_MODULE_08)};
  // Init SD card
  _knCardStorage = new KNCardStorage();

  // Register REST functions
  _knRest->AddFunc("ResetPSU", &RESTCallBack_ResetPSU);
  _knRest->AddFunc("PressPwr", &RESTCallBack_PressPwr);
  _knRest->AddFunc("PressRst", &RESTCallBack_PressRst);
  _knRest->AddFunc("GetPowerConsumption", &RESTCallback_GetPowerConsumption);

  // Init RTC clock
  _knClock = new KNClock();
  _knClock->UpdateFromNTPServer();

  // Init KNLog global variables
  LogClock = _knClock;
  LogCardStorage = _knCardStorage;

  // Init TaskScheduler
  _knTaskScheduler = new KNTaskScheduler();

  // Register tasks
  _knTaskScheduler->AddTask(new KNTask("Check Power Modules", PWR_CHECK_INTER, &TaskCallBack_RefreshModules, false));
  _knTaskScheduler->AddTask(new KNTask("Update time from NTP server", 0, 30, 0, &TaskCallBack_UpdateNTP));
}

/// <summary>
/// ATmega2560 main running loop.
/// </summary>
void loop()
{
	// Process Requests
	_knRest->Process();

	// Process Tasks
	_knTaskScheduler->Process();

	// Process Modules
	for (int i = 0; i < PWR_SWITCH_CNT; i++)
		_knPowerSwitches[i]->Process();

	delay(100);
}