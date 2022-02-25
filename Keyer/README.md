## Structure

### Firmware
operations in main.c
- check to see whether key_state is non-zero
- check to see whether no keys are being pressed
- if no keys are pressed AND key_state is non-zero, transmit key_state
- set key_state to 0