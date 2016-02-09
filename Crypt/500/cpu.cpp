#include "cpu.h"
#include <iostream>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;
cpu::cpu()
{
}

cpu::cpu(std::string Code, std::string UserKey, std::string FullString)
{
    this->Code = Code;
    this->Pc = 0; //reset vector at 0
    for(int ii = 0; ii < NUM_REG; ii++)
    {
        this->R[ii] = 0;
    }
    for(int ii = 0; ii < MEM_SIZE; ii++)
    {
        this->Memory[ii] = 0;
    }
    FillMemory(UserKey, FullString);
}

cpu::~cpu()
{
}

void cpu::FillMemory(std::string UserKey, std::string FullString)
{
    for(int ii = 0; ii < 8; ii++)
    {
        Memory[ii] = UserKey[ii];
    }
    for(int ii = 0; ii < 16; ii++)
    {
        Memory[8+ii] = FullString[ii];
    }
}

uint cpu::GetT6()
{
    return R[14];
}

uint cpu::GetT7()
{
    return R[15];
}

uint cpu::hexval(char ch)
{
    switch(ch)
    {
        case 'a':
        case 'A':
            return 10;
        case 'b':
        case 'B':
            return 11;
        case 'c':
        case 'C':
            return 12;
        case 'd':
        case 'D':
            return 13;
        case 'e':
        case 'E':
            return 14;
        case 'f':
        case 'F':
            return 15;
        default:
            return (uint)(ch-'0');
    }
}

uint cpu::hextoint(std::string hex)
{
    uint retval = 0;
    int string_length = hex.length();
    for(int ii = 0; ii < string_length; ii++)
    {
        char ch = hex[(string_length - ii) - 1];
        uint multiplier = 1;
        uint chint = hexval(ch);
        for(int jj = 0; jj < ii; jj++)
        {
            multiplier *= 16;
        }
        retval +=  chint * multiplier;
    }
    return retval;
}

instruction cpu::interpretinstruction()
{
    instruction result;
    if(this->Pc > (this->Code.length() - 4))
    {
        result.opcode = 255;
        return result; //halt opcode because it's not enough to construct a complete instruction
    }
    result.opcode = (hextoint(this->Code.substr(this->Pc, 2))) >> 2; //bits 0-5
    result.rs = (hextoint(this->Code.substr(this->Pc + 1, 2)) >> 1) & 0x1F; //bits 6-10
    result.rt = (hextoint(this->Code.substr(this->Pc + 2, 2))) & 0x1F; //bits 11-15
    result.rd = hextoint(this->Code.substr(this->Pc + 4, 2)) >> 3; //bits 16-20
    result.shift = (hextoint(this->Code.substr(this->Pc + 5, 2)) >> 2) &0x1F; //bits 21-25
    result.funct = hextoint(this->Code.substr(this->Pc + 6, 2)) & 0x3F; //bits 26-31
    result.immediate = hextoint(this->Code.substr(this->Pc + 4, 4)); //bits 16-31
    result.address = hextoint(this->Code.substr(this->Pc + 1, 7)) & 0x3FFFFF; //bits 0-25
    this->Pc += 8;
    return result;
}

int cpu::NumberOfSetBits(uint i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void cpu::Execute()
{
    instruction instr;
    while((instr = interpretinstruction()).opcode != 255)
    {
        uint diff = 0;
        switch (instr.opcode)
        {
            case 0x00: //R-Type operations
                {
                    switch (instr.funct) //different R-Types all have the same opcode
                    {
                        case 0x20: //add
                        case 0x21: //addu
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rs] + this->R[instr.rt];
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x24: //and
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rs] & this->R[instr.rt];
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }
                        case 0x1A: //div
                        case 0x1B: //divu
                            {
                                this->Lo = this->R[instr.rs] / this->R[instr.rt];
                                this->Hi = this->R[instr.rs] % this->R[instr.rt];
                                break;
                            }

                        case 0x08: //jr
                            {
                                uint initial = this->R[instr.rs];
                                this->Pc = this->R[instr.rs];
                                diff = NumberOfSetBits(initial ^ this->Pc);
                                break;
                            }

                        case 0x10: //mfhi
                            {
                                this->R[instr.rd] = this->Hi;
                                break;
                            }
                        case 0x12: //mflo
                            {
                                this->R[instr.rd] = this->Lo;
                                break;
                            }

                        case 0x18: //mult
                        case 0x19: //multu
                            {
                                this->Lo = this->R[instr.rs] * this->R[instr.rt];
                                break;
                            }

                        case 0x27: //nor
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = ~(this->R[instr.rs] | this->R[instr.rt]);
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x26: //xor
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rs] ^ this->R[instr.rt];
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x25: //or
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rs] | this->R[instr.rt];
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x2A: //slt
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = 0;
                                if(this->R[instr.rs] < this->R[instr.rt])
                                {
                                    this->R[instr.rd] = 1;
                                }
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x2B: //sltu
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = 0;
                                if((uint)this->R[instr.rs] < (uint)this->R[instr.rt])
                                {
                                    this->R[instr.rd] = 1;
                                }
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x00: //sll
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rt] << instr.shift;
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x02: //srl
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rt] >> instr.shift;
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x03: //sra (omg, hax)
                            {
                                uint initial = this->R[instr.rd];
                                bool hax = false;
                                if((this->R[instr.rt] & 0x8000000) != 0)
                                {
                                    hax = true;
                                }
                                this->R[instr.rd] = this->R[instr.rt] >> instr.shift;
                                if(hax)
                                {
                                    int andme = -1;
                                    andme = andme << (32 - instr.shift);
                                    this->R[instr.rd] &= andme;
                                }
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x22: //sub
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = this->R[instr.rs] - this->R[instr.rt];
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        case 0x23: //subu
                            {
                                uint initial = this->R[instr.rd];
                                this->R[instr.rd] = (int)((uint)this->R[instr.rs] - (uint)this->R[instr.rt]);
                                diff = NumberOfSetBits(initial ^ this->R[instr.rd]);
                                break;
                            }

                        default: //i don't know what the user tried to do here
                            {
                                break;
                            }
                    }

                }
                break;

            case 0x02: //j
                {
                    uint initial = this->Pc;
                    this->Pc = ((this->Pc & 0xf0000000) | (instr.address << 2));
                    diff = NumberOfSetBits(initial ^ this->Pc);
                    break;
                }

            case 0x03: //jal
                {
                    uint initial = this->Pc;
                    uint initial2 = this->R[31];
                    this->R[31] = this->Pc; //load up the linking register
                    this->Pc = ((this->Pc & 0xf0000000) | (instr.address << 2));
                    diff = NumberOfSetBits(initial ^ this->Pc);
                    diff += NumberOfSetBits(initial2 ^ this->R[31]);
                    break;
                }	

            case 0x04: //beq branches are absolute in this cpu
                {
                    uint initial = this->Pc;
                    if(this->R[instr.rs] == this->R[instr.rt])
                    {
                        this->Pc = instr.immediate << 3;
                    }
                    diff = NumberOfSetBits(initial ^ this->Pc);
                    break;
                }

            case 0x05: //bne this is not done the "mips way" branches are absolute here
                {
                    uint initial = this->Pc;
                    if(this->R[instr.rs] != this->R[instr.rt])
                    {
                        this->Pc = instr.immediate << 3;
                    }
                    diff = NumberOfSetBits(initial ^ this->Pc);
                    break;
                }

            case 0x08: //addi
            case 0x09: //addiu
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = this->R[instr.rs] + instr.immediate;
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }

            case 0x0A: //slti
            case 0x0B: //sltiu
                {
                    uint initial = this->R[instr.rt];
                    if(this->R[instr.rs] < this->R[instr.immediate])
                    {
                        this->R[instr.rt] = 1;
                    }
                    else
                    {
                        this->R[instr.rt] = 0;
                    }
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }

            case 0x0C: //andi
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = this->R[instr.rs] & instr.immediate;
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }	

            case 0x0D: //ori
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = this->R[instr.rs] | instr.immediate;
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }

            case 0x23: //lw
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = (this->Memory[instr.immediate + instr.rs + 3]) +  
                        (this->Memory[instr.immediate + instr.rs + 2] << 8) +  
                        (this->Memory[instr.immediate + instr.rs + 1] << 16) +
                        (this->Memory[instr.immediate + instr.rs] << 24);
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }

            case 0x24: //lbu
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = this->Memory[instr.immediate + instr.rs];
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }

            case 0x25: //lhu
                {
                    uint initial = this->R[instr.rt];
                    this->R[instr.rt] = (this->Memory[instr.immediate + instr.rs] << 8) + (this->Memory[instr.immediate + instr.rs + 1]);
                    diff = NumberOfSetBits(initial ^ this->R[instr.rt]);
                    break;
                }	

            case 0x28: //sb
                {
                    uint initial = this->Memory[instr.immediate + instr.rs];
                    this->Memory[instr.immediate + instr.rs] = (uchar)(this->R[instr.rt] & 0xFF);
                    diff = NumberOfSetBits(initial ^ this->Memory[instr.immediate + instr.rs]);
                    break;
                }

            case 0x29: //shu
                {
                    int stored = this->R[instr.rt];
                    char ch[2];
                    ch[0] = (stored >> 8) & 0xFF;
                    ch[1] = stored & 0xFF;
                    for(int ii = 0; ii < 2; ii++)
                    {
                        diff += NumberOfSetBits(ch[ii] ^ this->Memory[instr.immediate + instr.rs + ii]);
                        this->Memory[instr.immediate + instr.rs + ii] = ch[ii];
                    }
                    break;
                }

            case 0x2B: //sw
                {
                    int stored = this->R[instr.rt];
                    char ch[4];
                    ch[0] = stored >> 24 & 0xFF;
                    ch[1] = (stored >> 16) & 0xFF;
                    ch[2] = (stored >> 8) & 0xFF;
                    ch[3] = stored & 0xFF;
                    for(int ii = 0; ii < 4; ii++)
                    {
                        diff += NumberOfSetBits(ch[ii] ^ this->Memory[instr.immediate + instr.rs + ii]);
                        this->Memory[instr.immediate + instr.rs + ii] = ch[ii];
                    }
                    break;
                }

            default:
                break;
        }
        cout  << diff << "," <<  ends;
        this->R[0] = 0; //hardwired to 0. this is as close as I can get to that :-)
    }
    cout << "0" << endl;
}
