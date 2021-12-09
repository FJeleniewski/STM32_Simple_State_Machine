## STM32 course - Simple State Machine

This is the project of simple state machine for button.\
Button in this project contains four states: Press, Long press, Repeat, Release.\
States are presented by switchig on/off LED (one LED for each state).\
\
State - LED (Pin) connection:\
Press - LD12 (PA6)\
Long press - LD11 (PA7)\
Repeat - LD10 (PA6)\
Release - LD9 (PC7)

Press - if button is pressed for less than 2 second then LD12 is turn on/off.\
Long press - if button is pressed for 2 seconds then LD11 is turn on/off.\
Repeat - if button is pressed for more than 2 seconds then LD10 starts blinking.\
Release - every time when button is released, LD9 blink 2 times.\

Board: STM32 Nucleo-F411RE