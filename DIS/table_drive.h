#ifndef TABLE_DRIVE_H
#define TABLE_DRIVE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <bitset>
#include <fstream>
#include <sstream> // convert int to string
#include "exceptions.h"
#define NAME_WIDTH 7 // output manipulation
#define OUT_DIRECTORY "D:/code_blocks/disassemble/qt.txt"
using namespace std;

class map_table;
typedef map<int, string> Labels;
Labels label_set;

typedef void (*Function_Ptr) (bin_instr& instr, map_table& mt);
typedef map<string, Function_Ptr> Function_Map;
typedef map<string, string> Register_map;
typedef map<string, pair<string, string> > Name_map; // also maps type
 // collision of ALU-R in opcode, also maps type
typedef map<string, pair<string, string> > ALU_map;

void decide_name(bin_instr& instr, map_table& mt);
void complement(bitset<16>& origin);
void write_instructions(char *open_path);
void process_ALU_R(bin_instr& instr, map_table& mt);
void process_shift(bin_instr& instr, map_table& mt);
void process_ALU_I(bin_instr& instr, map_table& mt);
void process_LUI(bin_instr& instr, map_table& mt);
void process_L_S(bin_instr& instr, map_table& mt);
void process_beq_bne(bin_instr& instr, map_table& mt);
void process_bz(bin_instr& instr, map_table& mt);
void process_JUMP(bin_instr& instr, map_table& mt);
void process_JALR(bin_instr& instr, map_table& mt);
void process_JR(bin_instr& instr, map_table& mt);

class map_table {
public:
    map_table() {}
    void init_all_maps();
    // build up all the key-value pairs in the three maps
    void main_process();
    // get all bin_instr.assemble_instr using Function_Map
    Register_map reg_table; // used by process_xxx() functions
    friend void decide_name(bin_instr& instr, map_table& mt);

private:
    void init_func_table();
    void init_reg_table();
    void init_name_table();
    void init_type_table();
    void init_ALU_map(); // called by init_name_table()
    Function_Map func_table;
    Function_Ptr p;
    Function_Map::iterator func_iter; // used to find key-value
    Name_map name_table;
    ALU_map ALU_table;
};

void decide_name(bin_instr& instr, map_table& mt) // also decide the type
{
    string opcode = instr.machine_instr.substr(0, 6);
    pair<string, string> name_type;
    name_type = mt.name_table[opcode]; // find by key-value pairs
    if(name_type.first == "ALU_R_jump") // collision
    {
        string func = instr.machine_instr.substr(26, 31); // the last 6 bits
        name_type = mt.ALU_table[func];
        instr.name = name_type.first;
        instr.type = name_type.second;
    }
    else if(name_type.first == "bltz_bgez") // collision, distinguish by rt
    {
        string rt = instr.machine_instr.substr(11, 5);
        if(rt == "00000")
            instr.name = "bltz";
        else if(rt == "00001")
            instr.name = "bgez";
        instr.type = name_type.second;
    }
    else // common cases
    {
        instr.name = name_type.first;
        instr.type = name_type.second;
    }
}

void map_table::init_all_maps()
{
    init_func_table();
    init_reg_table();
    init_name_table();
}

void map_table::init_func_table() // map by type, not by name
{
    func_table.insert(make_pair<string, Function_Ptr>("ALU_R",
        reinterpret_cast<Function_Ptr>(&process_ALU_R)));

    // special kinds of ALU-R, including sll, srl and sra
    func_table.insert(make_pair<string, Function_Ptr>("shift",
        reinterpret_cast<Function_Ptr>(&process_shift)));
    func_table.insert(make_pair<string, Function_Ptr>("ALU_I",
        reinterpret_cast<Function_Ptr>(&process_ALU_I)));
    func_table.insert(make_pair<string, Function_Ptr>("lui",
        reinterpret_cast<Function_Ptr>(&process_LUI))); // a special kind of ALU_I
    func_table.insert(make_pair<string, Function_Ptr>("L_S", // load and save
        reinterpret_cast<Function_Ptr>(&process_L_S)));
    func_table.insert(make_pair<string, Function_Ptr>("beq_bne",
        reinterpret_cast<Function_Ptr>(&process_beq_bne)));
    func_table.insert(make_pair<string, Function_Ptr>("bz",
        reinterpret_cast<Function_Ptr>(&process_bz)));
    func_table.insert(make_pair<string, Function_Ptr>("JUMP", // j and jal
        reinterpret_cast<Function_Ptr>(&process_JUMP)));
    func_table.insert(make_pair<string, Function_Ptr>("jalr",
        reinterpret_cast<Function_Ptr>(&process_JALR)));
    func_table.insert(make_pair<string, Function_Ptr>("jr",
        reinterpret_cast<Function_Ptr>(&process_JR)));
}

void map_table::init_reg_table()
{
    reg_table["00000"] = "$zero"; // 0
    reg_table["00001"] = "$at"; // 1 ================
    reg_table["00010"] = "$v0"; // 2
    reg_table["00011"] = "$v1"; // 3
    reg_table["00100"] = "$a0"; // 4
    reg_table["00101"] = "$a1"; // 5
    reg_table["00110"] = "$a2"; // 6
    reg_table["00111"] = "$a3"; // 7
    reg_table["01000"] = "$t0"; // 8
    reg_table["01001"] = "$t1"; // 9
    reg_table["01010"] = "$t2"; // 10
    reg_table["01011"] = "$t3"; // 11
    reg_table["01100"] = "$t4"; // 12
    reg_table["01101"] = "$t5"; // 13
    reg_table["01110"] = "$t6"; // 14
    reg_table["01111"] = "$t7"; // 15
    reg_table["10000"] = "$s0"; // 16
    reg_table["10001"] = "$s1"; // 17
    reg_table["10010"] = "$s2"; // 18
    reg_table["10011"] = "$s3"; // 19
    reg_table["10100"] = "$s4"; // 20
    reg_table["10101"] = "$s5"; // 21
    reg_table["10110"] = "$s6"; // 22
    reg_table["10111"] = "$s7"; // 23
    reg_table["11000"] = "$t8"; // 24
    reg_table["11001"] = "$t9"; // 25
    reg_table["11100"] = "$gp"; // 28
    reg_table["11101"] = "$sp"; // 29
    reg_table["11110"] = "$fp"; // 30
    reg_table["11111"] = "$ra"; // 31
}

// tell the name of an instruction by its opcode,
// but there are three kind of collision, they are:
// 1. all 13 ALU-R instructions,
// 2. bltz and bgez,
// 3. jalr and jr.
void map_table::init_name_table() // opcode
{
    name_table["000000"] = make_pair<string, string>("ALU_R_jump", "");
    // collision, use a secondary map

    name_table["001000"] = make_pair<string, string>("addi", "ALU_I");
    name_table["001001"] = make_pair<string, string>("addiu", "ALU_I");
    name_table["001100"] = make_pair<string, string>("andi", "ALU_I");
    name_table["001101"] = make_pair<string, string>("ori", "ALU_I");
    name_table["001110"] = make_pair<string, string>("xori", "ALU_I");
    name_table["001010"] = make_pair<string, string>("slti", "ALU_I");
    name_table["001011"] = make_pair<string, string>("sltiu", "ALU_I");
    name_table["001111"] = make_pair<string, string>("lui", "lui");
    // different from all other ALU_I

    name_table["100011"] = make_pair<string, string>("lw", "L_S");
    name_table["100000"] = make_pair<string, string>("lb", "L_S");
    name_table["100100"] = make_pair<string, string>("lbu", "L_S");
    name_table["100001"] = make_pair<string, string>("lh", "L_S");
    name_table["100101"] = make_pair<string, string>("lhu", "L_S");

    name_table["101011"] = make_pair<string, string>("sw", "L_S");
    name_table["101000"] = make_pair<string, string>("sb", "L_S");
    name_table["101001"] = make_pair<string, string>("sh", "L_S");

    name_table["000100"] = make_pair<string, string>("beq", "beq_bne");
    name_table["000101"] = make_pair<string, string>("bne", "beq_bne");
    name_table["000110"] = make_pair<string, string>("blez", "bz");
    name_table["000111"] = make_pair<string, string>("bgtz", "bz");
    name_table["000001"] = make_pair<string, string>("bltz_bgez", "bz");
    // collision, distinguish by rt

    name_table["000010"] = make_pair<string, string>("j", "JUMP");
    name_table["000011"] = make_pair<string, string>("jal", "JUMP");

    init_ALU_map();
}

void map_table::init_ALU_map()
{
    // ALU_table["010100"] = make_pair<string, string>("add", "ALU_R");
    // ALU_table["010110"] = make_pair<string, string>("sub", "ALU_R");
    // ALU_table["010101"] = make_pair<string, string>("addu", "ALU_R");
    // ALU_table["010111"] = make_pair<string, string>("subu", "ALU_R");
    ALU_table["100000"] = make_pair<string, string>("add", "ALU_R");
    ALU_table["100010"] = make_pair<string, string>("sub", "ALU_R");
    ALU_table["100001"] = make_pair<string, string>("addu", "ALU_R");
    ALU_table["100011"] = make_pair<string, string>("subu", "ALU_R");
    ALU_table["101010"] = make_pair<string, string>("slt", "ALU_R");
    ALU_table["101011"] = make_pair<string, string>("sltu", "ALU_R");
    ALU_table["100100"] = make_pair<string, string>("and", "ALU_R");
    ALU_table["100101"] = make_pair<string, string>("or", "ALU_R");
    ALU_table["100110"] = make_pair<string, string>("xor", "ALU_R");
    ALU_table["100111"] = make_pair<string, string>("nor", "ALU_R");
    ALU_table["000000"] = make_pair<string, string>("sll", "shift");
    ALU_table["000010"] = make_pair<string, string>("srl", "shift");
    ALU_table["000011"] = make_pair<string, string>("sra", "shift");

    // ===== the JUMP instructions are in the same collision as ALU-R
    ALU_table["001001"] = make_pair<string, string>("jalr", "jalr");
    ALU_table["001000"] = make_pair<string, string>("jr", "jr");
}

void map_table::main_process()
{
    vector<bin_instr>::iterator instr_iter = instr_set.begin();
    for(; instr_iter != instr_set.end(); instr_iter++)
    {
        decide_name(*instr_iter, *this); // decide name and type
        // cout << (*instr_iter).get_name() << endl;
        string instr_type = (*instr_iter).get_type();
        func_iter = func_table.find(instr_type); // find the function
        if(func_iter != func_table.end()) // found
        {
            // cout << (*instr_iter).get_machine_code() << endl;
            p = func_iter->second; // point to the function
            (*p)(*instr_iter, *this); // process
        }
        else
            throw Exception("Function not found.");
            // cout << "Function not found." << endl; // exception
    }
}

void complement(bitset<16>& origin)
{
    origin.flip(); // invert all bits
    for(int i = 0; i <= 15; i++)
    {
        if(!origin.test(i)) // test(): whether this bit is 1
        {
            origin.set(i); // 1, end
            break;
        }
        else
            origin.reset(i); // 0, generate a carry in next iteration
    }
}

void write_instructions(char *open_path) // write to a file
{
    ofstream fout(open_path); // previous: "D:/code_blocks/disassemble/qt.txt"
    // cout << open_path << endl;
    if(!fout)
        throw Exception("Failed to open the file.");
        // cout << "Failed to open the file.(write)" << endl;
    Instructions::iterator i_iter = instr_set.begin();
    Labels::iterator l_iter;
    for(; i_iter != instr_set.end(); i_iter++)
    {
        int addr = (*i_iter).get_addr();
        l_iter = label_set.find(addr);
        if(l_iter != label_set.end()) // found
            fout << l_iter->second << ":\n";
        fout << (*i_iter) << "\n";
    }
    fout.flush();
    fout.close();
}

void process_ALU_R(bin_instr& instr, map_table& mt)
{
    string rs, rt, rd; // according to the order in machine code
    string bin_32_code = instr.get_machine_code(); // machine code
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    rd = mt.reg_table[bin_32_code.substr(16, 5)];
    string Name = instr.get_name();
    int size = Name.size();
    string space(NAME_WIDTH - size, ' '); // output manipulation
    string statement = Name + space + rd + ",  " + rs + ",  " + rt + ";";
    instr.set_assem_instr(statement);
}

void process_shift(bin_instr& instr, map_table& mt)
{
    string rd, rt, shamt;
    string bin_32_code = instr.get_machine_code();
    rd = mt.reg_table[bin_32_code.substr(16, 5)];
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    shamt = bin_32_code.substr(21, 5);
    if(shamt[0] == '1')
        throw Exception("Invalid shift amount.");
        // cout << "Invalid shift amount."; // must not be negative
    // only this init method can convert string to bitset<>:
    bitset<6> bin_shamt(shamt);
    int int_shamt = static_cast<int>(bin_shamt.to_ulong());
    // convert int_shamt to an "int_string":
    stringstream ss;
    string str_shamt;
    ss << int_shamt;
    ss >> str_shamt;
    ss.flush();

    string statement = instr.get_name() + "    " + rd + ",  "
                        + rt + ",  " + str_shamt + ";";
    instr.set_assem_instr(statement);
}

void process_ALU_I(bin_instr& instr, map_table& mt)
{
    string rs, rt, imme; // according to the order in machine code
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    imme = bin_32_code.substr(16, 16);
    bitset<16> bin_imme(imme);
    int int_imme;
    if(imme[0] == '1') // negative
    {
        complement(bin_imme); // get the complement of the complement
        int_imme = static_cast<int>(bin_imme.to_ulong());
        int_imme = -int_imme;
    }
    else // positive
        int_imme = static_cast<int>(bin_imme.to_ulong());
    stringstream ss;
    string str_imme;
    ss << int_imme;
    ss >> str_imme;
    ss.flush();

    string Name = instr.get_name();
    int size = Name.size();
    string space(NAME_WIDTH - size, ' ');

    string statement = Name + space + rt + ",  " + rs + ",  " + str_imme + ";";
    instr.set_assem_instr(statement);
}

void process_LUI(bin_instr& instr, map_table& mt)
{
    string rt, imme;
    string bin_32_code = instr.get_machine_code();
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    imme = bin_32_code.substr(16, 16);
    bitset<16> bin_imme(imme);
    int int_imme;
    if(imme[0] == '1') // negative
    {
        complement(bin_imme); // get the complement of the complement
        int_imme = static_cast<int>(bin_imme.to_ulong());
        int_imme = -int_imme;
    }
    else // positive
        int_imme = static_cast<int>(bin_imme.to_ulong());
    stringstream ss;
    string str_imme;
    ss << int_imme;
    ss >> str_imme;
    ss.flush();

    string statement = instr.get_name() + "    " + rt + ",  " + str_imme + ";";
    instr.set_assem_instr(statement);
}

void process_L_S(bin_instr& instr, map_table& mt)
{
    string rs, rt, offset; // offset will never be negative ?
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    offset = bin_32_code.substr(16, 16);
    bitset<16> bin_offset(offset);
    int int_offset = static_cast<int>(bin_offset.to_ulong());
    stringstream ss;
    string str_offset;
    ss << int_offset;
    ss >> str_offset;
    ss.flush();

    string Name = instr.get_name();
    int size = Name.size();
    string space(NAME_WIDTH - size, ' ');

    string statement = Name + space + rt + ",  " + str_offset + "(" + rs + ")" + ";";
    instr.set_assem_instr(statement);
}

string index = "00";
string index_increment()
{
    if(index[1] == '9')
    {
        index[1] = '0';
        index[0]++;
    }
    else
        index[1]++;
    return index;
}
// ===== how to print labels:
// keep a global variable label_count to count how many labels have been used
// print out "Label" + label_count.to_string() + ":".
// store the labels and the relative addr calculated first,
// use lable_set when printing out all assemble instructions.
void process_beq_bne(bin_instr& instr, map_table& mt)
{
    string rs, rt, offset;
    string label("Label_");
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    rt = mt.reg_table[bin_32_code.substr(11, 5)];
    offset = bin_32_code.substr(16, 16);
    bitset<16> bin_offset(offset);
    int int_offset;
    if(offset[0] == '1') // negative
    {
        complement(bin_offset);
        int_offset = static_cast<int>(bin_offset.to_ulong());
        int_offset = -int_offset;
    }
    else // positive
        int_offset = static_cast<int>(bin_offset.to_ulong());
    // calculate the place to put the label:
    int label_addr = instr.get_addr() + 1 + int_offset;
    label += index_increment();
    label_set[label_addr] = label;
    string statement = instr.get_name() + "    " + rs + ",  " + rt + ",  " + label + ";";
    instr.set_assem_instr(statement);
}

// apply to blez, bgtz, bltz, bgez
void process_bz(bin_instr& instr, map_table& mt)
{ // blez $rs, Label;
    string rs;
    string label("Label_");
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];

    string offset = bin_32_code.substr(16, 16);
    bitset<16> bin_offset(offset);
    int int_offset;
    if(offset[0] == '1') // negative
    {
        complement(bin_offset);
        int_offset = static_cast<int>(bin_offset.to_ulong());
        int_offset = -int_offset;
    }
    else // positive
        int_offset = static_cast<int>(bin_offset.to_ulong());
    int label_addr = instr.get_addr() + 1 + int_offset;
    label += index_increment();
    label_set[label_addr] = label;
    string statement = instr.get_name() + "   " + rs + ",  " + label + ";";
    instr.set_assem_instr(statement);
}

void process_JUMP(bin_instr& instr, map_table& mt)
{
    string target;
    string bin_32_code = instr.get_machine_code();
    target = bin_32_code.substr(6, 26); // must be positive
    bitset<26> bin_target(target);
    int int_target = static_cast<int>(bin_target.to_ulong());

    string label("Label_");
    label += index_increment();
    label_set[int_target] = label;

    string space;
    string Name = instr.get_name();
    if(Name.size() == 1) // j
        space = "      ";
    else // jal
        space = "    ";

    string statement = Name + space + label + ";";
    instr.set_assem_instr(statement);
}

void process_JALR(bin_instr& instr, map_table& mt)
{
    string rs, rd;
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    rd = mt.reg_table[bin_32_code.substr(16, 5)];
    string statement = instr.get_name() + "   " + rs + ",  " + rd + ";";
    instr.set_assem_instr(statement);
}

void process_JR(bin_instr& instr, map_table& mt)
{
    string rs;
    string bin_32_code = instr.get_machine_code();
    rs = mt.reg_table[bin_32_code.substr(6, 5)];
    string statement = instr.get_name() + "     " + rs + ";";
    instr.set_assem_instr(statement);
}

#endif // TABLE_DRIVE_H
