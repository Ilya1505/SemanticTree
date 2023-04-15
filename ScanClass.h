#pragma once
#include <iostream>
#include <string>
#include <map>

#define tId 1
#define tBool 2
#define tDouble 3
#define tVoid 4
#define tFor 5
#define tReturn 6
#define tMain 7
#define tClass 8
#define tTrue 9
#define tFalse 10
#define constInt 11
#define tToch 12
#define tZpt 13
#define tTzpt 14
#define tLs 15
#define tPs 16
#define tFls 17
#define tFps 18
#define tEq 19
#define tNeq 20
#define tOr 21
#define tAnd 22
#define tRightShift 23
#define tLeftShift 24
#define tPlus 25
#define tMinus 26
#define tMult 27
#define tDiv 28
#define tBitOr 29
#define tBitAnd 30
#define tNot 29
#define tSave 46
#define tEnd 100
#define tError 200



using namespace std;
class Scan_class
{
private:
	string text;
	int ptr;
	int line;
	

	string keyWords[9];
	int keyWordCode[9];
public:
	void Scaner(int &codeLex, string &stringLex);
	void ReadFile(std::string fileName);
	Scan_class();
	int GetLine();
	int GetPoint();
	void SetLine(int newLine);
	void SetPoint(int newpoint);
};