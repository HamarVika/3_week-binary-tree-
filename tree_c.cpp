// tree.cpp : Defines the entry point for the console application.
//З клавіатури вводиться послідовність англ.слів і їх перекладів укр мовою. З цих даних
//сформувала двійкове дерево-словник.

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
	char *eng;		//слово-термін в ДП
	char *ukr;		// переклад слова
};
struct tree_node{		//шаблон структури вузла дерева
	words wrd;			// поле даних
	tree_node *left, *right;		//вказівники на дочірні елементи
};

tree_node *root;		//вказівник на корінь дерева
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
char* LenWords(int k, ...);			//функція зі змінною кількістю параметрів, виводить найдовше слово
int _tmain(int argc, _TCHAR* argv[])
{
	tree_node *node;
	char term[30];
	cout<<"\tForming a tree. To complete the input press * \n";
	while((node=NewNode())!=NULL)		//поки не кінець вводу
		NewNode(node, &root);			//створюємо вузол і приєднуємо його до вже існуючих
	cout<<"\t\nFormed tree: \n";
	PrintTree(root);		//відображення дерева
	cout<<"\nheight of the tree - "<<TreeHeight(root)<<", left subtree - "<<TreeHeight(root->left)		//виводимо висоту дерева
		<<", right subtree - "<<TreeHeight(root->right)<<endl;
	cout<<"Structure of the tree: \n";
	ShowTree(root);				//структура дерева(дерево-граф)
	cout<<"\nSearch terms: \nTerm -> ";
	while(*gets(term)!='*'){						//цикл пошуку; поки не кінець вводу
		if((node=FindNode(term))!=NULL)			//якщо слово є в дереві
			cout<<"\t Translate: "<<node->wrd.ukr;		//виводимо переклад
		else  cout<<"This term doesnt exist!";
		cout<<"\nThe next term: ->";
	}
	cout<<"\nRemove the words:\nTerm ->";
	while(*gets(term)!='*'){		//цикл видалення
		DeleteNode(term,&root);
		cout<<"The next term ->";
	}
	cout<<"\n Tree Structure after removing ";
	ShowTree(root);			//відображення зміненого дерева	
	// використання функції зі змінною кількістю параметрів
	//вона виводить найдовше слово з кореня і двох дочірніх вузлів
	cout<<"\nThe longest word among root and 2 child nodes ->  ";
	if(root!=NULL && root->left!=NULL && root->right!=NULL){		//якщо дочірні вузли існують
		cout<<LenWords(3,root->wrd.eng, root->left->wrd.eng, root->right->wrd.eng);		//рахуємо довжину і виводимо найдовше 
		cout<<"\nThe longest word among root and 2 child nodes  ->  ";
		cout<<LenWords(2,root->left->wrd.eng, root->right->wrd.eng);
	}
	else cout<<"Not enought elements!";		//якщо дочірніх вузлів немає
	FreeTree(root);
	root=NULL;					
	system("pause");		
	return 0;
}

// функція введення слів-термінів і їх перекладів
int GetData(words *pwrd){
	static int num=1;			//для підрахунку кількості слів
	char wbuf[150];
	cout<<num++<<" term ->";
	if(*gets(wbuf)=='*') return 0;  //ознака кінця введення
	pwrd->eng=new char[strlen(wbuf)+1];
	strcpy(pwrd->eng, wbuf);	// запис слова-терміна в структуру
	cout<<"Translate ->";
	gets(wbuf);			//отримуємо переклад
	pwrd->ukr=new char[strlen(wbuf)+1];			//виділяємо на нього память
	strcpy(pwrd->ukr, wbuf);			//і записуємо в структуру
	return 1;
}
//  функція зі змінною кількістю параметрів
	//вона виводить найдовше слово з кореня і двох дочірніх вузлів
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
// функція звільнення ДП, зайнятої вузлом дерева
void FreeNodeMemo(tree_node *pnode){
	delete pnode->wrd.eng;		// витирання рядків у ДП
	delete pnode->wrd.ukr;
	delete pnode;		//витирання вузла
}
//функція формування нового вузла дерева. Виділяє ДП для вузла і заносить туди адреси текстових рядків, 
//введених через GetData.
tree_node* NewNode(){
	tree_node* pel;
	words buf;
	int (*p) (words *);			//вказівник на функцію
	p=GetData;					//GetData
	if(p(&buf)==0)		//кінець введення
		return 0;
	pel=new tree_node [sizeof(tree_node)];			//виділяємо на вузол память
	pel->wrd=buf;		//заповнення нового вузла
	pel->left=pel->right=NULL;		//новий елемент стає листком
	// Повертає адресу сформованого вузла
	return pel;
}

// функція приєднання нового вузла до впорядкованого дерева
void NewNode(tree_node* pnew, tree_node** root_addr){
	tree_node*proot=*root_addr;		//вказівник на корінь
	int cmp;
	if(proot==NULL){		//дерево порожнє
		*root_addr=pnew;		//новий елемент стає коренем дерева
		return;
	}
	if(!(cmp=strcmp(proot->wrd.eng, pnew->wrd.eng)))		//слово вже є
		FreeNodeMemo(pnew);		// звільнення ДП
	else 
		if(cmp>0)
			NewNode(pnew, &proot->left);		//ввести у ліве піддерево
		else NewNode(pnew, &proot->right);		//ввести у праве піддерево
		
}

//функція виведення даних дерева в симетричному порядку
void PrintTree(tree_node* proot){
	if(proot==NULL) return;
	PrintTree(proot->left);		//виведення лівого піддерева
	printf("%-15s - %s\n", proot->wrd.eng, proot->wrd.ukr);		//корінь
	PrintTree(proot->right);		//виведення правого піддерева
}
// функція відображення структури дерева
void ShowTree(tree_node* proot, int lev){
	if(lev==1) ShowLevels();		//виведення рядка розмітки рівнів
	if(proot==NULL) return;
	ShowTree(proot->right, lev+1);        // відображення правого піддерева
	printf("%*c%s\n", lev*8,' ',proot->wrd.eng);		//виведення кореня	
	ShowTree(proot->left, lev+1);			// відображення лівого піддерева
}
// функція обчислення висоти дерева
int TreeHeight(tree_node *proot){
	int lh, rh;
	if(proot==NULL) return 0;
	lh=TreeHeight(proot->left);			//висота лівого піддерева
	rh=TreeHeight(proot->right);			//висота правого піддерева
	return lh>rh?lh+1:rh+1;			//+1 враховує корінь
}
// функція виведення рядка розмітки рівнів
void ShowLevels(){
	int lvl;
	cout<<"\nLevel: ";
	for(lvl=1; lvl<=TreeHeight(root); lvl++)
		printf("%-8d", lvl);
	printf("\n");
}

// функція пошуку заданого слова в бінарному дереві
tree_node* FindNode(char* term){
	tree_node* pnode=root;
	int cmp;
	while(pnode!=NULL){			// цикл пошуку слова
		cmp=strcmp(pnode->wrd.eng, term);		//порівняння термінів
		if(cmp==0) return pnode;		//слово знайдено
		else 
			if(cmp>0)
				pnode=pnode->left;			//пошук у лівому піддереві
			else
				pnode=pnode->right;      //пошук у правому піддереві
	}
	return NULL;		//слово не знайдено
}
// функція видалення вузла дерева, що містить задане слово

void DeleteNode(char* term, tree_node **root_addr){
	 tree_node *proot=*root_addr, *pnewr, *ppar;
	 int cmp, subtr;
	 if(proot==NULL) return;
	 cmp=strcmp(proot->wrd.eng, term);	//порівняння термінів
	 if (cmp==0){		//видалення кореневого елемента
		 if(proot->left==NULL && proot->right==NULL)	//визначення
			 subtr=NoSubTree;		//складу піддерев
		 else   if (proot->left==NULL)
				   subtr=RightSubTree;
				else if (proot->right==NULL)
						subtr=LeftSubTree;
				else 
					subtr=TwoSubTree;
		switch(subtr) {			//перебудова дерева
		case NoSubTree:  *root_addr=NULL; break;		//немає піддерев
		case LeftSubTree:  *root_addr=proot->left; break;	//є тільки ліве піддерево
		case RightSubTree:  *root_addr=proot->right; break;		//є тільки праве піддерево
		case TwoSubTree:							//є обидва піддерева
							pnewr=proot->left;		//pnewr новий корінь
							ppar=proot;				//батьківський вузол pnewr
			while(pnewr->right!=NULL){  //пошук вузла заміни
				ppar=pnewr;		
				pnewr=pnewr->right;
			}
			pnewr->right=proot->right;		//приєднання правого піддерева
			if(ppar!=proot){
				ppar->right=pnewr->left;		//перемикання звязків
				pnewr->left=proot->left;		//у лівому піддереві
			}
			*root_addr=pnewr;		// зміна кореня
		}
		FreeNodeMemo(proot);		//витирання старого кореневого вузла
		return;
	 }
	 if(cmp>0) 
		 DeleteNode(term, &proot->left);	// видалення вузла в лівому піддереві
	 else DeleteNode(term, &proot->right);  //видалення вузла в правому піддереві
}
// функція повного витирання дерева
void FreeTree(tree_node *proot){
	if(proot==NULL) return;		//дерево порожнє
	FreeTree(proot->left);		//витирання лівого піддерева
	FreeTree(proot->right);		//витирання правого піддерева
	FreeNodeMemo(proot);		//видалення кореневого елемента
}

