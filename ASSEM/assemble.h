#ifndef _ASSEMBLE_H_
#define _ASSEMBLE_H_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <utility> // use pair<>
#include <vector>
#include "exceptions.h"
// #include "instruction.h" // class instruction
#define I_DIRECTORY "D://code_blocks/assembler/example1.txt"
#define BASE_ADDR 0 // if the memory has both data area and text (instr) area, it may be changed

using namespace std;

typedef pair<string, int> Label_info; // name and address of a label, to be matched later
typedef vector<pair<string, int> > Label_set;
typedef vector<single_instr> Instru_set; // a set of all single instruction objects
Label_set L_set;
Instru_set I_set;

string& trim_space(string& s)
{
	if(s.empty()) // null string
		return s;
	string::iterator iter;
	int first_valid = s.find_first_not_of(" \t");
	if(first_valid == -1) // ===== a string that has only space or tab
		return s.erase();
	iter = s.begin() + first_valid;
	s.erase(s.begin(), iter); // erase all spaces or tabs before the string
	iter = s.begin() + s.find_last_not_of(" \t");
	s.erase(iter + 1, s.end()); // erase all spaces or tabs after the string

	// remove all the spaces or tabs in the middle of string
	for(iter = s.begin(); iter != s.end(); iter++)
	{
		if(*iter == ' ' || *iter == '\t') // ===== ERROR: *iter == " ".
		{
			s.erase(iter);
			iter--;
		}
	}
	return s;
}

string& comment_filter(string& s) // do not support comment that covers multiple lines
{
	if(s.empty())
		return s;
	string::iterator iter;
	int start_pos;
	// find the position of comments, notice that '#' and "//" may appear in comment more than once
	// cout << s.find_first_of("#") << endl;
	start_pos = min(s.find_first_of("#"), s.find_first_of("//"));
	if(start_pos == -1) // ===== if not found, find_first_of() will return -1
		return s;
	iter = s.begin() + start_pos;
	s.erase(iter, s.end());
	return s;
}

void get_Instructions(char *open_path) // fill the empty Label_set and Instru_set.
{ // records labels: (just the ons in front of instructions, not those in J, JAL, BEQ and BNE)
    ifstream fin(open_path);
	if(!fin)
        throw Exception("Failed to open the file.");
        // cout << "Failed to open." << endl; // an exception
	string raw_instr; // instructions that have not been dealt with
	int row_index = 0; // every single instruction records its original posision, for syntax check
	int addr = BASE_ADDR; // it will be used when breaking string of a row into several instructions

	// ===== two getline() functions in C++ =======
	// I/O getline(): fin.getline(char*, const int); must use char []
	// file getline(): getline(fstream&, string&);
	while(getline(fin, raw_instr))
	{
		row_index++;
		if(raw_instr.empty()) // empty string
			continue;
		raw_instr = comment_filter(raw_instr);
		if(raw_instr.empty()) // a string that has only comment
			continue;
        raw_instr = trim_space(raw_instr);
		if(raw_instr.empty()) // a string that has only space and tab
			continue;
		int label_end; // the position of ':'
		int instr_end; // the position of ';'
		string label, instruction;
		while(!raw_instr.empty()) // every time a label or an instruction is drew, it is removed
		{
			label_end = raw_instr.find_first_of(":");
			instr_end = raw_instr.find_first_of(";");

			// ===== ERROR: special case: j LOOP; Exit1: lw  $s0, 40($sp);
            if((instr_end != -1 && label_end != -1 && instr_end < label_end)
            || (label_end == -1 && instr_end != -1))
			{
				if(instr_end != -1) // found
				{
					if(instr_end == 0) // just ';' with no instruction
                        throw Exception("Syntax Error: no instruction.");
                        // cout << "error!" << endl; // exception
					instruction = raw_instr.substr(0, instr_end + 1);
					I_set.push_back(single_instr(instruction, row_index, addr));
					raw_instr.erase(raw_instr.begin(), raw_instr.begin() + instr_end + 1);
					addr++;

					if(raw_instr.empty())
						break;
					label_end = raw_instr.find_first_of(":"); // update after deleting some part
				}
			}

			if(label_end != -1) // found
			{
				if(label_end == 0) // just ':' with no label name
                    throw Exception("No label name.");
                    // cout << "error!" << endl; // exception
				label = raw_instr.substr(0, label_end); // ===== ERROR: label_end + 1 (no ':')
				// ===== ERROR: label_end must add 1
                pair<string, int> temp_pair;
                temp_pair.first = label;
                temp_pair.second = addr;
                L_set.push_back(temp_pair);
				raw_instr.erase(raw_instr.begin(), raw_instr.begin() + label_end + 1);
				// ===== ERROR: label_end must add 1
			}
		}
	}
}

#endif
