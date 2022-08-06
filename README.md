# scr_precharge
Controller for Solid State Tesla Coil power supply precharging with SCRs. I got tired of welding relays and didn't have the room for larger (>30A) contactors. I had a bunch of Semikron SKKT dual SCR modules, which were the same Semipack 1 style diode modules I was already using. So, using this precharge controller gets rid of the main relay as well as the precharge resistor and relay.

Eagle files for Semipack 1 gate driver boards are also included. These can be soldered up for modules with either gate configuration (GK GK or GK KG). The parts for the spade connectors that work with the Semipack 1 modules are:
https://www.digikey.com/en/products/detail/keystone-electronics/3544/303877

ST Microelectronics has a good app note on controlling SCRs and topologies:
https://www.st.com/resource/en/application_note/an4606-inrushcurrent-limiter-circuits-icl-with-triacs-and-thyristors-scr-and-controlled-bridge-design-tips-stmicroelectronics.pdf

The board comprises of a zero crossing detector, an ATTINY85 which generates the appropriate PWM gate signals, GDT outputs, and opto-isolated control inputs for precharge and run modes. There are two versions of the board, only difference being the type of GDT used, depending on the type of SCR configuration (dual or half SCR/diode modules).

The SCRs will need a gate resistor sized appropriately for the amount of current necessary to turn it on. I used 5.1 ohms on the Semipack 1 SKKT modules. The precharge board outputs must also be phased properly. Additionally, on 120VAC input, the hot line must be connected to pin 2 of X2 otherwise the zero crossing detector will not be able to detect any zero crossings. With 240VAC, both lines are hot so it will not matter.

Pick your opto of choice, then size the resistors according to the logic voltage. I used this with a UD3 so the signals were 24V.

I have some boards left over, so if you just need a couple, shoot me a message!

![scrpchg.JPG](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/scrpchg.JPG?raw=true)
|:--:| 
| *Figure 1. Controller board. Note that this is was the original version and modifications needed to be made to the zero crossing detector. These changes are reflected in the latest board files.* |

![scr_precharge_single_sch.png](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/scr_precharge_single_REV_B/scr_precharge_single_sch.png?raw=true)
|:--:| 
| *Figure 2. Schematic of single output controller board.* |

![skkt_gate_brd.JPG](https://github.com/alexhanyuan/scr_precharge/blob/main/hw/skkt_gate_brd.JPG?raw=true)
|:--:| 
| *Figure 3. Dual Semikron SCR modules with the gate boards.* |