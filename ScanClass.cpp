#include "stdafx.h"
#include "ScanClass.h"


Scan_class::Scan_class()
{
	ptr = 0;
	line = 1;
	keyWords[0] = "bool";
	keyWords[1] = "double";
	keyWords[2] = "void";
	keyWords[3] = "for";
	keyWords[4] = "return";
	keyWords[5] = "main";
	keyWords[6] = "class";
	keyWords[7] = "true";
	keyWords[8] = "false";

	keyWordCode[0] = tBool;
	keyWordCode[1] = tDouble;
	keyWordCode[2] = tVoid;
	keyWordCode[3] = tFor;
	keyWordCode[4] = tReturn;
	keyWordCode[5] = tMain;
	keyWordCode[6] = tClass;
	keyWordCode[7] = tTrue;
	keyWordCode[8] = tFalse;
}

void Scan_class::ReadFile(std::string fileName) {
	string textInfo;
	FILE* file;
	fopen_s(&file, fileName.c_str(), "r");// открытие файла

	if (file == nullptr) {
		std::cout << "File is not open" << std::endl;
		system("pause");
		std::exit(0);
	}
	int size = 0;
	// чтение данных из файла
	while (!feof(file)) {
		char c;
		fscanf_s(file, "%c", &c);
		//fread(&c, sizeof(char), 1, file);
		textInfo.append(std::string(1, c));
	}

	textInfo.append("\0");
	textInfo.erase(textInfo.length() - 1);
	fclose(file);   // закрытие файла

	this->text = textInfo;
}

int Scan_class::GetLine() {
	return line;
}

int Scan_class::GetPoint() {
	return ptr;
}

void Scan_class::SetLine(int newLine) {
	line = newLine;
}

void Scan_class::SetPoint(int newpoint) {
	ptr = newpoint;
}

void Scan_class::Scaner(int &codeLex, string &stringLex)
{
	while (true) {
		stringLex = "";
		// пропуск пробелов и символов перехода на другую строку
		while ((text[ptr] == ' ') || (text[ptr] == '\n') || (text[ptr] == '\t')) {
			if (text[ptr] == '\n') line++;
			ptr++;
		}
		// проверка на однострочные комментарии
		if ((text[ptr] == '/') && (text[ptr + 1] == '/')) {
			ptr += 2;
			while (text[ptr] != '\n' && text[ptr] != '\0')
				ptr++;
		}//проверка на многострочные комментарии
		else if ((text[ptr] == '/') && (text[ptr + 1] == '*')) {
			ptr += 2;
			while ((text[ptr + 1] != '/') || (text[ptr] != '*')) {
				if (text[ptr] == '\n') line++;
				ptr++;
				if (text[ptr + 1] == '\0') {
					codeLex = tEnd;
					stringLex = "End";
					return;
				}
			}
			ptr += 2;
		}
		else if (text[ptr] == '\0'){
			codeLex = tEnd;
			stringLex = "End";
			return;
		}
		else
		{// проверка на константу
			if ((text[ptr] <= '9') && (text[ptr] >= '1')) {
				stringLex.append(string(1, text[ptr++]));
				while ((text[ptr] <= '9') && (text[ptr] >= '0'))
					stringLex.append(string(1, text[ptr++]));
				codeLex = constInt;
				return;
			}// проверка что константа равна 0
			else if (text[ptr] == '0' && (text[ptr + 1] <= '9'&&text[ptr + 1] >= '0')) {
				while ((text[ptr + 1] <= '9') && (text[ptr + 1] >= '0'))
					stringLex.append(string(1, text[ptr++]));
			}
			else if (text[ptr] == '0' && (text[ptr + 1] > '9' || text[ptr + 1] < '0')){
				stringLex.append(string(1, text[ptr++]));
				codeLex = constInt;
				return;
			}
			//проверка на ключевое слово или идентификатор
			else if (((text[ptr] >= 'a') && (text[ptr] <= 'z')) || text[ptr] == '_' || ((text[ptr] >= 'A') && (text[ptr] <= 'Z'))) {
				stringLex.append(string(1, text[ptr++]));
				while ((text[ptr] <= '9') && (text[ptr] >= '0') ||
					((text[ptr] >= 'a') && (text[ptr] <= 'z')) || text[ptr] == '_' || ((text[ptr] >= 'A')
					&& (text[ptr] <= 'Z'))) stringLex.append(string(1, text[ptr++]));
				for (size_t i = 0; i < 9; i++) {
					if (keyWords[i] == stringLex)
					{
						codeLex = keyWordCode[i];
						return;
					}
				}
				codeLex = tId;
				return;
			}
			else if (text[ptr] == ',') {
				stringLex.append(string(1, text[ptr++]));
				codeLex = tZpt;
				return;
			}
			else if (text[ptr] == '.') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tToch;
				return;
			}
			else if (text[ptr] == ';') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tTzpt;
				return;
			}
			else if (text[ptr] == '(') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tLs;
				return;
			}
			else if (text[ptr] == ')') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tPs;
				return;
			}
			else if (text[ptr] == '{') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tFls;
				return;
			}
			else if (text[ptr] == '}') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tFps;
				return;
			}
			else if (text[ptr] == '|') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tBitOr;
				if (text[ptr] == '|') {
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tOr;
					return;
				}
				else return;
			}
			else if (text[ptr] == '&') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tBitAnd;
				if (text[ptr] == '&') {
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tAnd;
					return;
				}
				else return;
			}
			else if (text[ptr] == '+') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tPlus;
				return;
			}
			else if (text[ptr] == '-') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tMinus;
				return;
			}
			else if (text[ptr] == '*') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tMult;
				return;
			}
			else if (text[ptr] == '/') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tDiv;
				return;
			}
			else if (text[ptr] == '!') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tNot;
				if (text[ptr] == '=') {
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tNeq;
					return;
				}
				else return;
			}
			else if (text[ptr] == '=') {
				stringLex.append(std::string(1, text[ptr++]));
				codeLex = tSave;
				if (text[ptr] == '=') {
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tEq;
					return;
				}
				else return;
			}
			else if (text[ptr] == '<') {

				if (text[ptr + 1] == '<') {
					stringLex.append(std::string(1, text[ptr++]));
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tLeftShift;
					return;
				}
			}
			else if (text[ptr] == '>') {

				if (text[ptr + 1] == '>') {
					stringLex.append(std::string(1, text[ptr++]));
					stringLex.append(std::string(1, text[ptr++]));
					codeLex = tRightShift;
					return;
				}
			}
			stringLex.append(std::string(1, text[ptr++]));
			cout << endl << "Ошибка в строке: " << line << endl;
			cout << "Неккорреткный символ: " << stringLex << endl;
			codeLex = tError;
			system("pause");
			exit(1);
		}
	}
}