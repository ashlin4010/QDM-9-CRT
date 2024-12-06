#include <avr/io.h> 

.org 0x0040          ; Timer1 Compare Match A interrupt vector

timer1_isr:
    ; Toggle LED on PD0
    ldi r16, (1 << PD0)    ; Load value to toggle PD0
    eor PORTD, r16         ; Toggle PD0
    reti                   ; Return from interrupt