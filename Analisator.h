#pragma once
#include "ScanClass.h"
#include "SemanticTree.h"

enum TypeVar {
	TBool = 11,
	TDouble,
	TInt,
	TypeDef = 100
};

class Analisator
{
	Scan_class scaner;
	int currentType;
	int savePoint;
	int saveLine;
	std::string currentLex;
	SemanticTree* root = new SemanticTree(NULL, NULL, NULL, new Node);
	std::string currentClass;

	void ShowError(std::string message);
	void AnaliseSosOp();
	void AnaliseData();
	int AnaliseExpression();
	int AnaliseAnd();
	int AnaliseBit();
	int AnaliseMove();
	int AnaliseAdd();
	int AnaliseMultiply();
	int AnaliseNe();
	int AnaliseBasicExpress();
	void AnaliseAskClassFunc();
	void AnaliseClass();
	void AnaliseClassFunc();
	void AnaliseOpAndData();
	void AnaliseFunc();
	void AnaliseFor();
	void AnaliseSave();

public:
	Analisator(std::string fileName);
	void Prog();
};