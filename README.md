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
