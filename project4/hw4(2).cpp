/*
Student Name:   Hakan SARAC
Student Number: 150140061
Date:           13 / 12 / 2018
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


//structs

struct NodeQueue {
	int ant_num;
	NodeQueue *next;
};    //data of queues

struct queueAnt {
	NodeQueue *front;
	NodeQueue *back;
	void create();
	void close();
	void enqueue(int);
	int dequeue();
	bool isEmpty();
};    

struct NodeStack {
	int ant_num;
	NodeStack *next;
};     //data of stacks

struct stackAnt {
	NodeStack *head;
	void create();
	void close();
	void push(int);
	int pop();
};

struct Ants {
	queueAnt ants;
	queueAnt holeDepths;
	stackAnt hole;
	void ReadFile(const char *);
	void ShowContents(bool);
	void CrossRoad();
};

//stack functions

void stackAnt::create() {
	head = NULL;
}

void stackAnt::close() {
	NodeStack *tra;
	while (head) {
		tra = head;
		head = head->next;
		delete tra;
	}
}

void stackAnt::push(int a) {
	NodeStack *temp = new NodeStack;
	temp->ant_num = a;
	temp->next = head;
	head = temp;
}

int stackAnt::pop() {
	NodeStack *temp;
	int a = head->ant_num;
	temp = head;
	head = head->next;
	delete temp;
	return a;
}

//queue functions

void queueAnt::create() {
	front = NULL;
	back = NULL;
}

void queueAnt::close() {
	NodeQueue *tra;
	while (front) {
		tra = front;
		front = front->next;
		delete tra;
	}
}

void queueAnt::enqueue(int a) {
	NodeQueue *temp = new NodeQueue;
	temp->ant_num = a;
	temp->next = NULL;
	if (isEmpty()) {
		back = temp;
		front = back;
	}
	else {
		back->next = temp;
		back = temp;
	}
}

int queueAnt::dequeue() {
	NodeQueue *temp;
	int a = front->ant_num;
	temp = front;
	front = front->next;
	delete temp;
	return a;
}

bool queueAnt::isEmpty() {
	return front == NULL;
}

//Ants functions

void Ants::ReadFile(const char *fptr) {

	holeDepths.create();
	ants.create();

	ifstream r(fptr);

	int i = 0;
	int antCoun;
	int depthCoun;
	r >> antCoun;   //first line, numbers of ants

	while (i < antCoun) {
		ants.enqueue(++i);    //add to ants queue
	}

	while (!r.eof()) {
		r >> depthCoun;
		if (r.eof()) break;
		holeDepths.enqueue(depthCoun);   //add to holeDepths
	}

	r.close();
}

void Ants::ShowContents(bool flag) {
	NodeQueue *iter;
	if (flag == false) {
		iter = holeDepths.front;
		while (iter) {             //print holeDepths
			cout << iter->ant_num << " ";
			iter = iter->next;
		}
		cout << endl;   
	}

	else {
		iter = ants.front;
		while (iter) {              //print ants number
			cout << iter->ant_num << " ";
			iter = iter->next;
		}
		cout << endl;
	}
}

void Ants::CrossRoad() {
	int holeSize;
	int temp;
	hole.create();
	while (!holeDepths.isEmpty()) {              
		holeSize = holeDepths.dequeue();
		for (int i = 0; i < holeSize; i++) {     //pop from ants queue push to hole stack
			temp = ants.dequeue();
			hole.push(temp);
		}
		for (int i = 0; i < holeSize; i++) {	 //pop from hole stack push to ants queue		
			temp = hole.pop();
			ants.enqueue(temp);
		}
	}
}

int main(int argc, char **argv) {
	Ants a;

	a.ReadFile(argv[1]); // store the number of ants and depths of holes
	cout << " The initial Ant sequence is: ";
	a.ShowContents(1); // list ant sequence ( initially : 1, 2, ... , N)
	cout << " The depth of holes are: ";
	a.ShowContents(0); // list depth of holes
	a.CrossRoad();
	cout << " The final Ant sequence is: ";
	a.ShowContents(1);

	a.hole.close();
	a.holeDepths.close();
	a.ants.close();
	return 0;
}