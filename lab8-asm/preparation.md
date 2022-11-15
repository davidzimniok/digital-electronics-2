# Lab 8: Combining C and assembly language

## Pre-Lab preparation

1. Use **AVR® Instruction Set Manual** from Microchip [Online Technical Documentation](https://onlinedocs.microchip.com/), find the description of selected instructions, and complete the table.

   | **Instruction** | **Operation** | **Description** | **Cycles** |
   | :-- | :-: | :-- | :-: |
   | `add Rd, Rr` | Rd = Rd + Rr | Adds two registers without the C flag and places the result in the destination register Rd. | 1 |
   | `andi Rd, K` | Rd = Rd and K | Logical AND between register Rd and 8-bit constant K | 1 |
   | `bld Rd, b` | Rd(b) = T | Copies the T bit in the SREG (Status Register) to bit b in register Rd. | 1 |
   | `bst Rd, b` | T = Rd(b) | Stores bit b from Rd to the T bit in SREG (Status Register). | 1 |
   | `com Rd` | Rd = 0xFF - Rd | This instruction performs a One’s Complement of register Rd. | 1 |
   | `eor Rd, Rr` | Rd = Rd xor Rr | Performs the logical EOR between the contents of register Rd and register Rr and places the result in the destination register Rd. | 1 |
   | `mul Rd, Rr` | R1:R0 = Rd × Rr (unsigned ‹ unsigned × unsigned) | This instruction performs 8-bit × 8-bit › 16-bit unsigned multiplication. | 2 |
   | `pop Rd` | Rd = STACK | This instruction loads register Rd with a byte from the STACK. The Stack Pointer is pre-incremented by 1 before the POP. | 1 |
   | `push Rr` | STACK = Rr | This instruction stores the contents of register Rr on the STACK. The Stack Pointer is post-decremented by 1 after the PUSH. | 1 |
   | `ret` | PC(15:0) = STACK, PC(21:0) = STACK | Returns from the subroutine. The return address is loaded from the STACK. The Stack Pointer uses a pre-increment scheme during RET. | 4 |
   | `rol Rd` | tmp=C[0], C[0:14]=C[1:15], C[15]=tmp | Shifts all bits in Rd one place to the left. The C flag is shifted into bit 0 of Rd. Bit 7 is shifted into the C flag. This operation, combined with LSL, effectively multiplies multi-byte signed and unsigned values by two. | 1 |
   | `ror Rd` | tmp=C[15], C[1:14]=C[0:15], C[0]=tmp | Shifts all bits in Rd one place to the right. The C flag is shifted into bit 7 of Rd. Bit 0 is shifted into the C flag. This operation, combined with ASR, effectively divides multi-byte signed values by two. Combined with LSR, it effectively divides multi-byte unsigned values by two. The Carry flag can be used to round the result. | 1 |
