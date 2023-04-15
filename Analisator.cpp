#include "stdafx.h"
#include "Analisator.h"

Analisator::Analisator(std::string fileName) {
	scaner.ReadFile(fileName);
	currentType = 0;
	savePoint = 0;
	saveLine = 0;
	root->SetCurrent(root);
	root->Add(new std::string("double"), TType);
	root->Add(new std::string("bool"), TType);
	root->Add(new std::string("int"), TType);
	root->Add(new std::string("void"), TType);
}

void Analisator::Prog() {
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType != tEnd) {
		if (currentType == tBool || currentType == tDouble) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseData();
		}
		else if (currentType == tClass) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseClass();
		}
		else if (currentType == tVoid) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseFunc();
		}
		else ShowError("Not data, class or function");
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	//root->Print();
	std::cout << "No errors" << std::endl;

}

void Analisator::AnaliseData() {
	std::string type;
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType == tId) {
		root->Cur->CheckClass(currentLex, true);
		type = currentLex;
		do {
			scaner.Scaner(currentType, currentLex);
			if (currentType != tId) ShowError("Not id");
			root->Cur->CheckIfVarNotExist(currentLex);
			root->Add(&currentLex, TVar)->SetTypeVar(&type);
			scaner.Scaner(currentType, currentLex);
		} while (currentType == tZpt);
	}
	else if (currentType == tBool || currentType == tDouble) {
		type = currentLex;
		do {
			scaner.Scaner(currentType, currentLex);
			if (currentType != tId) ShowError("Not id");
			root->Cur->CheckIfVarNotExist(currentLex);
			root->Add(&currentLex, TVar)->SetTypeVar(&type);
			scaner.Scaner(currentType, currentLex);
			if (currentType == tSave) {
				AnaliseExpression();
				scaner.Scaner(currentType, currentLex);
			}
		} while (currentType == tZpt);
	}
	else ShowError("Not type");
	if (currentType != tTzpt) ShowError("Not ;");
}

int Analisator::AnaliseExpression() {
	int type1, type2;
	type1 = AnaliseAnd();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tOr) {
		type2 = AnaliseAnd();
		type1 = TBool;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseAnd() {
	int type1, type2;
	type1 = AnaliseBit();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tAnd) {
		type2 = AnaliseBit();
		type1 = TBool;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseBit() {
	int type1, type2, curType;
	type1 = AnaliseMove();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tBitOr || currentType == tBitAnd || currentType == tEq || currentType == tNeq) {
		curType = currentType;
		type2 = AnaliseMove();
		if ((type1 == TDouble || type2 == TDouble) && (curType == tBitOr || curType == tBitAnd)) ShowError("Not allowed type");
		else if (curType == tBitOr || curType == tBitAnd) type1 = TInt;
		else type1 = TBool;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseMove() {
	int type1, type2;
	type1 = AnaliseAdd();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tLeftShift || currentType == tRightShift) {
		type2 = AnaliseAdd();
		if (type1 == TDouble || type2 == TDouble) ShowError("Not allowed type");
		else type1 = TInt;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseAdd() {
	int type1, type2;
	type1 = AnaliseMultiply();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tPlus || currentType == tMinus) {
		type2 = AnaliseMultiply();
		if (type1 == TDouble || type2 == TDouble) type1 = TDouble;
		else type1 = TInt;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseMultiply() {
	int type1, type2;
	type1 = AnaliseNe();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType == tMult || currentType == tDiv) {
		type2 = AnaliseNe();
		if (type1 == TDouble || type2 == TDouble) type1 = TDouble;
		else type1 = TInt;
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	scaner.SetPoint(savePoint);
	scaner.SetLine(saveLine);
	return type1;
}

int Analisator::AnaliseNe() {
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tNot) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
	}
	return AnaliseBasicExpress();
}

int Analisator::AnaliseBasicExpress() {
	int type = TypeDef;
	SemanticTree* clas;
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType == tLs) {
		type = AnaliseExpression();
		scaner.Scaner(currentType, currentLex);
		if (currentType != tPs) ShowError("Not )");
	}
	else if (currentType == tId) {
		clas = root->Cur->CheckIfVarExist(currentLex);
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
		while (currentType == tToch) {
			scaner.Scaner(currentType, currentLex);
			if (currentType != tId) ShowError("Not id");
			clas = clas->CheckClassVar(currentLex);
			savePoint = scaner.GetPoint();
			saveLine = scaner.GetLine();
			scaner.Scaner(currentType, currentLex);
		}
		if (clas->GetId() == "bool") type = TBool;
		else if (clas->GetId() == "double") type = TDouble;
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
	}
	else if (currentType == constInt) type = TInt;
	else if (currentType == tTrue || currentType == tFalse) type = TBool;
	else ShowError("Not basic expression");
	if (type == TypeDef) ShowError("Not allow type");
	return type;
}

void Analisator::AnaliseClass() {
	SemanticTree* savedPos;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tClass) ShowError("Not class");
	scaner.Scaner(currentType, currentLex);
	if (currentType != tId) ShowError("Not id");
	root->Cur->CheckClass(currentLex, false);
	root->Add(&currentLex, TType);
	currentClass = currentLex;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tFls) ShowError("Not {");
	savedPos = root->NewBlock();
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType != tFps) {
		if (currentType == tBool || currentType == tDouble || currentType == tId) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseData();
		}
		else if (currentType == tVoid) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseClassFunc();
		}
		else ShowError("Not data or method or }");
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
	root->SetCurrent(savedPos);
}

void Analisator::AnaliseClassFunc() {
	SemanticTree* ThisFunc;
	SemanticTree* savedPos;
	std::string curType;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tVoid) ShowError("Not type");
	scaner.Scaner(currentType, currentLex);
	if (currentType != tId) ShowError("Not id");
	ThisFunc = root->Add(&currentLex, TFunc);
	ThisFunc->SetTypeVar(new std::string("void"));
	ThisFunc->SetParentClass(&currentClass);
	savedPos = root->NewBlock();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tLs) ShowError("Not (");
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType == tBool || currentType == tDouble || currentType == tId) {
		if (currentType == tId) root->Cur->CheckClass(currentLex, true);
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		do {
			scaner.Scaner(currentType, currentLex);
			if (currentType != tBool && currentType != tDouble && currentType != tId) ShowError("Not type");
			if (currentType == tId) root->Cur->CheckClass(currentLex, true);
			curType = currentLex;
			scaner.Scaner(currentType, currentLex);
			if (currentType != tId) ShowError("Not id");
			root->Add(&currentLex, TVar)->SetTypeVar(&curType);
			ThisFunc->AddToFunc();
			scaner.Scaner(currentType, currentLex);
		} while (currentType == tZpt);
	}
	if (currentType != tPs) ShowError("Not )");
	ThisFunc->Up->CheckFunc(ThisFunc->GetId(), ThisFunc->GetParam(), false);
	AnaliseSosOp();
	root->SetCurrent(savedPos);
}

void Analisator::AnaliseSosOp() {
	scaner.Scaner(currentType, currentLex);
	if (currentType != tFls) ShowError("Not {");
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	while (currentType != tFps) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		AnaliseOpAndData();
		savePoint = scaner.GetPoint();
		saveLine = scaner.GetLine();
		scaner.Scaner(currentType, currentLex);
	}
}

void Analisator::AnaliseOpAndData() {
	SemanticTree* savedPos;
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType == tBool || currentType == tDouble) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		AnaliseData();
	}
	else if (currentType == tId) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		do {
			scaner.Scaner(currentType, currentLex);
			if (currentType != tId) ShowError("Not id");
			scaner.Scaner(currentType, currentLex);
		} while (currentType == tToch);
		if (currentType == tLs) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseAskClassFunc();
			scaner.Scaner(currentType, currentLex);
			if (currentType != tTzpt) ShowError("Not ;");
		}
		else if (currentType == tSave) {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseSave();
			scaner.Scaner(currentType, currentLex);
			if (currentType != tTzpt) ShowError("Not ;");
		}
		else {
			scaner.SetPoint(savePoint);
			scaner.SetLine(saveLine);
			AnaliseData();
		}
	}
	else if (currentType == tFls) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		savedPos = root->NewBlock();
		AnaliseSosOp();
		root->SetCurrent(savedPos);
	}
	else if (currentType == tFor) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		savedPos = root->NewBlock();
		AnaliseFor();
		root->SetCurrent(savedPos);
	}
	else if (currentType != tTzpt) ShowError("Not operator or data");
}

void Analisator::AnaliseAskClassFunc() {
	SemanticTree* clas;
	std::string savId = "";
	int par = 0;
	bool f = false;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tId) ShowError("Not id");
	savId = currentLex;
	scaner.Scaner(currentType, currentLex);
	if (currentType == tToch) {
		clas = root->Cur->CheckIfVarExist(savId);
		f = true;
	}
	while (currentType == tToch) {
		scaner.Scaner(currentType, currentLex);
		if (currentType != tId) ShowError("Not id");
		savId = currentLex;
		scaner.Scaner(currentType, currentLex);
		if (currentType == tToch) {
			clas = clas->CheckClassVar(savId);
		}
	}
	if (currentType != tLs) ShowError("Not (");
	savePoint = scaner.GetPoint();
	saveLine = scaner.GetLine();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tPs) {
		scaner.SetPoint(savePoint);
		scaner.SetLine(saveLine);
		do {
			AnaliseExpression();
			par++;
			savePoint = scaner.GetPoint();
			saveLine = scaner.GetLine();
			scaner.Scaner(currentType, currentLex);
		} while (currentType == tZpt);
		if (currentType != tPs) ShowError("Not )");
	}
	if (f) clas->CheckClassFunc(savId, par);
	else root->Cur->CheckFunc(savId, par, true);
}

void Analisator::AnaliseSave() {
	SemanticTree* clas;
	int type;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tId) ShowError("Not id");
	clas = root->Cur->CheckIfVarExist(currentLex);
	scaner.Scaner(currentType, currentLex);
	while (currentType == tToch) {
		scaner.Scaner(currentType, currentLex);
		if (currentType != tId) ShowError("Not id");
		clas = clas->CheckClassVar(currentLex);
		scaner.Scaner(currentType, currentLex);
	}
	if (currentType != tSave) ShowError("Not =");
	type = AnaliseExpression();
	if (type == TypeDef) ShowError("Not allow type");
}

void Analisator::AnaliseFor() {
	scaner.Scaner(currentType, currentLex);
	if (currentType != tFor) ShowError("Not for");
	scaner.Scaner(currentType, currentLex);
	if (currentType != tLs) ShowError("Not (");
	AnaliseSave();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tTzpt) ShowError("Not ;");
	AnaliseExpression();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tTzpt) ShowError("Not ;");
	AnaliseSave();
	scaner.Scaner(currentType, currentLex);
	if (currentType != tPs) ShowError("Not )");
	AnaliseSosOp();
}

void Analisator::AnaliseFunc() {
	SemanticTree* savedPos;
	scaner.Scaner(currentType, currentLex);
	if (currentType != tVoid) ShowError("Not void");
	scaner.Scaner(currentType, currentLex);
	if (currentType != tId && currentType != tMain) ShowError("Not id");
	root->Cur->CheckFunc(currentLex, 0, false);
	root->Add(&currentLex, TFunc)->SetTypeVar(new std::string("void"));
	scaner.Scaner(currentType, currentLex);
	if (currentType != tLs) ShowError("Not (");
	scaner.Scaner(currentType, currentLex);
	if (currentType != tPs) ShowError("Not )");
	savedPos = root->NewBlock();
	AnaliseSosOp();
	root->SetCurrent(savedPos);
}

void Analisator::ShowError(std::string message) {
	std::cout << message + "\nAt line" + std::to_string(scaner.GetLine()) << std::endl;
	//root->Print();
	system("pause");
	std::exit(0);
}