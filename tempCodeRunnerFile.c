#include <stdio.h>
#include <stdint.h>

void print_binary(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
    printf("\n");
}

int main() {
    // Initial state: 00100000 (Bit 5 is ON)
    uint8_t motor_ctrl = 0x20; 
    
    printf("Initial state: ");
    print_binary(motor_ctrl);

    // 1. SET bit 3 to 1 (Turn the motor ON)
    // Remember: Ammunition (1) goes on the left, target position goes on the right.
    motor_ctrl = motor_ctrl | (1 << 3);
    


    // 2. CLEAR bit 5 to 0 (Clear the overheating error flag)
    motor_ctrl = motor_ctrl & ~(1 << 5);
    


    // 3. TOGGLE bit 0 (Flip the motor direction from Forward to Reverse)
    motor_ctrl = motor_ctrl ^ (1 << 0);

    printf("Final state:   ");
    print_binary(motor_ctrl);

    return 0;
}