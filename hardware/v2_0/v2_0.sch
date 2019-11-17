EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title "Four DC-Motor Controller"
Date ""
Rev ""
Comp "Benjamin Vedder & Joel Svensson"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
Comment5 ""
Comment6 ""
Comment7 ""
Comment8 ""
Comment9 ""
$EndDescr
$Sheet
S 4500 2300 1250 1850
U 5AF92F5C
F0 "base_sheet" 50
F1 "base.sch" 50
F2 "DRV1_CH1" I R 5750 3250 50 
F3 "DRV1_CH2" I R 5750 3350 50 
F4 "DRV1_CH3" I R 5750 3450 50 
F5 "DRV1_CH4" I R 5750 3550 50 
F6 "DRV2_CH3" I R 5750 3700 50 
F7 "DRV2_CH2" I R 5750 3800 50 
F8 "DRV2_CH1" I R 5750 3900 50 
F9 "DRV2_CH4" I R 5750 4000 50 
$EndSheet
$Sheet
S 800  4600 1750 1400
U 5B1BB5D0
F0 "power_sheet" 50
F1 "power.sch" 50
$EndSheet
Text Notes 950  4850 0    50   ~ 0
5V, 3.3V
$Sheet
S 3250 4700 1750 1350
U 5B4411CA
F0 "mounitng_sheet" 50
F1 "mounting.sch" 50
$EndSheet
Text Notes 3600 4950 0    50   ~ 0
Mounting holes\n
$Sheet
S 6600 2450 1100 1650
U 5B516B04
F0 "motor_control_sheet" 50
F1 "motor_control.sch" 50
F2 "5V_DRV_1" I L 6600 2750 50 
F3 "5V_DRV_2" I L 6600 2850 50 
F4 "5V_DRV_3" I L 6600 2950 50 
F5 "5V_DRV_4" I L 6600 3050 50 
F6 "5V_DRV_NFAULT" I R 7700 2750 50 
F7 "DRV1_NFAULT" I R 7700 2850 50 
F8 "DRV1_CH1" I L 6600 3250 50 
F9 "DRV1_CH2" I L 6600 3350 50 
F10 "DRV1_CH3" I L 6600 3450 50 
F11 "DRV1_CH4" I L 6600 3550 50 
F12 "DRV2_CH3" I L 6600 3900 50 
F13 "DRV2_CH2" I L 6600 3800 50 
F14 "DRV2_CH1" I L 6600 3700 50 
F15 "DRV2_CH4" I L 6600 4000 50 
F16 "DRV2_NFAULT" I R 7700 2950 50 
$EndSheet
$Sheet
S 5450 4600 1700 1400
U 5CF66F52
F0 "nrf52" 50
F1 "nrf52.sch" 50
$EndSheet
Wire Wire Line
	5750 3250 6600 3250
Wire Wire Line
	5750 3350 6600 3350
Wire Wire Line
	5750 3450 6600 3450
Wire Wire Line
	5750 3550 6600 3550
Wire Wire Line
	5750 3700 6600 3700
Wire Wire Line
	5750 3800 6600 3800
Wire Wire Line
	5750 3900 6600 3900
Wire Wire Line
	5750 4000 6600 4000
$EndSCHEMATC
