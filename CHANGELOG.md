# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.1] - 2022-09-16
### [Added]
- use connect event to change MTU and connect parameters
### [Changed]
- device name - Sarcopenia Project
### [Removed]
- auth callback becauce not use it

## [0.3.0] - 2022-09-12
### [Changed]
- EMG increase to 2 channels, and use internal timer to control sampling
- Use queue from zephyr lib instead queue coded by me

## [0.2.1] - 2022-05-04
### [Added]
- Queue model, have the pop, push, and clean function
- Complete read me document
### [Removed]
- Linked list model

## [0.2.0] - 2022-05-02
### [Added]
- Linked list model, have the append, clean, and encode function
- Increase the ADC transmission speed

## [0.1.0] - 2022-02-24
### [Added]
- Sample ADC and MPU6050 data
- Implement BLE with ADC and IMU Serveic