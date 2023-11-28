#include "lab.h"
#include <stdint.h>
#include <stdio.h>

void setControl(uint32_t instBits, ControlSignals& outputSignals) {
  uint32_t opcode = instBits & 0x0000007F;  // get opcode of risc-v instruction

  // set control values by opcode
  switch (opcode) {
    // add, sub, or, and
    case 0x00000033:
      outputSignals.Branch = Zero;
      outputSignals.MemRead = Zero;
      outputSignals.MemtoReg = Zero;
      outputSignals.ALUOp = 2;
      outputSignals.MemWrite = Zero;
      outputSignals.ALUSrc = Zero;
      outputSignals.RegWrite = One;
      break;
    // addi, andi, ori
    case 0x00000013:
      outputSignals.Branch = Zero;
      outputSignals.MemRead = Zero;
      outputSignals.MemtoReg = Zero;
      outputSignals.ALUOp = 2;
      outputSignals.MemWrite = Zero;
      outputSignals.ALUSrc = One;
      outputSignals.RegWrite = One;
      break;
    // lw
    case 0x00000003:
      outputSignals.Branch = Zero;
      outputSignals.MemRead = One;
      outputSignals.MemtoReg = One;
      outputSignals.ALUOp = 0;
      outputSignals.MemWrite = Zero;
      outputSignals.ALUSrc = One;
      outputSignals.RegWrite = One;
      break;
    // sw
    case 0x00000023:
      outputSignals.Branch = Zero;
      outputSignals.MemRead = Zero;
      outputSignals.MemtoReg = DC;
      outputSignals.ALUOp = 0;
      outputSignals.MemWrite = One;
      outputSignals.ALUSrc = One;
      outputSignals.RegWrite = Zero;
      break;
    // beq
    case 0x00000063:
      outputSignals.Branch = One;
      outputSignals.MemRead = Zero;
      outputSignals.MemtoReg = DC;
      outputSignals.ALUOp = 1;
      outputSignals.MemWrite = Zero;
      outputSignals.ALUSrc = Zero;
      outputSignals.RegWrite = Zero;
      break;
  }
}

// Assume that the lower 32-bits of instBits contain the instruction.
uint32_t getExtendedBits(uint32_t instBits) {
  uint32_t returnVal = 0;
  uint32_t opcode = instBits & 0x0000007F;  // get opcode of risc-v instruction

  // sign extend the bits
  switch (opcode) {
    // add, sub, or, and (R-Type)
    case 0x00000033:
      break;
    // addi, andi, ori (I-Type)
    case 0x00000013:
      if (instBits >> 31) {
        returnVal = instBits >> 20;
        returnVal = returnVal | 0xFFFFF000;
      }
      else {
        returnVal = instBits >> 20;
      }
      break;
    // lw (I-Type)
    case 0x00000003:
      returnVal = instBits >> 20;
      break;
    // sw (S-Type)
    case 0x00000023:
      returnVal = ((instBits >> 20) & 0x00000FE0) | ((instBits >> 7) & 0x0000001F);
      break;
    // beq (SB-Type)
    case 0x00000063:
      returnVal = ((instBits >> 19) & (1 << 12)) | ((instBits << 4) & (1 << 11)) | ((instBits >> 20) & 0x000003F0) | ((instBits >> 7) & 0x0000001E);
      break;
  }

  return returnVal;
}
