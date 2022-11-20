loop:
        rdtsc $2
        rdrand $3
        jmp loop
