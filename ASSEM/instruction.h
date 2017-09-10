#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

class single_instr {
public:
	single_instr(string s, int index, int addr): assem_instr(s), org_row_index(index), 
			relative_addr(addr) {}

	string get_name();

	void show() {
		cout << assem_instr << endl;
		cout << org_row_index << " " << relative_addr << endl;
	}

	friend ostream& operator << (ostream& out, const single_instr& ins);

	string assem_instr;
	string name; // such as "sub", "jal"
	int org_row_index; // to point out the position of syntax error
	int relative_addr; // address of the instruction
	string machine_code; // translate to 32-bit machine code
};

string single_instr::get_name()
{ // ===== caution: the name may be given in Caps
	 // ===== substr(a, b): start from a and end at b - 1, not b
	int end_name = assem_instr.find_first_of('$');
	if(end_name != -1) // no-J-type instructions or jalr or jr
	{
		name = assem_instr.substr(0, end_name);
		for(int i = 0; i < end_name; i++) // convert every char in name to its lower form
			name[i] = tolower(name[i]);
		return name;
	}
	else // j or jal or error
	{
		if(assem_instr.substr(0, 3) == "jal" || assem_instr.substr(0, 3) == "JAL")
			return name = "jal";
		else
			return name = "j";
	}
}

ostream& operator << (ostream& out, const single_instr& ins)
{
	out << ins.name << " ";
	out << ins.machine_code;
	return out;
}

#endif