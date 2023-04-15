#include "stdafx.h"
#include "SemanticTree.h"

SemanticTree* SemanticTree::Cur = nullptr;

SemanticTree::SemanticTree(SemanticTree* l, SemanticTree* r, SemanticTree* u, Node* d)
{
	n = new Node;
	Up = u; Left = l; Right = r;
	memcpy(n, d, sizeof(Node));
}

void SemanticTree::SetLeft(Node* Data)
// создать левого потомка от текущей вершины 
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data);
	Left = a; // связали с новой вершиной
}

void SemanticTree::SetRight(Node* Data)
// создать правого потомка от текущей вершины 
{
	SemanticTree* a = new SemanticTree(NULL, NULL, this, Data);
	Right = a; // связали с новой вершиной
}

SemanticTree* SemanticTree::FindUp(SemanticTree* From, std::string id)
// поиск данных от заданной вершины до корня вверх 
{
	SemanticTree* i = From; // текущая вершина поиска
	while ((i != NULL) && (id != i->n->id))
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

SemanticTree* SemanticTree::FindUpInLev(SemanticTree* From, std::string id){
	SemanticTree* i = From; // текущая вершина поиска
	while ((i != NULL) && (id != i->n->id) && i->n->id != "")
		i = i->Up; // поднимаемся наверх по связям
	return i;
}

SemanticTree* SemanticTree::FindDown(SemanticTree* From, std::string id) {
	SemanticTree* i = From; // текущая вершина поиска
	while ((i != NULL) && (id != i->n->id))
		i = i->Left; // спускаемся вниз по связям
	return i;
}

SemanticTree* SemanticTree::Add(std::string* lex, int objType) {
	Node newData;
	newData.typeObject = objType;
	newData.id = *lex;
	newData.typeVar = "";
	Cur->SetLeft(&newData);
	Cur = Cur->Left;
	return Cur;
}

SemanticTree* SemanticTree::NewBlock() {
	SemanticTree* sav;
	Node dat;
	dat.id = "";
	dat.typeObject = NULL;
	if (Cur->Right != NULL) {
		Cur->SetLeft(&dat);
		Cur = Cur->Left;
	}
	Cur->SetRight(&dat);
	sav = Cur;
	Cur = Cur->Right;
	return sav;
}

void SemanticTree::PrintError(std::string error, std::string lex) {
	std::cout << error + ": " + lex << std::endl;
	//Print();
	system("pause");
	exit(1);
}

void SemanticTree::SetTypeVar(std::string* type) {
	n->typeVar = *type;
}

void SemanticTree::SetParentClass(std::string* clas) {
	n->ParentClass = *clas;
}

void SemanticTree::SetCurrent(SemanticTree* tree) {
	Cur = tree;
}

void SemanticTree::AddToFunc() {
	n->param++;
}

void SemanticTree::Print() {
	std::cout << "ID -> " + n->id << std::endl;
	if (Up != NULL) std::cout << "Up -> " + Up->n->id << std::endl;
	if (Left != NULL) std::cout << "Left -> " + Left->n->id << std::endl;
	if (Right != NULL) std::cout << "Right -> " + Right->n->id << std::endl;
	std::cout << std::endl;
	if (Right != NULL) Right->Print();
	if (Left != NULL) Left->Print();
}

void SemanticTree::CheckFunc(std::string lex, int par, bool typeFind) {
	bool res = false;
	SemanticTree* tree;
	tree = FindUp(this, lex);
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TFunc && tree->n->param == par)
			res = true;
		tree = FindUp(tree->Up, lex);
	}
	if (res != typeFind) {
		if (typeFind) PrintError("Func dont exist", lex);
		else PrintError("Func already exist", lex);
	}
}

SemanticTree* SemanticTree::CheckIfVarExist(std::string lex) {
	bool res = false;
	SemanticTree* tree;
	SemanticTree* var;
	tree = FindUp(this, lex);
	var = tree;
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TVar) {
			res = true;
			var = tree;
		}
		else tree = FindUp(tree->Up, lex);
	}
	if (!res) PrintError("Var dont exist", lex);
	tree = FindUp(tree, var->GetTypeVar());
	res = false;
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TType) {
			res = true;
		}
		else tree = FindUp(tree->Up, var->n->typeVar);
	}
	if (res) return tree;
	else return NULL;
	return NULL;
}

void SemanticTree::CheckIfVarNotExist(std::string lex){
	bool res = false;
	SemanticTree* tree;
	SemanticTree* var;
	tree = FindUpInLev(this, lex);
	var = tree;
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TVar) {
			res = true;
			var = tree;
		}
		else if (tree->n->id == "") break;
		else tree = FindUpInLev(tree->Up, lex);
	}
	if (res) PrintError("Var already exist", lex);
}

void SemanticTree::CheckClass(std::string lex, bool typeFind) {
	bool res = false;
	SemanticTree* tree;
	tree = FindUp(this, lex);
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TType)
			res = true;
		tree = FindUp(tree->Up, lex);
	}
	if (res != typeFind) {
		if (typeFind) PrintError("Class dont exist", lex);
		else PrintError("Class already exist", lex);
	}
	
}

void SemanticTree::CheckClassFunc(std::string lex, int par) {
	bool res = false;
	SemanticTree* tree;
	tree = FindDown(this->Right, lex);
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TFunc && tree->n->param == par)
			res = true;
		tree = FindDown(tree->Left, lex);
	}
	if (!res) PrintError("Func is not in type", lex);
}

SemanticTree* SemanticTree::CheckClassVar(std::string lex) {
	bool res = false;
	SemanticTree* tree;
	SemanticTree* var;
	tree = FindDown(this->Right, lex);
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TVar) {
			res = true;
			var = tree;
		}
		else tree = FindDown(tree->Left, lex);
	}
	if (!res) PrintError("Var is not in type", lex);
	tree = FindUp(this, var->n->typeVar);
	res = false;
	while (!res && tree != NULL) {
		if (tree->n->typeObject == TType) {
			res = true;
		}
		else tree = FindUp(tree->Up, var->n->typeVar);
	}
	if (res) return tree;
	else return NULL;
}