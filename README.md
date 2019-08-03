# KNPowerManager

## Description

Arduino Mega ADK based project to allow remote control of computer's power state (IPMI style working with any computer).

## Hardware

- Main controller : Arduino Mega + Ethernet Shield + RTC Module.
- Client module : Custom made module (PCB + 3D Print) and customized computer's rear bracket.

## Features

- Manageable from a basic WebUI or a RESTful API style.
- Get power consumption of each computer.
- Get power state of each computer.
- Hard resetting the PSU of a computer.
- Press the power button of a computer.
- Press the reset button of a computer.
- Allow to schedule the above operations for each computer.
- Maximum 8 computers can be controlled.

## WebUI usage
- Accessed by browser at the configured TCP/IP address in KNPowerManager.ino file.

## RESTful API usage
- http://xxx.xxx.xxx.xxx/func/ResetPSU?id=0 => Reset PSU of module index 0.
- http://xxx.xxx.xxx.xxx/func/PressPwr?id=0&duration=1000 => Press power button of module index 0 for 1 second.
- http://xxx.xxx.xxx.xxx/func/PressRst?id=0&duration=1000 => Press power button of module index 0 for 1 second.
