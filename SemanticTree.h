#pragma once
#include <string>
#include <iostream>
#define MAX_N 50

enum TypeObj {
	TVar = 1,
	TFunc,
	TType
};



struct Node {
	std::string id;
	int typeObject = -1;
	std::string typeVar;
	std::string ParentClass;
	int param = 0;
};

class SemanticTree
{
public:
	Node* n;
	SemanticTree* Left, *Right, *Up;
	static SemanticTree* Cur;
	SemanticTree(SemanticTree* l, SemanticTree* r, SemanticTree* u, Node* Data);
	void SetLeft(Node* Data);
	void SetRight(Node* Data);
	SemanticTree* FindUp(SemanticTree* From, std::string id);
	SemanticTree* FindUpInLev(SemanticTree* From, std::string id);
	SemanticTree* FindDown(SemanticTree* From, std::string id);
	SemanticTree* Add(std::string* lex, int objType);
	SemanticTree* NewBlock();
	void PrintError(std::string error, std::string lex);
	void SetTypeVar(std::string* type);
	void SetParentClass(std::string* clas);
	static void SetCurrent(SemanticTree* tree);
	void AddToFunc();
	std::string GetId() { return n->id; };
	int GetTypeObject() { return n->typeObject; };
	std::string GetTypeVar() { return n->typeVar; };
	int GetParam() { return n->param; };
	void Print();

	void CheckFunc(std::string lex, int par, bool typeFind);
	SemanticTree* CheckIfVarExist(std::string lex);
	void CheckIfVarNotExist(std::string lex);
	void CheckClass(std::string lex, bool typeFind);
	void CheckClassFunc(std::string lex, int par);
	SemanticTree* CheckClassVar(std::string lex);
};