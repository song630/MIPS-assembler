#ifndef _TABLE_DRIVEN_
#define _TABLE_DRIVEN_

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <bitset>
#include "exceptions.h"
// #include "instruction.h"
// #include "assemble.h"
using namespace std;
enum REG_ORDER {RType, Shift, IType, Branch, Jalr};
// RType:  xxx rd, rs, rt;
// Shift:  xxx rd, rt, shamt; (like sll, srl)
// IType:  xxx rt, rs, imm; (like addi)
// Branch: xxx rs, rt, imm; (like bne)
// Jalr:   xxx rs, rd;

// =========== how to use: ========== //
// 1. In test.cpp, include the three headers.
// 2. First execute get_instructions(), init_reg_map();
// 3. Then define a table_process object, like ob;
// 4. Execute ob.init(), ob.assem_all_to_bin();

class single_instr;
extern vector<pair<string, int> > L_set; // external global variables
extern vector<single_instr> I_set;
typedef map<string, string> Reg_map; // map the name of reg with an integer
Reg_map R_map;

typedef void (*Function_ptr) (single_instr& instr);
typedef map<string, Function_ptr> Function_map; // string is like "add", "sub"

class table_process { // the class has a function_map, dealing with all instructions in I_set
public:
	table_process() {}
	void init();
	// build up all key-value pairs (equal to the amount of different instructions)
	void assem_all_to_bin(); // Label_set& L_set, Instru_set& I_set
	// calculate all single_instr::machine_code

private:
	Function_map table_of_funcs;
	Function_map::iterator iter; // used to find key-value
	Function_ptr p; // point to exact one process function
};

string get_reg_number(string s);
string three_regs(single_instr& instr, REG_ORDER type);
string LOAD_SAVE_get_lower_bits(single_instr& instr);
int string_to_int(string& s);
string relative_offset_calculate(single_instr& instr);
string direct_target_calculate(single_instr& instr);
void write_to_file(char *open_path);

void init_reg_map()
{
	R_map["zero"] = "00000"; // 0
    R_map["at"] = "00001";
	R_map["v0"] = "00010";    R_map["V0"] = "00010"; // 2
	R_map["v1"] = "00011";    R_map["V1"] = "00011"; // 3
	R_map["a0"] = "00100";    R_map["A0"] = "00100"; // 4
	R_map["a1"] = "00101";    R_map["A1"] = "00101"; // 5
	R_map["a2"] = "00110";    R_map["A2"] = "00110"; // 6
	R_map["a3"] = "00111";    R_map["A3"] = "00111"; // 7
	R_map["t0"] = "01000";    R_map["T0"] = "01000"; // 8
	R_map["t1"] = "01001";    R_map["T1"] = "01001"; // 9
	R_map["t2"] = "01010";    R_map["T2"] = "01010"; // 10
	R_map["t3"] = "01011";    R_map["T3"] = "01011"; // 11
	R_map["t4"] = "01100";    R_map["T4"] = "01100"; // 12
	R_map["t5"] = "01101";    R_map["T5"] = "01101"; // 13
	R_map["t6"] = "01110";    R_map["T6"] = "01110"; // 14
	R_map["t7"] = "01111";    R_map["T7"] = "01111"; // 15
	R_map["s0"] = "10000";    R_map["S0"] = "10000"; // 16
	R_map["s1"] = "10001";    R_map["S1"] = "10001"; // 17
	R_map["s2"] = "10010";    R_map["S2"] = "10010"; // 18
	R_map["s3"] = "10011";    R_map["S3"] = "10011"; // 19
	R_map["s4"] = "10100";    R_map["S4"] = "10100"; // 20
	R_map["s5"] = "10101";    R_map["S5"] = "10101"; // 21
	R_map["s6"] = "10110";    R_map["S6"] = "10110"; // 22
	R_map["s7"] = "10111";    R_map["S7"] = "10111"; // 23
	R_map["t8"] = "11000";    R_map["T8"] = "11000"; // 24
	R_map["t9"] = "11001";    R_map["T9"] = "11001"; // 25
	R_map["gp"] = "11100";    R_map["GP"] = "11100"; // 28
	R_map["sp"] = "11101";    R_map["SP"] = "11101"; // 29
	R_map["fp"] = "11110";    R_map["FP"] = "11110"; // 30
	R_map["ra"] = "11111";    R_map["RA"] = "11111"; // 31

	R_map["r0"] = "00000";    R_map["R0"] = "00000"; // 0
	R_map["r2"] = "00010";    R_map["R2"] = "00010"; // 2
	R_map["r3"] = "00011";    R_map["R3"] = "00011"; // 3
	R_map["r4"] = "00100";    R_map["R4"] = "00100"; // 4
	R_map["r5"] = "00101";    R_map["R5"] = "00101"; // 5
	R_map["r6"] = "00110";    R_map["R6"] = "00110"; // 6
	R_map["r7"] = "00111";    R_map["R7"] = "00111"; // 7
	R_map["r8"] = "01000";    R_map["R8"] = "01000"; // 8
	R_map["r9"] = "01001";    R_map["R9"] = "01001"; // 9
	R_map["r10"] = "01010";   R_map["R10"] = "01010"; // 10
	R_map["r11"] = "01011";   R_map["R11"] = "01011"; // 11
	R_map["r12"] = "01100";   R_map["R12"] = "01100"; // 12
	R_map["r13"] = "01101";   R_map["R13"] = "01101"; // 13
	R_map["r14"] = "01110";   R_map["R14"] = "01110"; // 14
	R_map["r15"] = "01111";   R_map["R15"] = "01111"; // 15
	R_map["r16"] = "10000";   R_map["R16"] = "10000"; // 16
	R_map["r17"] = "10001";   R_map["R17"] = "10001"; // 17
	R_map["r18"] = "10010";   R_map["R18"] = "10010"; // 18
	R_map["r19"] = "10011";   R_map["R19"] = "10011"; // 19
	R_map["r20"] = "10100";   R_map["R20"] = "10100"; // 20
	R_map["r21"] = "10101";   R_map["R21"] = "10101"; // 21
	R_map["r22"] = "10110";   R_map["R22"] = "10110"; // 22
	R_map["r23"] = "10111";   R_map["R23"] = "10111"; // 23
	R_map["r24"] = "11000";   R_map["R24"] = "11000"; // 24
	R_map["r25"] = "11001";   R_map["R25"] = "11001"; // 25
	R_map["r28"] = "11100";   R_map["R28"] = "11100"; // 28
	R_map["r29"] = "11101";   R_map["R29"] = "11101"; // 29
	R_map["r30"] = "11110";   R_map["R30"] = "11110"; // 30
	R_map["r31"] = "11111";   R_map["R31"] = "11111"; // 31
}

string get_reg_number(string s) // return a integer ranges from 0-31
{ // return -1 if there is an error
	// string s is like (r30, R5), or (sp, FP, ra, v0, a2, T6, S8)
	Reg_map::iterator it;
	it = R_map.find(s);
	if(it != R_map.end()) // found
		return it->second;
	else
        throw Exception("Invalid register.");
        // cout << "invalid register" << endl; // exception
}

void table_process::assem_all_to_bin() // ===== MAIN PROCESS
{
	vector<single_instr>::iterator i;
	for(i = I_set.begin(); i != I_set.end(); i++)
	{
		string instr_name = (*i).get_name(); // get name of the instruction
		if(!instr_name.empty()) // exception
		{
			iter = table_of_funcs.begin();
			iter = table_of_funcs.find(instr_name); // find the correct function
			if(iter != table_of_funcs.end()) // found
			{
				// cout << (*i).assem_instr << endl;
				// cout << (*i).name << endl;
				p = iter->second; // point to the function
				(*p)(*i); // process
				// cout << (*i) << endl;
			}
			else // not found. exception
                throw Exception("Function not found.");
                // cout << "function not found!" << endl;
		}
	}
}

void write_to_file(char *open_path)
{
    ofstream fout(open_path);
    fout << "memory_initialization_radix=2;\n";
    fout << "memory_initialization_vector=\n";
    vector<single_instr>::iterator instr_iter = I_set.begin();
    for(; instr_iter != I_set.end() - 1; instr_iter++)
        fout << (*instr_iter).machine_code << ",\n";
    fout << (*instr_iter).machine_code << ";\n";
    fout.flush();
    fout.close();
}

string three_regs(single_instr& instr, REG_ORDER type) // deal with ALU-R-type
{ // ===== in assembly language, the order is rd, rs, rt;
	// but in machine code, the order is rs, rt, rd.
	string rd, rs, rt;
	string r1, r2, r3;
	instr.assem_instr.erase(0, instr.name.size()); // now the first char is '$'

	// first reg:
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
        throw Exception("Syntax error.");
        // cout << "error" << endl;
	r1 = instr.assem_instr.substr(1, reg_end - 1);
	instr.assem_instr.erase(0, reg_end + 1);

	// second reg:
	reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
        throw Exception("Syntax error.");
        // cout << "error" << endl;
	r2 = instr.assem_instr.substr(1, reg_end - 1);
	instr.assem_instr.erase(0, reg_end + 1);

	// third reg:
	if(type == RType) // among the three types, only RType has the third reg
	{
		reg_end = instr.assem_instr.find_first_of(';');
		if(reg_end == -1)
            throw Exception("Syntax error.");
            // cout << "error" << endl;
		r3 = instr.assem_instr.substr(1, reg_end - 1);
	}

	if(type == RType)
	{
		rd = get_reg_number(r1);
		rs = get_reg_number(r2);
		rt = get_reg_number(r3);
		return rs + rt + rd;
	}
	else if(type == IType)
	{
		rt = get_reg_number(r1);
		rs = get_reg_number(r2);
		return rs + rt;
	}
	else if(type == Shift) // put the case of the least probability at the last
	{
		rd = get_reg_number(r1);
		rt = get_reg_number(r2);
		return rt + rd;
	}
	else if(type == Branch) // type == Branch
	{
		rs = get_reg_number(r1);
		rt = get_reg_number(r2);
		return rs + rt;
	}
	else // type == Jalr
	{
		rs = get_reg_number(r1);
		rd = get_reg_number(r2);
		return rs + rd;
	}
}

// apply to: lw, lb, lbu, lh, lhu, sw, sb, sh
string LOAD_SAVE_get_lower_bits(single_instr& instr) // lw$fp,48($sp);
{
	string rt, rs, offset;
	instr.assem_instr.erase(0, instr.name.size()); // $fp,48($sp);
	// first reg:
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
        throw Exception("Syntax error.");
        // cout << "error" << endl;
	rt = instr.assem_instr.substr(1, reg_end - 1);
	rt = get_reg_number(rt);
	instr.assem_instr.erase(0, reg_end + 1); // 48($sp);
	// immediate:
	reg_end = instr.assem_instr.find_first_of('(');
	if(reg_end == -1)
        throw Exception("Syntax error.");
        // cout << "error" << endl;
	offset = instr.assem_instr.substr(0, reg_end);
	int imme = string_to_int(offset);
	// cout << "lw&sw: " << imme << endl;
	bitset<16> bin_imme = imme;
	offset = bin_imme.to_string();
	instr.assem_instr.erase(0, reg_end + 2); // sp);
	// second reg:
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // sp)
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // sp
	rs = get_reg_number(instr.assem_instr);
	return rs + rt + offset;
}

int string_to_int(string& s) // translate the imm string to an int (then to binary string)
{
	string::iterator s_iter = s.begin();
	int rtn = 0;
	if(s[0] == '-') // ===== a negative number, should calculate its complement later
		s_iter++;
	for(; s_iter != s.end(); s_iter++)
		rtn = rtn * 10 + (*s_iter) - '0';
	if(s[0] == '-')
		rtn = -rtn;
	return rtn;
}

// apply to beq, bne, blez, bgtz, bltz, bgtz
string relative_offset_calculate(single_instr& instr) // deal with Label;
{ // use vector<pair<string, int> > L_set;
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // what's left: Label
	int start_addr = instr.relative_addr * 4 + 4; // addr of next instruction
	int end_addr; // destination
	int offset_int;
	string offset_str;
	vector<pair<string, int> >::iterator l_iter = L_set.begin();
	for(; l_iter != L_set.end(); l_iter++)
		if(instr.assem_instr == l_iter->first) // match label
		{
			end_addr = (l_iter->second) * 4;
			break;
		}
	if(l_iter == L_set.end())
	{
        cout << instr.assem_instr << endl;
        // throw Exception("Label dismatch");
        // cout << "dismatch" << endl; // exception
	}
	offset_int = end_addr - start_addr;
	bitset<16> offset_bin = offset_int >> 2; // ===== right shift
	offset_str = offset_bin.to_string();
	return offset_str;
}

// apply to j, jal
string direct_target_calculate(single_instr& instr) // deal with Label;
{
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // what's left: Label
	int target; // destination
	string target_str;
	vector<pair<string, int> >::iterator l_iter = L_set.begin();
	for(; l_iter != L_set.end(); l_iter++)
		if(instr.assem_instr == l_iter->first) // match label
		{
			target = l_iter->second; // do not *4, or it has to be shifted right later
			break;
		}
	if(l_iter == L_set.end())
	{
        cout << instr.assem_instr << endl;
        // throw Exception("Label dismatch.");
        // cout << "dismatch" << endl; // exception
	}
	bitset<26> target_bin = target;
	target_str = target_bin.to_string();
	return target_str;
}

// ================== ALU-R ===================== //

void process_add(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
    string func = "100000";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_sub(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
    string func = "100010";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_addu(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
    string func = "100001";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_subu(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
    string func = "100011";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_slt(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "101010";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_sltu(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "101011";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_and(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "100100";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_or(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "100101";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_xor(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "100110";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_nor(single_instr& instr)
{
	REG_ORDER type = RType;
	string regs = three_regs(instr, type);
	string func = "100111";
	instr.machine_code = "000000" + regs + "00000" + func;
}

void process_sll(single_instr& instr)
{
	REG_ORDER type = Shift;
	string regs = three_regs(instr, type); // the front part of string is removed

	// get shamt:
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // remove ';', leaving only imm
	int shift_amount = string_to_int(instr.assem_instr);
	bitset<5> bin_sh_amt = shift_amount;
	string shamt = bin_sh_amt.to_string();

	string func = "000000";
	instr.machine_code = "00000000000" + regs + shamt + func;
}

void process_srl(single_instr& instr)
{
	REG_ORDER type = Shift;
	string regs = three_regs(instr, type); // the front part of string is removed

	// get shamt:
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // remove ';', leaving only imm
	int shift_amount = string_to_int(instr.assem_instr);
	bitset<5> bin_sh_amt = shift_amount;
	string shamt = bin_sh_amt.to_string();

	string func = "000010";
	instr.machine_code = "00000000000" + regs + shamt + func;
}

void process_sra(single_instr& instr)
{
	REG_ORDER type = Shift;
	string regs = three_regs(instr, type); // the front part of string is removed

	// get shamt:
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // remove ';', leaving only imm
	int shift_amount = string_to_int(instr.assem_instr);
	bitset<5> bin_sh_amt = shift_amount;
	string shamt = bin_sh_amt.to_string();

	string func = "000011";
	instr.machine_code = "00000000000" + regs + shamt + func;
}

// ================== ALU-I ==================== //

void process_addi(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001000";
	string regs = three_regs(instr, type);

	// get imme (16-bit):
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // remove ';', leaving only imm
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	// ===== bitset will automatically convert negative number to its complement
	string imme = bin_oper.to_string();

	instr.machine_code = opcode + regs + imme;
}

// ===== sign-extend the immediate in addi, zero-extend the immediate in addiu.
// bitset<n> bin = num: default is sign-extension.

void process_addiu(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001001";
	string regs = three_regs(instr, type);
	// get imme (16-bit):
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_andi(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001100";
	string regs = three_regs(instr, type);
	// get imme:
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_ori(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001101";
	string regs = three_regs(instr, type);
	// get imme:
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_xori(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001110";
	string regs = three_regs(instr, type);
	// get imme:
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_slti(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001010";
	string regs = three_regs(instr, type);
	// get imme:
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_sltiu(single_instr& instr)
{
	REG_ORDER type = IType;
	string opcode = "001011";
	string regs = three_regs(instr, type);
	// get imme:
	instr.assem_instr.erase(instr.assem_instr.end() - 1);
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + regs + imme;
}

void process_lui(single_instr& instr) // rs = "00000"
{ // like lui$t0,100;
	string opcode = "001111";
	instr.assem_instr.erase(0, 3); // what's left: $t0,100;
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end <= 1)
        throw Exception("Syntax error.");
        // cout << "error" << endl; // exception
	string rt = instr.assem_instr.substr(1, reg_end - 1);
	rt = get_reg_number(rt);

	instr.assem_instr.erase(0, reg_end + 1); // what's left: 100;
	instr.assem_instr.erase(instr.assem_instr.end() - 1); // what's left: 100
	int operand = string_to_int(instr.assem_instr);
	bitset<16> bin_oper = operand;
	string imme = bin_oper.to_string();
	instr.machine_code = opcode + "00000" + rt + imme;
}

// =================== Fetch ==================== //

void process_lw(single_instr& instr)
{
	string opcode = "100011";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr); // rs + rt + offset
	instr.machine_code = opcode + lower_26_bit;
}

void process_lb(single_instr& instr)
{
	string opcode = "100000";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

void process_lbu(single_instr& instr)
{
	string opcode = "100100";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

void process_lh(single_instr& instr)
{
	string opcode = "100001";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

void process_lhu(single_instr& instr)
{
	string opcode = "100101";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

// ==================== Save ==================== //

void process_sw(single_instr& instr)
{
	string opcode = "101011";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

void process_sb(single_instr& instr)
{
	string opcode = "101000";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

void process_sh(single_instr& instr)
{
	string opcode = "101001";
	string lower_26_bit = LOAD_SAVE_get_lower_bits(instr);
	instr.machine_code = opcode + lower_26_bit;
}

// ==================== Branch ===================== //

void process_beq(single_instr& instr)
{
	REG_ORDER type = Branch;
	string opcode = "000100";
	string regs = three_regs(instr, type); // what's left: Label;
	// deal with the label:
	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + regs + offset;
}

void process_bne(single_instr& instr) // the same as above
{
	REG_ORDER type = Branch;
	string opcode = "000101";
	string regs = three_regs(instr, type);
	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + regs + offset;
}

void process_blez(single_instr& instr) // blez$s0,Lable;
{
    // REG_ORDER type = Branch;
	string opcode = "000110";
	string rs;

	// get rs:
	instr.assem_instr.erase(0, instr.name.size()); // $s0,Lable;
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
		cout << "error" << endl;
	rs = instr.assem_instr.substr(1, reg_end - 1);
	rs = get_reg_number(rs);
	instr.assem_instr.erase(0, reg_end + 1); // Lable;

	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + rs + "00000" + offset;
}

void process_bgtz(single_instr& instr) // the same as blez
{
    // REG_ORDER type = Branch;
	string opcode = "000111";
	string rs;
	instr.assem_instr.erase(0, instr.name.size());
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
		cout << "error" << endl;
	rs = instr.assem_instr.substr(1, reg_end - 1);
	rs = get_reg_number(rs);
	instr.assem_instr.erase(0, reg_end + 1);
	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + rs + "00000" + offset;
}

void process_bltz(single_instr& instr) // the same as blez
{
    // REG_ORDER type = Branch;
	string opcode = "000001";
	string rs;
	instr.assem_instr.erase(0, instr.name.size());
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
		cout << "error" << endl;
	rs = instr.assem_instr.substr(1, reg_end - 1);
	rs = get_reg_number(rs);
	instr.assem_instr.erase(0, reg_end + 1);
	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + rs + "00000" + offset;
}

void process_bgez(single_instr& instr) // the same as blez
{
    // REG_ORDER type = Branch;
	string opcode = "000001";
	string rs;
	instr.assem_instr.erase(0, instr.name.size());
	int reg_end = instr.assem_instr.find_first_of(',');
	if(reg_end == -1)
		cout << "error" << endl;
	rs = instr.assem_instr.substr(1, reg_end - 1);
	rs = get_reg_number(rs);
	instr.assem_instr.erase(0, reg_end + 1);
	string offset = relative_offset_calculate(instr);
	instr.machine_code = opcode + rs + "00001" + offset;
}

// ==================== Jump =================== //

void process_j(single_instr& instr) // jLOOP;
{
	string opcode = "000010";
	instr.assem_instr.erase(0, instr.name.size()); // LOOP;
	string target = direct_target_calculate(instr);
	instr.machine_code = opcode + target;
}

void process_jal(single_instr& instr) // the same as above
{
	string opcode = "000011";
	instr.assem_instr.erase(0, instr.name.size());
	string target = direct_target_calculate(instr);
	instr.machine_code = opcode + target;
}

void process_jalr(single_instr& instr) // jalr$s0,$s1;
{
	REG_ORDER type = Jalr;
	string opcode = "000000";
	string regs = three_regs(instr, type); // get 10-bit rs + rd
	string rd = regs.substr(5, 9);
	string rs = regs.erase(5, 9);
	string func = "001001";
	instr.machine_code = opcode + rs + "00000" + rd + "00000" + func;
}

void process_jr(single_instr& instr) // jr$ra;
{
	string opcode = "000000";
	string rs = "11111";
	string func = "001000";
	instr.machine_code = opcode + rs + "00000" + "00000" + "00000" + func;
}

void table_process::init()
{
	// ====================== ALU-R ======================== //
	
	table_of_funcs.insert(make_pair<string, Function_ptr>("add",
		reinterpret_cast<Function_ptr>(&process_add)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sub",
		reinterpret_cast<Function_ptr>(&process_sub)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("addu",
		reinterpret_cast<Function_ptr>(&process_addu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("subu",
		reinterpret_cast<Function_ptr>(&process_subu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("slt",
		reinterpret_cast<Function_ptr>(&process_slt)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sltu",
		reinterpret_cast<Function_ptr>(&process_sltu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("and",
		reinterpret_cast<Function_ptr>(&process_and)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("or",
		reinterpret_cast<Function_ptr>(&process_or)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("xor",
		reinterpret_cast<Function_ptr>(&process_xor)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("nor",
		reinterpret_cast<Function_ptr>(&process_nor)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sll",
		reinterpret_cast<Function_ptr>(&process_sll)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("srl",
		reinterpret_cast<Function_ptr>(&process_srl)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sra",
		reinterpret_cast<Function_ptr>(&process_sra)));

	// ====================== ALU-I ======================== //

	table_of_funcs.insert(make_pair<string, Function_ptr>("addi",
		reinterpret_cast<Function_ptr>(&process_addi)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("addiu",
		reinterpret_cast<Function_ptr>(&process_addiu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("andi",
		reinterpret_cast<Function_ptr>(&process_andi)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("ori",
		reinterpret_cast<Function_ptr>(&process_ori)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("xori",
		reinterpret_cast<Function_ptr>(&process_xori)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("slti",
		reinterpret_cast<Function_ptr>(&process_slti)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sltiu",
		reinterpret_cast<Function_ptr>(&process_sltiu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("lui",
		reinterpret_cast<Function_ptr>(&process_lui)));

	// ====================== Fetch ========================= //

	table_of_funcs.insert(make_pair<string, Function_ptr>("lw",
		reinterpret_cast<Function_ptr>(&process_lw)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("lb",
		reinterpret_cast<Function_ptr>(&process_lb)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("lbu",
		reinterpret_cast<Function_ptr>(&process_lbu)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("lh",
		reinterpret_cast<Function_ptr>(&process_lh)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("lhu",
		reinterpret_cast<Function_ptr>(&process_lhu)));

	// ====================== Save ========================== //

	table_of_funcs.insert(make_pair<string, Function_ptr>("sw",
		reinterpret_cast<Function_ptr>(&process_sw)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sb",
		reinterpret_cast<Function_ptr>(&process_sb)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("sh",
		reinterpret_cast<Function_ptr>(&process_sh)));

	// ====================== Branch ======================== //

	table_of_funcs.insert(make_pair<string, Function_ptr>("beq",
		reinterpret_cast<Function_ptr>(&process_beq)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("bne",
		reinterpret_cast<Function_ptr>(&process_bne)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("blez",
		reinterpret_cast<Function_ptr>(&process_blez)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("bgtz",
		reinterpret_cast<Function_ptr>(&process_bgtz)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("bltz",
		reinterpret_cast<Function_ptr>(&process_bltz)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("bgez",
		reinterpret_cast<Function_ptr>(&process_bgez)));

	// ====================== Jump ========================== //

	table_of_funcs.insert(make_pair<string, Function_ptr>("j",
		reinterpret_cast<Function_ptr>(&process_j)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("jal",
		reinterpret_cast<Function_ptr>(&process_jal)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("jalr",
		reinterpret_cast<Function_ptr>(&process_jalr)));
	table_of_funcs.insert(make_pair<string, Function_ptr>("jr",
		reinterpret_cast<Function_ptr>(&process_jr)));
}

#endif
