// ���������� ��� ������������� � GCC, ��� �� ���������� safe-������ �������
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstring>
#include "asm_x86.h"
#include "asm_x64.h"

using namespace std;

// ����������� �������������� ������
enum Arch
{
    X86,    // 32-������
    X64     // 64-������
};

// �������� ����, ��� ������ ���������� � ���������� ��������
bool startswith(const string& line, const string& prefix)
{
    if (line.size() < prefix.size())
        return false;
    for (int i = 0; i < prefix.size(); ++i)
        if (line[i] != prefix[i])
            return false;
    return true;
}

// �������� �� ������ ������ ������� �� ������ �����������
bool isFuncName(const string& line)
{
#ifdef __unix__
    return isalpha(line[0]) || line[0] == '_';
#else
    return line[0] == '_';
#endif
}

// �������� �� ��� ������� main-�� �� ������ �����������
bool isMainName(const string& name)
{
#ifdef __unix__
    return name == "main";
#else
    return name == "_main";
#endif
}

// �������� ����� malloc() ��� ������ �����������
inline const char* getMallocCall(bool arch)
{
#ifdef __unix__
    if (arch)
        return UNIX_MALLOC_x64;
    else
        return UNIX_MALLOC_x86;
#else
    return WIN_MALLOC;
#endif
}

// ��������� ��������� �� ��������� ������
// ���������� true, ���� ��������� ���������, false � ��������� ������
bool parseArgs(int argc, char** argv, vector<string>& inFiles, int& stackSize, Arch& arch)
{
    // ��� ���������� ���������
    enum ArgType
    {
        IN_FILE,        // ��� �������� ����� ��� ��� �����
        STACK_SIZE,     // ������ �����
    } next = IN_FILE;

    for (int i = 1; i < argc; ++i)
    {
        if (next == STACK_SIZE)
        {
            char* numberEnd;
            stackSize = strtol(argv[i], &numberEnd, 10);
            if ((numberEnd - argv[i]) < strlen(argv[i]))
                return false;
            next = IN_FILE;
        }
        else if (!strcmp(argv[i], "-s"))
            next = STACK_SIZE;
        else if (!strcmp(argv[i], "-x86"))
            arch = X86;
        else if (!strcmp(argv[i], "-x64"))
            arch = X64;
        else
            inFiles.push_back(argv[i]);
    }
    return true;
}

// ��������� ��������� ����� �� files � ������ fileLines � ��������� ��� ����� ������� � funcs
void parseFiles(const vector<string>& files,
                unordered_set<string>& funcs,
                vector<vector<string>>& fileLines)
{
    for (const string& file : files)
    {
        vector<string> lines;
        ifstream fin(file);
        string line;
        while (getline(fin, line))
        {
            lines.push_back(line);
            if (isFuncName(line))
            {
                // ��� ������ - ��� �������
                line.pop_back();
                funcs.insert(line);
            }
        }
        fin.close();
        fileLines.push_back(lines);
    }
}

// ������������ ������ ������ �� fileLines � ���������� �������� ����� ������� � ������������
void processFiles(const vector<string> &files,
                const vector<vector<string>>& fileLines,
                const unordered_set<string>& funcs,
                int stackSize, Arch& arch)
{
    // �������, ������� ���� � .s �����, �� ������������ ������������. � ������ �� ���������.
    unordered_set<string> builtinFuncs;
#ifdef __unix__
    // ��� x86
    builtinFuncs.insert("__x86.get_pc_thunk.ax");
    builtinFuncs.insert("__x86.get_pc_thunk.bx");
    builtinFuncs.insert("__x86.get_pc_thunk.dx");
    builtinFuncs.insert("__x86.get_pc_thunk.bp");
#else
    // � MinGW ����� ������� ���
#endif

    bool insertedSaveRet = false;
    for (int i = 0; i < fileLines.size(); ++i)
    {
        ofstream fout(files[i]);
        string currFunc = "";
        bool wasNoTabLine = true; // ����� ������� - ������ ������ ��� \t � ������ ����� ��
        for (string line : fileLines[i])
        {
            if (!insertedSaveRet && line[0] != '\t')
            {
                // ������� save_ret ������� ��������� ����� ������ �������� ������� �����
                fout << (arch == X64 ? SAVE_RET_x64 : SAVE_RET_x86);
                insertedSaveRet = true;
            }
            if (isFuncName(line))
            {
                // ������ �������
                currFunc = line;
                currFunc.pop_back();
                wasNoTabLine = false;
            }
            else if (line[0] != '\t')
            {
                if (wasNoTabLine)
                {
                    // ������� �����������, �� ����� �� ��������
                    currFunc = "";
                }
                else
                {
                    // ��� ������ ����� ����� �������, ��� �������� � ������
                    wasNoTabLine = true;
                    if (isMainName(currFunc))
                    {
                        // � ������ main ���������� ��������� �������������
                        char inserted[256];
                        sprintf(inserted, arch == X64 ? INIT_x64 : INIT_x86, stackSize, getMallocCall(arch));
                        fout << line << endl << inserted;
                        continue;
                    }
                }
            }
            else if (startswith(line, "\tcall"))
            {
                // ����� ������� (���������� call)
                string funcName = line.substr(sizeof("\tcall\t") - 1);
                if (funcs.count(funcName) && !builtinFuncs.count(funcName))
                {
                    fout << (arch == X64 ? CALL_x64 : CALL_x86) << funcName << endl;
                    continue;
                }
            }
            else if (startswith(line, "\tret"))
            {
                // ���������� ret
                if (funcs.count(currFunc) && !builtinFuncs.count(currFunc) && !isMainName(currFunc))
                {
                    fout << (arch == X64 ? RET_x64 : RET_x86);
                    continue;
                }
            }
            fout << line << endl;
        }
        fout.close();
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        cout << "Error: No input files\n";
        return 1;
    }

    vector<string> inFiles;
    int stackSize = 0x10000;
    Arch arch = X64;

    if (!parseArgs(argc, argv, inFiles, stackSize, arch))
    {
        cout << "Error: Incorrect argument\n";
        return 2;
    }

    if (inFiles.empty())
    {
        cout << "Error: No input files\n";
        return 1;
    }

    unordered_set<string> funcs;
    vector<vector<string>> fileLines;
    parseFiles(inFiles, funcs, fileLines);
    processFiles(inFiles, fileLines, funcs, stackSize, arch);
    
    return 0;
}