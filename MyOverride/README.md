
- \mbed-os\targets\TARGET_NUVOTON\TARGET_M451\device\TOOLCHAIN_ARM_STD\M453.sct
    Modify ROM size to (128-2)KB from 256KB for ARMC6 compiler.

- \mbed-os\targets\TARGET_NUVOTON\TARGET_M451\device\TOOLCHAIN_GCC_ARM\M453.ld
    Modify ROM size to (128-2)KB from 256KB for GCC compiler.

- \mbed-os\targets\TARGET_NUVOTON\TARGET_M451\analogin_api.c
    Speed down the ADC clock.