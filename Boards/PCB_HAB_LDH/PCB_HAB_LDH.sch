EESchema Schematic File Version 2
LIBS:PCB_HAB_LDH-rescue
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:mysensors_arduino
LIBS:mysensors_connectors
LIBS:mysensors_logic
LIBS:mysensors_mcu
LIBS:mysensors_memories
LIBS:mysensors_network
LIBS:mysensors_radios
LIBS:mysensors_regulators
LIBS:mysensors_security
LIBS:mysensors_sensors
LIBS:G3MB-202P
LIBS:dc2205vchina
LIBS:LE33CZ
LIBS:HKLPM01
LIBS:makerlab
LIBS:PCB_HAB_LDH-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ArduinoProMini IC1
U 1 1 5A1796D5
P 5950 3100
F 0 "IC1" H 5200 4350 40  0000 L BNN
F 1 "ArduinoProMini" H 6350 1700 40  0000 L BNN
F 2 "ProMiniLDH_PCB:pro_mini_LDH_0.9Pads" H 5950 3100 30  0001 C CIN
F 3 "" H 5950 3100 60  0000 C CNN
	1    5950 3100
	1    0    0    -1  
$EndComp
$Comp
L NRF24L01 U3
U 1 1 5A1797CA
P 3800 4600
F 0 "U3" H 3900 4850 60  0000 C CNN
F 1 "NRF24L01" H 4050 4350 60  0000 C CNN
F 2 "mysensors_radios:NRF24L01-SMD" H 3800 4450 60  0001 C CNN
F 3 "" H 3800 4450 60  0000 C CNN
	1    3800 4600
	1    0    0    -1  
$EndComp
$Comp
L G3MB-202P-RESCUE-PCB_HAB_LDH K1
U 1 1 5A1799D5
P 7850 1650
F 0 "K1" H 7474 1876 50  0000 L BNN
F 1 "G3MB-202P" H 7474 1475 50  0000 L BNN
F 2 "ReleSSD:RELAY_G3MB-202P" H 7850 1650 50  0001 L BNN
F 3 "Unavailable" H 7850 1650 50  0001 L BNN
F 4 "Omron" H 7850 1650 50  0001 L BNN "MF"
F 5 "None" H 7850 1650 50  0001 L BNN "Price"
F 6 "" H 7850 1650 50  0001 L BNN "Description"
F 7 "None" H 7850 1650 50  0001 L BNN "Package"
F 8 "G3MB-202P" H 7850 1650 50  0001 L BNN "MP"
	1    7850 1650
	0    1    -1   0   
$EndComp
$Comp
L Varistor RV1
U 1 1 5A17A43B
P 1450 2950
F 0 "RV1" V 1575 2950 50  0000 C CNN
F 1 "Varistor" V 1325 2950 50  0000 C CNN
F 2 "Huella_Varistor_LDH:RV_Disc_D7_W3.4_P5" V 1380 2950 50  0001 C CNN
F 3 "" H 1450 2950 50  0001 C CNN
	1    1450 2950
	-1   0    0    1   
$EndComp
$Comp
L Screw_Terminal_01x02 J1
U 1 1 5A17AA23
P 700 3050
F 0 "J1" H 700 3150 50  0000 C CNN
F 1 "AC INPUT" H 700 2850 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-2_P5.08mm" H 700 3050 50  0001 C CNN
F 3 "" H 700 3050 50  0001 C CNN
	1    700  3050
	-1   0    0    1   
$EndComp
$Comp
L Screw_Terminal_01x02 J2
U 1 1 5A17ABA2
P 10500 2300
F 0 "J2" H 10500 2400 50  0000 C CNN
F 1 "AC OUTPUT" H 10500 2100 50  0000 C CNN
F 2 "Connectors_Terminal_Blocks:TerminalBlock_bornier-2_P5.08mm" H 10500 2300 50  0001 C CNN
F 3 "" H 10500 2300 50  0001 C CNN
	1    10500 2300
	1    0    0    -1  
$EndComp
$Comp
L Fuse F1
U 1 1 5A17AC9F
P 900 1650
F 0 "F1" V 980 1650 50  0000 C CNN
F 1 "Fusible" V 825 1650 50  0000 C CNN
F 2 "Huella_Fusibles_LDH:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" V 830 1650 50  0001 C CNN
F 3 "" H 900 1650 50  0001 C CNN
	1    900  1650
	1    0    0    -1  
$EndComp
$Comp
L Fuse F2
U 1 1 5A17B00C
P 900 2400
F 0 "F2" V 980 2400 50  0000 C CNN
F 1 "Termo" V 825 2400 50  0000 C CNN
F 2 "Huella_Fusibles_LDH:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" V 830 2400 50  0001 C CNN
F 3 "" H 900 2400 50  0001 C CNN
	1    900  2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  2250 900  1800
Wire Wire Line
	900  3050 900  5650
Wire Wire Line
	8300 2250 8225 2250
Wire Wire Line
	8300 4800 8300 2250
Wire Wire Line
	8300 1050 8200 1050
Wire Wire Line
	10300 1400 10300 2300
Wire Wire Line
	8200 1400 10300 1400
Wire Wire Line
	8300 850  8300 1050
Wire Wire Line
	900  850  8300 850 
Wire Wire Line
	900  5650 9850 5650
Wire Wire Line
	9850 5650 9850 2400
Wire Wire Line
	9850 2400 10300 2400
Wire Wire Line
	900  2950 900  2550
Wire Wire Line
	900  1500 900  850 
Wire Wire Line
	1450 2800 1450 850 
Connection ~ 1450 850 
Wire Wire Line
	1450 3100 1450 5650
Connection ~ 1450 5650
Wire Wire Line
	1700 850  1700 2450
Connection ~ 1700 850 
Wire Wire Line
	1700 2650 1700 5650
Connection ~ 1700 5650
Wire Wire Line
	3400 2850 3800 2850
Wire Wire Line
	4900 4800 8300 4800
Wire Wire Line
	4900 2250 4900 5200
$Comp
L L78L33_SOT89 U2
U 1 1 5A1F4637
P 3800 3450
F 0 "U2" H 3650 3575 50  0000 C CNN
F 1 "L78L33_SOT89" H 3775 3575 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-89-3" H 3800 3650 50  0001 C CIN
F 3 "" H 3800 3400 50  0001 C CNN
	1    3800 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 2000 3800 3150
Connection ~ 3800 2850
Wire Wire Line
	3500 3450 2750 3450
Wire Wire Line
	2750 3450 2750 5200
Wire Wire Line
	2750 5200 4900 5200
Wire Wire Line
	3800 4950 3800 5200
Connection ~ 3800 5200
$Comp
L CP1 C1
U 1 1 5A1F4CD8
P 4400 3400
F 0 "C1" H 4425 3500 50  0000 L CNN
F 1 "CP1" H 4425 3300 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D5.0mm_P2.50mm" H 4400 3400 50  0001 C CNN
F 3 "" H 4400 3400 50  0001 C CNN
	1    4400 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 3950 4400 3950
Wire Wire Line
	4400 3950 4400 3550
Connection ~ 3800 3950
Wire Wire Line
	3400 2250 4900 2250
Wire Wire Line
	4900 4200 5050 4200
Wire Wire Line
	4400 3250 4400 2250
Connection ~ 4400 2250
Wire Wire Line
	3300 4500 3400 4500
Wire Wire Line
	3300 4600 3400 4600
Wire Wire Line
	3300 4700 3400 4700
Text Label 3300 4500 0    60   ~ 0
a4
Text Label 3300 4600 0    60   ~ 0
a5
Text Label 3300 4700 0    60   ~ 0
a6
Wire Wire Line
	4300 4500 4200 4500
Wire Wire Line
	4300 4600 4200 4600
Wire Wire Line
	4300 4700 4200 4700
Text Label 4200 4500 0    60   ~ 0
a1
Text Label 4200 4600 0    60   ~ 0
a2
Text Label 4200 4700 0    60   ~ 0
a3
Entry Wire Line
	4300 4500 4400 4600
Entry Wire Line
	4300 4600 4400 4700
Entry Wire Line
	4300 4700 4400 4800
Entry Wire Line
	3200 4400 3300 4500
Entry Wire Line
	3200 4500 3300 4600
Entry Wire Line
	3200 4600 3300 4700
Wire Wire Line
	3800 3750 3800 4250
Wire Bus Line
	3200 4400 3200 5000
Wire Bus Line
	3200 5000 4400 5000
Wire Bus Line
	4400 5000 4400 4600
Wire Bus Line
	4400 4600 7200 4600
Wire Bus Line
	7200 4600 7200 2300
Entry Bus Bus
	7100 2200 7200 2300
Entry Bus Bus
	7100 2900 7200 3000
Entry Bus Bus
	7100 3000 7200 3100
Entry Bus Bus
	7100 3400 7200 3500
Entry Bus Bus
	7100 3200 7200 3300
Entry Bus Bus
	7100 3300 7200 3400
Wire Wire Line
	7100 2200 6950 2200
Text Label 6950 2200 0    60   ~ 0
a2
Wire Wire Line
	7100 3300 6950 3300
Text Label 6950 3300 0    60   ~ 0
a4
Wire Wire Line
	7100 3200 6950 3200
Text Label 6950 3200 0    60   ~ 0
a5
Wire Wire Line
	6950 3400 7100 3400
Text Label 6950 3400 0    60   ~ 0
a6
Wire Wire Line
	7100 3000 6950 3000
Text Label 6950 3000 0    60   ~ 0
a1
Wire Wire Line
	6950 2900 7100 2900
Text Label 6950 2900 0    60   ~ 0
a3
Wire Wire Line
	6950 2600 8450 2600
Wire Wire Line
	8450 2600 8450 2100
Wire Wire Line
	8450 2100 8225 2100
NoConn ~ 6950 2000
NoConn ~ 6950 2100
NoConn ~ 6950 2300
NoConn ~ 6950 2400
NoConn ~ 6950 2500
NoConn ~ 6950 2700
NoConn ~ 6950 2800
NoConn ~ 6950 3600
NoConn ~ 6950 3700
NoConn ~ 6950 3800
NoConn ~ 6950 3900
NoConn ~ 6950 4000
NoConn ~ 6950 4100
NoConn ~ 6950 4200
NoConn ~ 6950 4300
NoConn ~ 5050 3800
NoConn ~ 5050 3900
NoConn ~ 5050 2300
Connection ~ 4900 4800
NoConn ~ 5050 4300
Connection ~ 4900 4200
$Comp
L HKLPM01 HKLP1
U 1 1 5A20A446
P 2600 2550
F 0 "HKLP1" H 2099 2851 50  0000 L BNN
F 1 "HKLPM01" H 2600 2550 50  0001 L BNN
F 2 "Huella_HLK-PM01_LDH:HKLPM01" H 2600 2550 50  0001 L BNN
F 3 "" H 2600 2550 60  0001 C CNN
	1    2600 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 2450 1800 2450
Wire Wire Line
	1700 2650 1800 2650
Wire Wire Line
	3800 2000 5050 2000
NoConn ~ 5050 4100
$EndSCHEMATC
