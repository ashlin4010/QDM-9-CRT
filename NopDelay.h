// NopDelay.h
#ifndef NOP_DELAY_H
#define NOP_DELAY_H

// Inline function for a single NOP
inline void Nop() {
    __asm__ __volatile__("nop");
}

// Template to recursively generate NOP instructions
template <unsigned int N>
inline void NopDelay() {
    NopDelay<N - 1>();
    Nop();
}

// Template specialization to stop recursion
template <>
inline void NopDelay<1>() {
    Nop();
}

#endif // NOP_DELAY_H
