#ifndef BINARY_INSTRUCTION_H
#define BINARY_INSTRUCTION_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

class map_table;
class bin_instr {
public:
    bin_instr(string bin_string, int address): machine_instr(bin_string), rela_addr(address) {}
    string get_name() {
        return name;
    }

    string get_type() {
        return type;
    }

    void set_assem_instr(const string& s) {
        assemble_instr = s;
    }

    string get_machine_code() {
        return machine_instr;
    }

    int get_addr() {
        return rela_addr;
    }

    friend ofstream& operator << (ofstream& fout, const bin_instr& instr);
    // output in the target file directly
    friend void decide_name(bin_instr& instr, map_table& mt);
    void show() {
        cout << machine_instr << endl;
    }

private:
    string name;
    string type;
    string machine_instr;
    string assemble_instr;
    int rela_addr;
};

ofstream& operator << (ofstream& fout, const bin_instr& instr)
{
    fout << instr.assemble_instr;
    return fout;
}

#endif // BINARY_INSTRUCTION_H
