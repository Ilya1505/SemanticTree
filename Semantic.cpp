// Semantic.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <Conio.h>
#include <Locale.h>
#include "Analisator.h"
#include <Windows.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Analisator analisator("date.txt");
	analisator.Prog();
	system("pause");
	return 0;
}

