# Lab 5 - Alex Guymon and Brittney Morales

In this lab we were testing the delay with RTC and timers with the use of delay and message queue.

## Calculating our Jittering

$T = 2.193 ms$\
$f = 456.277$\
$duty = 49.99%$\

the jitter for our stm32 is 0.2 ms per 2 ms, so for hour we lose 0.72 in an hour.\
the Equation we use is $.0002 * 60 * 60$.

## Kernel.c

- is 0.01 ms faster by period, and 1.5 Hz faster on frequency.

## GPIO_Interrupt.c

The delay between the sync signal and the output from the board. is about half the frequency.
The delay is about 5 microseconds, just running the file itself.

## Busy.c

with our busy loop it is a bit faster, and it is no longer halving the frequency with the use of the 1ms.\
It is 1ms high and 1ms low giving us a frequency of 500Hz (just about).

## RTC.c

22 mHz is our standard of Deviation.

## Message queue

When using the message queue to the interrupt a delay of about 23 microseconds. Which is 4-5 times longer than without the use of message queue.
