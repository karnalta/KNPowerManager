#ifndef _declaration_H
#define _declaration_H

#ifndef NULL
#define NULL 0
#endif

// Externals Libraries
#include <avr/pgmspace.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NTPClient.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <Vector.h>

// Global Settings
#define DEBUG_MODE 0			// Print debug messages to serial
#define HTTP_BUFFER_SIZE 100	// HTTP request max lenght
#define MAX_FUNC 10				// Maximum number of REST function that can be registered
#define MAX_VAR 20				// Maximum number of REST variable that can be registered
#define TASK_MAX_DELAY 30		// Maximum task retard in seconds
#define PWR_SWITCH_CNT 8		// Maximum number of KNPowerManager modules
#define PWR_FACTOR 1.00f		// Power consumption adjustement factor
#define PWR_5V_OFFSET -3		// Offset applied to 512 theorical 2.5V
#define PWR_THRESHOLD 7			// Minimum consumption (W) to assume device is on
#define PWR_CHECK_INTER 30		// Interval between KNPowerManager's modules check

// Power module hard presence
#define PWR_MODULE_01 1
#define PWR_MODULE_02 0
#define PWR_MODULE_03 0
#define PWR_MODULE_04 0
#define PWR_MODULE_05 0
#define PWR_MODULE_06 0
#define PWR_MODULE_07 0
#define PWR_MODULE_08 0

// Pins Definition (Power Relay)
#define PIN_RELAY01	2
#define PIN_RELAY02	3
#define PIN_RELAY03	4
#define PIN_RELAY04	5
#define PIN_RELAY05	6
#define PIN_RELAY06	7
#define PIN_RELAY07	8
#define PIN_RELAY08	9

// Pins Definition (Temperature Sensor)
#define PIN_TMPSS01 A0
#define PIN_TMPSS02 A1
#define PIN_TMPSS03 A2

// Pins Definition (Power Sensor)
#define PIN_PWRSS01 A8
#define PIN_PWRSS02 A9
#define PIN_PWRSS03 A10
#define PIN_PWRSS04 A11
#define PIN_PWRSS05 A12
#define PIN_PWRSS06 A13
#define PIN_PWRSS07 A14
#define PIN_PWRSS08 A15

// Pins Definition (Power Switch Button)
#define PIN_PWRSW01 22
#define PIN_PWRSW02 23
#define PIN_PWRSW03 24
#define PIN_PWRSW04 25
#define PIN_PWRSW05 26
#define PIN_PWRSW06 27
#define PIN_PWRSW07 28
#define PIN_PWRSW08 29

// Pins Definition (Reset Switch Button)
#define PIN_RSTSW01 40
#define PIN_RSTSW02 41
#define PIN_RSTSW03 42
#define PIN_RSTSW04 43
#define PIN_RSTSW05 44
#define PIN_RSTSW06 45
#define PIN_RSTSW07 46
#define PIN_RSTSW08 47

// NTP Server
#define NTP_BELGIUM "0.be.pool.ntp.org"

// Class pre-declaration
class KNCardStorage;
class KNClock;
class KNLog;
class KNPwrSwitch;
class KNRest;
class KNTask;
class KNTaskScheduler;
class KNWebServer;

// Global access KNLog class
#include "KNLog.h"

// Strings storage in flash memoty
const char global_01[] PROGMEM = "Welcome to KN Power Manager !";
const char global_02[] PROGMEM = "Requiered parameters missing, cannot proceed operation.";
const char* const global_table[] PROGMEM = { global_01, global_02 };

const char knrest_01[] PROGMEM = "RESTful : Incoming request detected.";
const char knrest_02[] PROGMEM = "RESTful : Request recieved.";
const char knrest_03[] PROGMEM = "RESTful : Request first line content : ";
const char knrest_04[] PROGMEM = "RESTful : Callback function : ";
const char knrest_05[] PROGMEM = "RESTful : Variable function : ";
const char knrest_06[] PROGMEM = "RESTful : Request ignored. ";
const char* const knrest_table[] PROGMEM = { knrest_01, knrest_02, knrest_03, knrest_04, knrest_05, knrest_06 };

const char knwebserver_01[] PROGMEM = "KNWebServer : DNS query for %s returned %s";
const char knwebserver_02[] PROGMEM = "KNWebServer : HTTP request recieved.";
const char knwebserver_03[] PROGMEM = "KNWebServer : HTTP request first line content : ";
const char* const knwebserver_table[] PROGMEM = { knwebserver_01, knwebserver_02, knwebserver_03 };

const char kntaskscheduler_01[] PROGMEM = "KNTaskScheduler : Scheduled task executed : ";
const char* const kntaskscheduler_table[] PROGMEM = { kntaskscheduler_01 };

const char knclock_01[] PROGMEM = "KNClock : Cannot find RTC module !";
const char knclock_02[] PROGMEM = "KNClock : Update from NTP server failed !";
const char knclock_03[] PROGMEM = "KNClock : Update from NTP server completed.";
const char* const knclock_table[] PROGMEM = { knclock_01, knclock_02, knclock_03 };

const char knpwrswitch_01[] PROGMEM = "KNPwrSwitch : Quick power switch started.";
const char knpwrswitch_02[] PROGMEM = "KNPwrSwitch : Quick power switch completed.";
const char knpwrswitch_03[] PROGMEM = "KNPwrSwitch : Power switch button press started.";
const char knpwrswitch_04[] PROGMEM = "KNPwrSwitch : Power switch button press completed.";
const char knpwrswitch_05[] PROGMEM = "KNPwrSwitch : Reset switch button press started.";
const char knpwrswitch_06[] PROGMEM = "KNPwrSwitch : Reset switch button press completed.";
const char knpwrswitch_07[] PROGMEM = "KNPwrSwitch : Consumption measurement on module %s (RP : %s w/h - AP : %s w/h - PF : %s).";
const char* const knpwrswitch_table[] PROGMEM = { knpwrswitch_01, knpwrswitch_02, knpwrswitch_03, knpwrswitch_04, knpwrswitch_05, knpwrswitch_06, knpwrswitch_07 };

const char kncardstorage_01[] PROGMEM = "KNCardStorage : SD card initialization failed !";
const char kncardstorage_02[] PROGMEM = "KNCardStorage : HTML files not present !";
const char kncardstorage_03[] PROGMEM = "KNCardStorage : SD card successfully mounted (Size : %.02f GB).";
const char* const kncardstorage_table[] PROGMEM = { kncardstorage_01, kncardstorage_02, kncardstorage_03 };

#endif
