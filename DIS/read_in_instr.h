#ifndef READ_IN_INSTR_H
#define READ_IN_INSTR_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <map>
#include "exceptions.h"
#define COE_DIRECTORY "D://code_blocks/disassemble/single.coe"
// "D:/code_blocks/assembler/output.txt" from Qt
#define BASE_ADDR 0 // if the memory has both data area and text (instr) area, it may be changed
using namespace std;

typedef vector<bin_instr> Instructions;
typedef map<char, string> Hex_map;

// two global variables, will be passed to table_drive.h after filled
// Labels label_set;
Instructions instr_set;
Hex_map hex_table; // hexadecimal to binary

string trim(string& s);
string remove_space(string& s);
void hex_table_init();
string hex_to_bin(string& instr);
string pre_process(ifstream& fin);
void get_instructions(char *open_path); // ===== MAIN PROCESS

string trim(string& s) // reomve all the space and tabs at front or back
{
    if(s.empty())
        return s;
    string::iterator s_iter;
    int first_valid = s.find_first_not_of(" \t");
    if(first_valid == -1)
        return s.erase();
    s_iter = s.begin() + first_valid;
    s.erase(s.begin(), s_iter); // erase all spaces or tabs before the string
    s_iter = s.begin() + s.find_last_not_of(" \t");
    s.erase(s_iter + 1, s.end()); // erase all spaces or tabs after the string
    return s;
}

string remove_space(string& s) // remove all space and tab in the middle
{
    if(s.empty())
        return s;
    string::iterator s_iter = s.begin();
    for(; s_iter != s.end(); s_iter++)
    {
        if(*s_iter == ' ' || *s_iter == '\t')
            s.erase(s_iter--);
    }
    return s;
}

void hex_table_init()
{
    hex_table['0'] = "0000";
    hex_table['1'] = "0001";
    hex_table['2'] = "0010";
    hex_table['3'] = "0011";
    hex_table['4'] = "0100";
    hex_table['5'] = "0101";
    hex_table['6'] = "0110";
    hex_table['7'] = "0111";
    hex_table['8'] = "1000";
    hex_table['9'] = "1001";
    hex_table['a'] = "1010";  hex_table['A'] = "1010";
    hex_table['b'] = "1011";  hex_table['B'] = "1011";
    hex_table['c'] = "1100";  hex_table['C'] = "1100";
    hex_table['d'] = "1101";  hex_table['D'] = "1101";
    hex_table['e'] = "1110";  hex_table['E'] = "1110";
    hex_table['f'] = "1111";  hex_table['F'] = "1111";
}

string hex_to_bin(string& instr)
{
    string rtn; // return to it
    if(instr.empty())
        throw Exception("hex_to_bin() Syntax error.");
        // cout << "hex_to_bin() Syntax error." << endl;
    string::iterator s_iter = instr.begin();
    for(; s_iter != instr.end(); s_iter++) // 8 iterations
        rtn += hex_table[*s_iter];
    // cout << rtn << endl;
    return rtn;
}

int RADIX;
// usually there will be two statements at the beginning of the file:
// memory_initialization_radix=2;
// memory_initialization_vector=...;
string pre_process(ifstream& fin) // before reading in instructions. get the radix (2 or 16)
{ // remove the text before real instructions
    string row_text;

    // radix declaration is required, or there is an error
    getline(fin, row_text);
    trim(row_text);
    remove_space(row_text);
    int radix_start = row_text.find_first_of('=');
    if(radix_start == -1)
        throw Exception("No declaration.");
        // cout << "no declaration. Syntax error." << endl; // no radix_declare, exception
    // get radix:
    int radix_end = row_text.find_first_of(';');
    if(radix_end == -1)
        throw Exception("Syntax error: Absence of ';' at the end.");
        // cout << "cannot find first ';'. Syntax error." << endl; // exception
    if(radix_end == radix_start + 2 && row_text[radix_start + 1] == '2')
        RADIX = 2;
    else if(radix_end == radix_start + 3 && row_text[radix_start + 1] == '1'
    && row_text[radix_start + 2] == '6')
        RADIX = 16;
    else
        throw Exception("Invalid radix.");
        // cout << "in pre_process(). Invalid radix." << endl; // exception
    row_text.erase(0, radix_end + 1); // erase the front part

    if(RADIX == 16)
        hex_table_init();

    // then find the second '=':
    if(row_text.empty()) // the second '=' is at the second line
    {
        getline(fin, row_text);
        trim(row_text);
        remove_space(row_text);
    } // statements in if will not be executed if the second '=' is at the first line
    int instr_start = row_text.find_first_of('=');
    row_text.erase(0, instr_start + 1);
    if(row_text.empty()) // the real instructions are at the next line
    {
        getline(fin, row_text);
        trim(row_text);
        remove_space(row_text);
        return row_text;
    }
    else // real instructions are at the same line
        return row_text;
} // return to a string which begins with the first machine code

void get_instructions(char *open_path) // fill in instr_set[] ========= pass open_path in
{
    int current_addr = 0; // for calculating the relative address
    string raw_instr; // to be split
    string single_instr; // after split
    ifstream fin(open_path); // ======== previous: COE_DIRECTORY
    if(!fin)
        throw Exception("Failed to open the file.");
        // cout << "Failed to open the file." << endl; // exception
    raw_instr = pre_process(fin); // fin has been modified
    // cout << "pre_process done." << endl;
    do {
        trim(raw_instr);
        remove_space(raw_instr);
        while(!raw_instr.empty())
        {
            int end = raw_instr.find_first_of(',');
            if(end == -1)
                end = raw_instr.find_first_of(';'); // the last instr
            if(end == -1)
                throw Exception("Syntax error: Absence of ';' at the end.");
                // cout << "cannot find last ';'. Syntax error." << endl; // exception
            single_instr = raw_instr.substr(0, end);
            if(RADIX == 16) // hexadecimal to binary
                single_instr = hex_to_bin(single_instr);
            instr_set.push_back(bin_instr(single_instr, current_addr++));
            raw_instr.erase(0, end + 1);
        }
    } while(getline(fin, raw_instr));
    // Instructions::iterator instr_iter = instr_set.begin();
    // for(; instr_iter != instr_set.end(); instr_iter++)
        // (*instr_iter).show();
}

#endif // READ_IN_INSTR_H
