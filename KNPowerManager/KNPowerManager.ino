/*
    Name:       KNPowerManager.ino
    Created:    29-05-19 10:27:22
    Author:     MATHIEU Dimitri
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

/// <summary>
/// Task to update power modules data.
/// </summary>
void TASK_CheckPowerModules()
{
	// Check consumption
	for (int i = 0; i < PWR_SWITCH_CNT; i++)
		_knPowerSwitches[i]->RefreshPowerConsumption();
}

/// <summary>
/// Tasks to update the datetime from NTP server.
/// </summary>
void TASK_UpdateNTPTime()
{
	_knClock->UpdateFromNTPServer();
}

/// <summary>
/// ATmega2560 setup.
/// </summary>
void setup()
{
  // Serial debug
  if (DEBUG_MODE) { Serial.begin(9600); }

  // Init Pins
  pinMode(PIN_RELAY01, OUTPUT);
  pinMode(PIN_RELAY02, OUTPUT);
  pinMode(PIN_RELAY03, OUTPUT);
  pinMode(PIN_RELAY04, OUTPUT);
  pinMode(PIN_RELAY05, OUTPUT);
  pinMode(PIN_RELAY06, OUTPUT);
  pinMode(PIN_RELAY07, OUTPUT);
  pinMode(PIN_RELAY08, OUTPUT);
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
  _knPowerSwitches = new KNPwrSwitch*[PWR_SWITCH_CNT]{ new KNPwrSwitch("Module 01", PIN_RELAY01, PIN_PWRSS01, PIN_PWRSW01, PIN_RSTSW01),
													   new KNPwrSwitch("Module 02", PIN_RELAY02, PIN_PWRSS02, PIN_PWRSW02, PIN_RSTSW02),
													   new KNPwrSwitch("Module 03", PIN_RELAY03, PIN_PWRSS03, PIN_PWRSW03, PIN_RSTSW03),
													   new KNPwrSwitch("Module 04", PIN_RELAY04, PIN_PWRSS04, PIN_PWRSW04, PIN_RSTSW04),
													   new KNPwrSwitch("Module 05", PIN_RELAY05, PIN_PWRSS05, PIN_PWRSW05, PIN_RSTSW05),
													   new KNPwrSwitch("Module 06", PIN_RELAY06, PIN_PWRSS06, PIN_PWRSW06, PIN_RSTSW06),
													   new KNPwrSwitch("Module 07", PIN_RELAY07, PIN_PWRSS07, PIN_PWRSW07, PIN_RSTSW07),
													   new KNPwrSwitch("Module 08", PIN_RELAY08, PIN_PWRSS08, PIN_PWRSW08, PIN_RSTSW08)};
  // Init RTC clock
  _knClock = new KNClock();
  _knClock->UpdateFromNTPServer();

  // Init SD card
  _knCardStorage = new KNCardStorage();

  // Init KNLog variables
  LogClock = _knClock;
  LogCardStorage = _knCardStorage; 



  // Create RESTful functions
  _knRest->AddFunc("UpdateNTP", &TASK_UpdateNTPTime);

  // Init TaskScheduler
  _knTaskScheduler = new KNTaskScheduler();

  // Create tasks
  _knTaskScheduler->AddTask(new KNTask("Check Power Modules", PWR_CHECK_INTER, &TASK_CheckPowerModules, false));
  _knTaskScheduler->AddTask(new KNTask("Update time from NTP server", 0, 30, 0, &TASK_UpdateNTPTime));

  // Welcome
  KNLog::LogEvent(&(global_table[0]));
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

	delay(100);
}