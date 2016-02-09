#ifndef __INSTRUCTION__
#define __INSTRUCTION__
#include <iostream>

class instruction
{
	typedef unsigned int uint;
public:
	instruction(){;}
	instruction(uint opcode, uint rs, uint rt, uint rd, uint immediate, 
		uint shift,uint funct, uint address){
	this->opcode = opcode;
	this->rs = rs;
	this->rt = rt;
	this->rd = rd;
	this->immediate = immediate;
	this->shift = shift;
	this->funct = funct;
	this->address = address;
}
	~instruction(){;}
	uint opcode;
	uint rs;
	uint rt;
	uint rd;
	uint immediate;
	uint shift;
	uint funct;
	uint address;
};
#endif

#ifndef __CPU__
#define __CPU__
#include <string>
#define NUM_REG 32
#define MEM_SIZE 8196
class cpu
{
	typedef unsigned int uint;
	typedef unsigned char uchar;
public:
	cpu();
	cpu(std::string Code, std::string UserKey, std::string FullString);
	~cpu();
	void Execute();
	uint GetT6();
	uint GetT7();

private:
	uint R[NUM_REG]; //32
	uint Hi;
	uint Lo;
	uchar Memory[MEM_SIZE];
	uint Pc;
	instruction instr;
	std::string Code;
	std::string UserKey;
	uint hexval(char ch);
	uint hextoint(std::string hex);
	instruction interpretinstruction();
	void FillMemory(std::string UserKey, std::string FullString);
    int NumberOfSetBits(uint i);
};
#endif
