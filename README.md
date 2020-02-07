# RC5_Module_Embedded_C

As we know technology is developing day by day.
Every where we see remote  operated devices are replacing the ordinary switching/ controlling methods.
It also proves itself  a very cheap and effective way to control the devices, but when we see our rooms or our houses  we are still using the ordinary methods of switching to control all the home appliances.
So we developed the RC5 protocol based device which can easily replace the ordinary switching board. 
Which works on Manchester coding and this device enables us cotrolling our devices conviently.

5.9 RC5 REMOTE DECODER SERIAL OUT
The RC5 remote decoder board is based on the IC ST3617 which decodes the received
remote control data and output 16 bit serial data output. The decoded data contains
various information like Toggle Bit, Address of Remote and Command Key Pressed. This decoded information from transmitter can be used in various ways to make any remote controlled application. The transmitter should be a RC5(Philips) protocol type normally found in house hold TV remote controls.Custom RC5 transmitter can be designed using SAA3010 IC
 
 
5.10 BLOCK DIAGRAME
ST3617 needs only one component to work that is IR receiver like TSOP1738 or similar. The data is output as simple 2 bytes of serial data consisting of 16 bits of information for each key press on the remote.

 
5.11 PHILIPS CHIP RC5 PROTOCOL
Let us review the RC5 protocol to understand the RC5 decoder IC ST3617 better. The RC5 code from Philips is possibly the most used protocol by hobbyists, probably because of the wide availability of cheap remote controls. The protocol is well defined for different device types ensuring compatibility with your whole entertainment system.

FEATURES OF PROTOCOL
5 bit address and 6 bit command length
 Bi-phase coding (aka Manchester coding)
Carrier frequency of 36kHz or 38kHz
Constant bit time of 1.778ms (64 cycles of 36 kHz), Different timing for 38Khz

5.11 MODULATION
RC5 Modulation The protocol uses bi-phase modulation (or so-called Manchester coding) of a 36kHz IR carrier frequency. All bits are of equal length of 1.778ms in this protocol, with half of the bit time filled with a burst of the 36kHz carrier and the other half being idle. A logical zero is represented by a burst in the first half of the bit time. A logical one is represented by a burst in the second half of the bit time. The pulse/pause ratio of the 36kHz carrier frequency is 1/3 or 1/4 which reduces power consumption.
 
5.12 OUTPUT DATA FORMAT
Output from ST3617 is in two bytes, thus making total 16 bits of data, let us see meaning of each bit  A4-A0 = RC5 address of remote control, For TV remote this is zero. D5-D0 = RC5 command for each keypress at remote control. For Key 1 its 1, Key 2 = 2 and such, Find a table on last of this datasheet showing key value for each key press
