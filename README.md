# scr_precharge
Tesla Coil power supply precharging with SCRs. I got tired of welding relays and didn't have the room for larger (>30A) contactors. I had a bunch of Semikron SKKT dual SCR modules, which were the same Semipack 1 style diode modules I was already using. So, using this precharge controller gets rid of the main relay as well as the precharge resistor and relay. Eagle files for Semipack 1 gate driver boards are also included. These can be soldered up for modules with either gate configuration (GK GK or GK KG). 

The board comprises of a zero crossing detector, an ATTINY85 which generates the appropriate PWM gate signals, GDT outputs, and opto-isolated control inputs for precharge and run modes. There are two versions of the board, only difference being the type of GDT used, depending on the type of SCR configuration (dual or half SCR/diode modules).

![alt text](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/scrpchg.JPG?raw=true)

![alt text](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/scr_precharge_single_REV_B/scr_precharge_single_sch.png?raw=true)

![alt text](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/skkt_gate_brd.JPG?raw=true)
