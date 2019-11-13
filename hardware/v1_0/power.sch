EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 6
Title ""
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
$Comp
L robot_pcb0-rescue:MCP16311_2-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue U5
U 1 1 5B1BBDDC
P 4700 4400
F 0 "U5" H 4700 4750 60  0000 C CNN
F 1 "MCP16311/2" H 4700 4100 60  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4750 4400 60  0001 C CNN
F 3 "" H 4750 4400 60  0000 C CNN
	1    4700 4400
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:INDUCTOR-crf_1 L2
U 1 1 5B1BBDE3
P 6100 4450
F 0 "L2" V 6300 4450 40  0000 C CNN
F 1 "22U" V 6050 4450 40  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_MD-5050" H 6100 4450 60  0001 C CNN
F 3 "" H 6100 4450 60  0000 C CNN
	1    6100 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 4600 6200 4600
Wire Wire Line
	6200 4600 6200 4650
Wire Wire Line
	6200 5050 6200 4950
Wire Wire Line
	6450 5050 6450 4950
Connection ~ 5400 5050
Wire Wire Line
	3950 4350 3950 4650
Connection ~ 4550 3900
Wire Wire Line
	4550 4050 4550 3900
Wire Wire Line
	3950 4050 4550 4050
Wire Wire Line
	3950 4050 3950 4250
Wire Wire Line
	3950 4250 4100 4250
Connection ~ 5400 4250
Wire Wire Line
	5150 3900 5400 3900
Wire Wire Line
	5400 4550 5300 4550
Wire Wire Line
	5400 3900 5400 4250
Wire Wire Line
	5300 4250 5400 4250
Connection ~ 6450 4450
Wire Wire Line
	3950 3900 4150 3900
Wire Wire Line
	3950 3700 3950 3900
Wire Wire Line
	6450 3700 3950 3700
Wire Wire Line
	4450 3900 4550 3900
Wire Wire Line
	5700 4000 5700 4050
Wire Wire Line
	5500 4000 5700 4000
Wire Wire Line
	5500 4350 5500 4000
Wire Wire Line
	5300 4350 5500 4350
Wire Wire Line
	6400 4450 6450 4450
Wire Wire Line
	5700 4350 5700 4450
Wire Wire Line
	5300 4450 5700 4450
Wire Wire Line
	3950 4950 3950 5050
Wire Wire Line
	3700 4950 3700 5050
Wire Wire Line
	3700 4450 3700 4550
Wire Wire Line
	6250 4100 6250 4150
Wire Wire Line
	6250 4150 6450 4150
Wire Wire Line
	4100 4450 3850 4450
Wire Wire Line
	3850 4450 3850 4550
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C18
U 1 1 5B1BBE0D
P 3700 4800
F 0 "C18" H 3725 4900 50  0000 L CNN
F 1 "2.2u" H 3725 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3738 4650 30  0001 C CNN
F 3 "" H 3700 4800 60  0000 C CNN
	1    3700 4800
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C20
U 1 1 5B1BBE14
P 3950 4800
F 0 "C20" H 3975 4900 50  0000 L CNN
F 1 "2.2u" H 3975 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3988 4650 30  0001 C CNN
F 3 "" H 3950 4800 60  0000 C CNN
	1    3950 4800
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C24
U 1 1 5B1BBE1B
P 6200 4800
F 0 "C24" H 6225 4900 50  0000 L CNN
F 1 "2.2u" H 6225 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6238 4650 30  0001 C CNN
F 3 "" H 6200 4800 60  0000 C CNN
	1    6200 4800
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:CP1-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C26
U 1 1 5B1BBE22
P 6450 4800
F 0 "C26" H 6475 4900 50  0000 L CNN
F 1 "100u" H 6475 4700 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-12_Kemet-T" H 6450 4800 60  0001 C CNN
F 3 "" H 6450 4800 60  0000 C CNN
	1    6450 4800
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C22
U 1 1 5B1BBE2F
P 5700 4200
F 0 "C22" H 5725 4300 50  0000 L CNN
F 1 "100n" H 5725 4100 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5738 4050 30  0001 C CNN
F 3 "" H 5700 4200 60  0000 C CNN
	1    5700 4200
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:R-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue R9
U 1 1 5B1BBE36
P 5000 3900
F 0 "R9" V 5080 3900 50  0000 C CNN
F 1 "10k" V 5000 3900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4930 3900 30  0001 C CNN
F 3 "" H 5000 3900 30  0000 C CNN
	1    5000 3900
	0    1    1    0   
$EndComp
$Comp
L robot_pcb0-rescue:R-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue R7
U 1 1 5B1BBE3D
P 4300 3900
F 0 "R7" V 4380 3900 50  0000 C CNN
F 1 "52.3k" V 4300 3900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4230 3900 30  0001 C CNN
F 3 "" H 4300 3900 30  0000 C CNN
	1    4300 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 5050 5400 5150
Wire Wire Line
	4550 3900 4850 3900
Wire Wire Line
	5400 4250 5400 4550
Wire Wire Line
	5400 4550 5400 5050
Wire Wire Line
	5700 4450 5800 4450
Wire Wire Line
	3700 4550 3700 4650
Text GLabel 6250 4100 1    60   Input ~ 0
5V
Wire Wire Line
	4100 4350 3950 4350
Wire Wire Line
	5400 5050 6200 5050
Wire Wire Line
	6200 5050 6450 5050
Connection ~ 3950 5050
Wire Wire Line
	3950 5050 5400 5050
Wire Wire Line
	3700 5050 3950 5050
Connection ~ 6200 5050
Wire Wire Line
	6450 4450 6450 4600
Wire Wire Line
	6450 4600 6450 4650
Connection ~ 6450 4150
Wire Wire Line
	6450 4150 6450 4450
Wire Wire Line
	6450 3700 6450 4150
Connection ~ 6450 4600
Connection ~ 3850 4550
Connection ~ 3700 4550
Wire Wire Line
	3850 4550 4100 4550
Wire Wire Line
	3700 4550 3850 4550
Connection ~ 5700 4450
Connection ~ 5400 4550
$Comp
L robot_pcb0-rescue:MCP16311_2-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue U4
U 1 1 5B1BFD2A
P 4650 2100
F 0 "U4" H 4650 2450 60  0000 C CNN
F 1 "MCP16311/2" H 4650 1800 60  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 4700 2100 60  0001 C CNN
F 3 "" H 4700 2100 60  0000 C CNN
	1    4650 2100
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:INDUCTOR-crf_1 L1
U 1 1 5B1BFD31
P 6050 2150
F 0 "L1" V 6250 2150 40  0000 C CNN
F 1 "22U" V 6000 2150 40  0000 C CNN
F 2 "Inductor_SMD:L_Taiyo-Yuden_MD-5050" H 6050 2150 60  0001 C CNN
F 3 "" H 6050 2150 60  0000 C CNN
	1    6050 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 2300 6150 2300
Wire Wire Line
	6150 2300 6150 2350
Wire Wire Line
	6150 2750 6150 2650
Wire Wire Line
	6400 2750 6400 2650
Connection ~ 5350 2750
Wire Wire Line
	3900 2050 3900 2350
Wire Wire Line
	4050 2050 3900 2050
Connection ~ 4500 1600
Wire Wire Line
	4500 1750 4500 1600
Wire Wire Line
	3900 1750 4500 1750
Wire Wire Line
	3900 1750 3900 1950
Wire Wire Line
	3900 1950 4050 1950
Wire Wire Line
	5350 2250 5250 2250
Wire Wire Line
	5250 1950 5350 1950
Connection ~ 6400 2150
Wire Wire Line
	3900 1600 4100 1600
Wire Wire Line
	3900 1400 3900 1600
Wire Wire Line
	6400 1400 6400 1850
Wire Wire Line
	4400 1600 4500 1600
Wire Wire Line
	5650 1700 5650 1750
Wire Wire Line
	5450 2050 5450 1700
Wire Wire Line
	5250 2050 5450 2050
Wire Wire Line
	6350 2150 6400 2150
Wire Wire Line
	5650 2050 5650 2150
Wire Wire Line
	5250 2150 5650 2150
Wire Wire Line
	3900 2650 3900 2750
Wire Wire Line
	3650 2650 3650 2750
Wire Wire Line
	3650 2250 3800 2250
Connection ~ 3650 2250
Wire Wire Line
	6200 1800 6200 1850
Wire Wire Line
	6200 1850 6400 1850
Connection ~ 6400 1850
Wire Wire Line
	4050 2150 3800 2150
Wire Wire Line
	3800 2150 3800 2250
Connection ~ 3800 2250
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C17
U 1 1 5B1BFD5B
P 3650 2500
F 0 "C17" H 3675 2600 50  0000 L CNN
F 1 "2.2u" H 3675 2400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3688 2350 30  0001 C CNN
F 3 "" H 3650 2500 60  0000 C CNN
	1    3650 2500
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C19
U 1 1 5B1BFD62
P 3900 2500
F 0 "C19" H 3925 2600 50  0000 L CNN
F 1 "2.2u" H 3925 2400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3938 2350 30  0001 C CNN
F 3 "" H 3900 2500 60  0000 C CNN
	1    3900 2500
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C23
U 1 1 5B1BFD69
P 6150 2500
F 0 "C23" H 6175 2600 50  0000 L CNN
F 1 "2.2u" H 6175 2400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6188 2350 30  0001 C CNN
F 3 "" H 6150 2500 60  0000 C CNN
	1    6150 2500
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:CP1-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C25
U 1 1 5B1BFD70
P 6400 2500
F 0 "C25" H 6425 2600 50  0000 L CNN
F 1 "100u" H 6425 2400 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-3528-12_Kemet-T" H 6400 2500 60  0001 C CNN
F 3 "" H 6400 2500 60  0000 C CNN
	1    6400 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5B1BFD77
P 5350 2850
F 0 "#PWR030" H 5350 2600 50  0001 C CNN
F 1 "GND" H 5350 2700 50  0000 C CNN
F 2 "" H 5350 2850 60  0000 C CNN
F 3 "" H 5350 2850 60  0000 C CNN
	1    5350 2850
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C21
U 1 1 5B1BFD7D
P 5650 1900
F 0 "C21" H 5675 2000 50  0000 L CNN
F 1 "100n" H 5675 1800 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5688 1750 30  0001 C CNN
F 3 "" H 5650 1900 60  0000 C CNN
	1    5650 1900
	1    0    0    -1  
$EndComp
$Comp
L robot_pcb0-rescue:R-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue R8
U 1 1 5B1BFD84
P 4950 1600
F 0 "R8" V 5030 1600 50  0000 C CNN
F 1 "10k" V 4950 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4880 1600 30  0001 C CNN
F 3 "" H 4950 1600 30  0000 C CNN
	1    4950 1600
	0    1    1    0   
$EndComp
$Comp
L robot_pcb0-rescue:R-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue R6
U 1 1 5B1BFD8B
P 4250 1600
F 0 "R6" V 4330 1600 50  0000 C CNN
F 1 "33k" V 4250 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4180 1600 30  0001 C CNN
F 3 "" H 4250 1600 30  0000 C CNN
	1    4250 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	6150 2750 6400 2750
Wire Wire Line
	5350 2750 6150 2750
Wire Wire Line
	5350 2750 5350 2850
Wire Wire Line
	4500 1600 4800 1600
Wire Wire Line
	5650 2150 5750 2150
Wire Wire Line
	3650 2250 3650 2350
Wire Wire Line
	6400 1850 6400 2150
Wire Wire Line
	3800 2250 4050 2250
Wire Wire Line
	3650 1850 3650 2250
Text GLabel 3650 1850 1    60   Input ~ 0
5V
$Comp
L power:VCC #PWR032
U 1 1 5B1BFD9C
P 6200 1800
F 0 "#PWR032" H 6200 1900 30  0001 C CNN
F 1 "VCC" H 6200 1900 30  0000 C CNN
F 2 "" H 6200 1800 60  0001 C CNN
F 3 "" H 6200 1800 60  0001 C CNN
	1    6200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2150 6400 2300
Wire Wire Line
	6400 2300 6400 2350
Connection ~ 6400 2300
Connection ~ 6150 2750
Wire Wire Line
	5350 2250 5350 2750
Connection ~ 5650 2150
Wire Wire Line
	5350 1950 5350 2250
Wire Wire Line
	5350 1600 5350 1950
Connection ~ 5350 2250
Connection ~ 5350 1950
Connection ~ 3900 2750
Wire Wire Line
	3900 2750 5350 2750
Wire Wire Line
	3650 2750 3900 2750
Wire Wire Line
	5450 1700 5650 1700
Wire Wire Line
	6400 1400 3900 1400
Wire Wire Line
	5100 1600 5350 1600
$Comp
L Connector:Screw_Terminal_01x02 J4
U 1 1 5B51C131
P 2050 3650
F 0 "J4" H 1970 3325 50  0000 C CNN
F 1 "Power_terminal" H 2350 3400 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal" H 2050 3650 50  0001 C CNN
F 3 "~" H 2050 3650 50  0001 C CNN
	1    2050 3650
	-1   0    0    1   
$EndComp
Text GLabel 3700 4450 1    60   Input ~ 0
V_IN
Text GLabel 2500 3550 2    60   Input ~ 0
V_IN
$Comp
L power:GND #PWR031
U 1 1 5B52681D
P 5400 5150
F 0 "#PWR031" H 5400 4900 50  0001 C CNN
F 1 "GND" H 5400 5000 50  0000 C CNN
F 2 "" H 5400 5150 60  0000 C CNN
F 3 "" H 5400 5150 60  0000 C CNN
	1    5400 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5B529148
P 2500 3700
F 0 "#PWR029" H 2500 3450 50  0001 C CNN
F 1 "GND" H 2500 3550 50  0000 C CNN
F 2 "" H 2500 3700 60  0000 C CNN
F 3 "" H 2500 3700 60  0000 C CNN
	1    2500 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3650 2500 3700
Wire Wire Line
	2250 3550 2350 3550
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5B5F2005
P 2350 3400
F 0 "#FLG02" H 2350 3475 50  0001 C CNN
F 1 "PWR_FLAG" H 2350 3574 50  0000 C CNN
F 2 "" H 2350 3400 50  0001 C CNN
F 3 "~" H 2350 3400 50  0001 C CNN
	1    2350 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3400 2350 3550
Connection ~ 2350 3550
Wire Wire Line
	2350 3550 2500 3550
Wire Wire Line
	2250 3650 2500 3650
Text GLabel 7900 3500 0    50   Input ~ 0
V_IN
Wire Wire Line
	7900 3500 8000 3500
$Comp
L Device:R_Small R11
U 1 1 5AF98894
P 8100 3500
F 0 "R11" V 7904 3500 50  0000 C CNN
F 1 "56k" V 7995 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8100 3500 50  0001 C CNN
F 3 "~" H 8100 3500 50  0001 C CNN
	1    8100 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	8200 3500 8300 3500
$Comp
L Device:R_Small R12
U 1 1 5AF9BA2E
P 8500 3500
F 0 "R12" V 8696 3500 50  0000 C CNN
F 1 "33k" V 8605 3500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8500 3500 50  0001 C CNN
F 3 "~" H 8500 3500 50  0001 C CNN
	1    8500 3500
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5AF9BB8D
P 8950 3500
F 0 "#PWR0102" H 8950 3250 50  0001 C CNN
F 1 "GND" H 8955 3327 50  0000 C CNN
F 2 "" H 8950 3500 50  0001 C CNN
F 3 "" H 8950 3500 50  0001 C CNN
	1    8950 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 3500 8700 3500
Wire Wire Line
	8300 3500 8300 3200
Wire Wire Line
	8400 3500 8300 3500
Connection ~ 8300 3500
Text GLabel 8300 3200 1    50   Input ~ 0
V_METER
Text Notes 8600 3150 0    50   ~ 0
DONE: Calculate the resistances needed here.\n      47k, 31k gives a max output of 3.33V at 8.4V input.\n      51k, 33k gives a max output of 3.3V at 8.4V input.\n      56k, 33k gives a max output of 3.115V at 8.4V input  \n
Text Notes 7300 4200 2    50   ~ 0
5V needed for CAN
$Comp
L robot_pcb0-rescue:C-RESCUE-RCCAR_IF_Board_GPS-RCCAR_IF_Board_GPS-rescue-uwb_pcb-rescue C33
U 1 1 5AFD4290
P 8500 3850
F 0 "C33" H 8525 3950 50  0000 L CNN
F 1 "2.2u" H 8525 3750 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8538 3700 30  0001 C CNN
F 3 "" H 8500 3850 60  0000 C CNN
	1    8500 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	8300 3500 8300 3850
Wire Wire Line
	8300 3850 8350 3850
Wire Wire Line
	8700 3500 8700 3850
Wire Wire Line
	8700 3850 8650 3850
Connection ~ 8700 3500
Wire Wire Line
	8700 3500 8950 3500
Text Notes 9450 4400 2    50   ~ 0
E12 serien\n
Text Notes 4100 5500 2    50   ~ 0
The DRV8833 can handle 11.8V max on the VM (motor voltage)
$EndSCHEMATC
