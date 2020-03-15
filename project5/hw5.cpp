/*
Student Name:   Hakan SARAC
Student Number: 150140061
Date:           24 / 12 / 2018
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#define MAX_SIZE 101

using namespace std;

struct elements {
	int value;
	elements *left;
	elements *right;
};

struct stack {
	int stackValue;
	stack *next;
};

struct stackTree {
	stack *head;
	void create();//
	void close();//
	void push(int);//
	int pop();  //
	bool isEmpty(); //
};

struct treeNode {
	elements *root;
	stackTree myStack;
	int target;
	void create();//
	void close(elements *); //
	void add(int, int); //
	int arr[MAX_SIZE];
	void readFile(const char *); //
	bool checkPath(elements *, int);
	void print();
};

void stackTree::create() {
	head = NULL;
}

void stackTree::close() {
	stack *tra;
	while (head) {
		tra = head;
		head = head->next;
		delete tra;
	}
}

void stackTree::push(int a) {
	stack *temp = new stack;
	temp->stackValue = a;
	temp->next = head;
	head = temp;
}

int stackTree::pop() {
	stack *temp;
	int a = head->stackValue;
	temp = head;
	head = head->next;
	delete temp;
	return a;
}

bool stackTree::isEmpty() {
	return head == NULL;
}

void treeNode::create() {
	root = NULL;
}

void treeNode::close(elements *p) {
	if (p) {
		if (p->left != NULL) {
			close(p->left);
			p->left = NULL;
		}
		if (p->right != NULL) {
			close(p->right);
			p->right = NULL;
		}
		delete p;
	}
}
void treeNode::readFile(const char *fptr) {
	ifstream r(fptr);
	int i = 0;
	int temp = 0;
	target = 0;
	while (!r.eof()) {
		r >> temp;
		arr[++i] = temp;
	}
	target = arr[i];
	arr[i] = -1;
	i = 1;
	while (arr[i] != -1) {
		add(arr[i], i);
		i++;
	}
}

void treeNode::add(int newElement, int i) {
	elements *newnode = new elements;
	elements *tra;
	myStack.create();
	newnode->value = newElement;
	newnode->left = NULL;
	newnode->right = NULL;
	if (root == NULL) {
		root = newnode;
		return;
	}

	tra = root;
	while (i != 1) {
		myStack.push(i % 2);
		i = i / 2;
	}
	int temp;;
	while (!myStack.isEmpty()) {
		temp = myStack.pop();
		if (temp == 0) {
			if (tra->left) {
				tra = tra->left;
			}
			else {
				tra->left = newnode;
			}
		}
		else {
			if (tra->right) {
				tra = tra->right;
			}
			else {
				tra->right = newnode;
			}
		}
	}
}

bool treeNode::checkPath(elements *tra, int target) {
	if (!tra) return false; //if we've reached the end of the tree
	if (target - tra->value < 0) return false;
	myStack.push(tra->value);
	if (target - tra->value == 0) return true;
	if (checkPath(tra->left, target - tra->value)) return true; // left sum is done
	if (checkPath(tra->right, target - tra->value)) return true; // right sum is done
	//else
	myStack.pop();
	return false;
}

void treeNode::print() {
	checkPath(root->left, target - root->value); //check left path
	int orderarr[100],i=0;
	if (!myStack.isEmpty()) {
		cout << "Path Found: ";
		cout << root->value << " ";
		while (!myStack.isEmpty()) {
			orderarr[i++] = myStack.pop();
		}
		i--;
		while (i >= 0) {
			cout << orderarr[i--] << " ";
		}
		cout << endl;
	}
	else cout << "No Path Found" << endl;
	i = 0;
	checkPath(root->right, target - root->value); //check right path
	if (!myStack.isEmpty()) {
		cout << "Path Found: ";
		cout << root->value << " ";
		while (!myStack.isEmpty()) {
			orderarr[i++] = myStack.pop();
		}
		i--;
		while (i >= 0) {
			cout << orderarr[i--]<<" ";
		}
		cout << endl;
	}
	else cout << "No Path Found" << endl;
	myStack.close();

}

int main(int argc, char **argv) {
	treeNode a;
	a.create();
	a.readFile(argv[1]);
	a.print();
	a.close(a.root);
	return 0;
}