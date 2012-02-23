// tree.cpp : Defines the entry point for the console application.
//� ��������� ��������� ����������� ����.��� � �� ��������� ��� �����. � ��� �����
//���������� ������� ������-�������.

#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include<string>
#include<stdarg.h>
using namespace std;
const int NoSubTree=0;
const int LeftSubTree=-1;
const int RightSubTree=1;
const int TwoSubTree=2;
const int MAX_LEN=200;
struct words{
	char *eng;		//�����-����� � ��
	char *ukr;		// �������� �����
};
struct tree_node{		//������ ��������� ����� ������
	words wrd;			// ���� �����
	tree_node *left, *right;		//��������� �� ������ ��������
};

tree_node *root;		//�������� �� ����� ������
int GetData(words *pwrd);
tree_node* NewNode();
void NewNode(tree_node *pnew, tree_node **root_addr);
void PrintTree(tree_node * proot);
void ShowTree(tree_node *proot, int lev=1);
int TreeHeight(tree_node *proot);
tree_node* FindNode(char* term);
void DeleteNode(char *term, tree_node** root_addr);
void FreeNodeMemo(tree_node *proot);
void FreeTree(tree_node *proot);
void ShowLevels();				
char* LenWords(int k, ...);			//������� � ������ ������� ���������, �������� �������� �����
int _tmain(int argc, _TCHAR* argv[])
{
	tree_node *node;
	char term[30];
	cout<<"\tForming a tree. To complete the input press * \n";
	while((node=NewNode())!=NULL)		//���� �� ����� �����
		NewNode(node, &root);			//��������� ����� � �������� ���� �� ��� ��������
	cout<<"\t\nFormed tree: \n";
	PrintTree(root);		//����������� ������
	cout<<"\nheight of the tree - "<<TreeHeight(root)<<", left subtree - "<<TreeHeight(root->left)		//�������� ������ ������
		<<", right subtree - "<<TreeHeight(root->right)<<endl;
	cout<<"Structure of the tree: \n";
	ShowTree(root);				//��������� ������(������-����)
	cout<<"\nSearch terms: \nTerm -> ";
	while(*gets(term)!='*'){						//���� ������; ���� �� ����� �����
		if((node=FindNode(term))!=NULL)			//���� ����� � � �����
			cout<<"\t Translate: "<<node->wrd.ukr;		//�������� ��������
		else  cout<<"This term doesnt exist!";
		cout<<"\nThe next term: ->";
	}
	cout<<"\nRemove the words:\nTerm ->";
	while(*gets(term)!='*'){		//���� ���������
		DeleteNode(term,&root);
		cout<<"The next term ->";
	}
	cout<<"\n Tree Structure after removing ";
	ShowTree(root);			//����������� �������� ������	
	// ������������ ������� � ������ ������� ���������
	//���� �������� �������� ����� � ������ � ���� ������� �����
	cout<<"\nThe longest word among root and 2 child nodes ->  ";
	if(root!=NULL && root->left!=NULL && root->right!=NULL){		//���� ������ ����� �������
		cout<<LenWords(3,root->wrd.eng, root->left->wrd.eng, root->right->wrd.eng);		//������ ������� � �������� �������� 
		cout<<"\nThe longest word among root and 2 child nodes  ->  ";
		cout<<LenWords(2,root->left->wrd.eng, root->right->wrd.eng);
	}
	else cout<<"Not enought elements!";		//���� ������� ����� ����
	FreeTree(root);
	root=NULL;					
	system("pause");		
	return 0;
}

// ������� �������� ���-������ � �� ���������
int GetData(words *pwrd){
	static int num=1;			//��� ��������� ������� ���
	char wbuf[150];
	cout<<num++<<" term ->";
	if(*gets(wbuf)=='*') return 0;  //������ ���� ��������
	pwrd->eng=new char[strlen(wbuf)+1];
	strcpy(pwrd->eng, wbuf);	// ����� �����-������ � ���������
	cout<<"Translate ->";
	gets(wbuf);			//�������� ��������
	pwrd->ukr=new char[strlen(wbuf)+1];			//�������� �� ����� ������
	strcpy(pwrd->ukr, wbuf);			//� �������� � ���������
	return 1;
}
//  ������� � ������ ������� ���������
	//���� �������� �������� ����� � ������ � ���� ������� �����
char* LenWords(int k, ...){
	char *arg, *pm;
	int len, max=0;
	va_list param;
	va_start(param,k);
	for(int j=1; j<=k; j++){
		arg=va_arg(param,char*);
		if((len=strlen(arg))>max) {
			max=len;
			pm=arg;
		}
	}
	va_end(param);
	return pm;
}
// ������� ��������� ��, ������� ������ ������
void FreeNodeMemo(tree_node *pnode){
	delete pnode->wrd.eng;		// ��������� ����� � ��
	delete pnode->wrd.ukr;
	delete pnode;		//��������� �����
}
//������� ���������� ������ ����� ������. ������ �� ��� ����� � �������� ���� ������ ��������� �����, 
//�������� ����� GetData.
tree_node* NewNode(){
	tree_node* pel;
	words buf;
	int (*p) (words *);			//�������� �� �������
	p=GetData;					//GetData
	if(p(&buf)==0)		//����� ��������
		return 0;
	pel=new tree_node [sizeof(tree_node)];			//�������� �� ����� ������
	pel->wrd=buf;		//���������� ������ �����
	pel->left=pel->right=NULL;		//����� ������� ��� �������
	// ������� ������ ������������ �����
	return pel;
}

// ������� ��������� ������ ����� �� �������������� ������
void NewNode(tree_node* pnew, tree_node** root_addr){
	tree_node*proot=*root_addr;		//�������� �� �����
	int cmp;
	if(proot==NULL){		//������ ������
		*root_addr=pnew;		//����� ������� ��� ������� ������
		return;
	}
	if(!(cmp=strcmp(proot->wrd.eng, pnew->wrd.eng)))		//����� ��� �
		FreeNodeMemo(pnew);		// ��������� ��
	else 
		if(cmp>0)
			NewNode(pnew, &proot->left);		//������ � ��� ��������
		else NewNode(pnew, &proot->right);		//������ � ����� ��������
		
}

//������� ��������� ����� ������ � ������������ �������
void PrintTree(tree_node* proot){
	if(proot==NULL) return;
	PrintTree(proot->left);		//��������� ����� ��������
	printf("%-15s - %s\n", proot->wrd.eng, proot->wrd.ukr);		//�����
	PrintTree(proot->right);		//��������� ������� ��������
}
// ������� ����������� ��������� ������
void ShowTree(tree_node* proot, int lev){
	if(lev==1) ShowLevels();		//��������� ����� ������� ����
	if(proot==NULL) return;
	ShowTree(proot->right, lev+1);        // ����������� ������� ��������
	printf("%*c%s\n", lev*8,' ',proot->wrd.eng);		//��������� ������	
	ShowTree(proot->left, lev+1);			// ����������� ����� ��������
}
// ������� ���������� ������ ������
int TreeHeight(tree_node *proot){
	int lh, rh;
	if(proot==NULL) return 0;
	lh=TreeHeight(proot->left);			//������ ����� ��������
	rh=TreeHeight(proot->right);			//������ ������� ��������
	return lh>rh?lh+1:rh+1;			//+1 ������� �����
}
// ������� ��������� ����� ������� ����
void ShowLevels(){
	int lvl;
	cout<<"\nLevel: ";
	for(lvl=1; lvl<=TreeHeight(root); lvl++)
		printf("%-8d", lvl);
	printf("\n");
}

// ������� ������ �������� ����� � �������� �����
tree_node* FindNode(char* term){
	tree_node* pnode=root;
	int cmp;
	while(pnode!=NULL){			// ���� ������ �����
		cmp=strcmp(pnode->wrd.eng, term);		//��������� ������
		if(cmp==0) return pnode;		//����� ��������
		else 
			if(cmp>0)
				pnode=pnode->left;			//����� � ����� �������
			else
				pnode=pnode->right;      //����� � ������� �������
	}
	return NULL;		//����� �� ��������
}
// ������� ��������� ����� ������, �� ������ ������ �����

void DeleteNode(char* term, tree_node **root_addr){
	 tree_node *proot=*root_addr, *pnewr, *ppar;
	 int cmp, subtr;
	 if(proot==NULL) return;
	 cmp=strcmp(proot->wrd.eng, term);	//��������� ������
	 if (cmp==0){		//��������� ���������� ��������
		 if(proot->left==NULL && proot->right==NULL)	//����������
			 subtr=NoSubTree;		//������ �������
		 else   if (proot->left==NULL)
				   subtr=RightSubTree;
				else if (proot->right==NULL)
						subtr=LeftSubTree;
				else 
					subtr=TwoSubTree;
		switch(subtr) {			//���������� ������
		case NoSubTree:  *root_addr=NULL; break;		//���� �������
		case LeftSubTree:  *root_addr=proot->left; break;	//� ����� ��� ��������
		case RightSubTree:  *root_addr=proot->right; break;		//� ����� ����� ��������
		case TwoSubTree:							//� ������ ��������
							pnewr=proot->left;		//pnewr ����� �����
							ppar=proot;				//����������� ����� pnewr
			while(pnewr->right!=NULL){  //����� ����� �����
				ppar=pnewr;		
				pnewr=pnewr->right;
			}
			pnewr->right=proot->right;		//��������� ������� ��������
			if(ppar!=proot){
				ppar->right=pnewr->left;		//����������� ������
				pnewr->left=proot->left;		//� ����� �������
			}
			*root_addr=pnewr;		// ���� ������
		}
		FreeNodeMemo(proot);		//��������� ������� ���������� �����
		return;
	 }
	 if(cmp>0) 
		 DeleteNode(term, &proot->left);	// ��������� ����� � ����� �������
	 else DeleteNode(term, &proot->right);  //��������� ����� � ������� �������
}
// ������� ������� ��������� ������
void FreeTree(tree_node *proot){
	if(proot==NULL) return;		//������ ������
	FreeTree(proot->left);		//��������� ����� ��������
	FreeTree(proot->right);		//��������� ������� ��������
	FreeNodeMemo(proot);		//��������� ���������� ��������
}

