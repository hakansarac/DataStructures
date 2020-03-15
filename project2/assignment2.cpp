/*
Student Name:   Hakan SARAC
Student Number: 150140061
Date:           08 / 11 / 2018
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#define MAX_CHAR 100000
using namespace std;


struct occur_node {
	char character;
	occur_node *next;
	int occurence;
};


struct vocab_node {
	char character;
	vocab_node *next;
	occur_node *list;
};


struct vocab_list {
	vocab_node *head;
	void create();
	void print();
	void add_char(char);
	void add_occurence(char, char);
	int get_occurence(char);
	int get_union_occurence(char, char);
};


struct language_model {
	vocab_list *vocabularylist;
	void readData(const char *);
	double calculateProbability(char, char);
};



typedef language_model language_data_list;
language_data_list language_data;
typedef vocab_node vocab_node;
typedef occur_node occur_node;



void vocab_list::create() {
	head = NULL;
}

void vocab_list::print() {

	vocab_node *iter = head;
	occur_node *iter_occ;
	while (iter != NULL) {
		if (iter->character == ' ') {
			cout << "<SP>" << " :" << endl;
		}
		else {
			cout << iter->character << " :" << endl;
		}
		iter_occ = iter->list;
		while (iter_occ != NULL) {
			if (iter_occ->character == ' ') {
				cout << "      <" << "<SP>" << " , " << iter_occ->occurence << ">" << endl;
			}
			else {
				cout << "      <" << iter_occ->character << " , " << iter_occ->occurence << ">" << endl;
			}
			iter_occ = iter_occ->next;
		}
		cout << endl << endl;
		iter = iter->next;
	}

}

void vocab_list::add_char(char new_ch) {



	int dif = 'z' - 'Z';
	if (new_ch >= 'A' && new_ch <= 'Z')  new_ch += dif;   //buyuk harfleri kucult

	vocab_node *iter = head;
	vocab_node *temp = new vocab_node;

	//head null ise yeni olu�tur
	if (head == NULL) {
		head = new vocab_node;
		head->character = new_ch;
		head->next = NULL;
		head->list = NULL;

	}

	//headle ayni ise bir sey yapma
	else if (head->character == new_ch) {}

	//headden kucuk ve alfabediki bir karakterse basa at
	else if (head->character > new_ch && (new_ch >= 97 && new_ch <= 122)) {

		temp->next = iter;
		temp->character = new_ch;
		temp->list = NULL;
		head = temp;

	}

	//alfabe karakteri ve headden kucuk degilse araya sirasina gore at
	else if (new_ch >= 97 && new_ch <= 122) {

		while (iter->next != NULL) {
			if (iter->next->character >= new_ch || iter->next->character < 97 || iter->next->character>122) break;
			iter = iter->next;
		}
		if (iter->next->character == new_ch) {}
		else {
			temp->next = iter->next;
			iter->next = temp;
			temp->character = new_ch;
			temp->list = NULL;
		}

	}

	//ozel karakterse sona at
	else {

		while (iter->next != NULL) {
			if (iter->next->character == new_ch) { return; }
			iter = iter->next;
		}

		iter->next = new vocab_node;
		iter->next->next = NULL;
		iter->next->character = new_ch;
		iter->next->list = NULL;


	}
}


void vocab_list::add_occurence(char first, char second) {
	int flag = 0;
	vocab_node *iter = head;
	first = first >= 'A' && first <= 'Z' ? first + 32 : first;
	second = second >= 'A' && second <= 'Z' ? second + 32 : second;      //buyuk harfleri kucult
	while (iter->character != first) {
		iter = iter->next;
	}
	occur_node *iter_occ = iter->list;
	if (iter_occ == NULL) {
		iter->list = new occur_node;
		iter->list->next = NULL;
		iter->list->character = second;
		iter->list->occurence = 1;
	}
	else {
		if (iter_occ->character == second) {
			iter_occ->occurence++;

		}
		else {
			while (iter_occ->next != NULL) {
				if (iter_occ->next->character == second) {
					flag = 1;
					iter_occ->next->occurence++;
					break;
				}
				iter_occ = iter_occ->next;
			}
			if (iter_occ->next == NULL && flag == 0) {
				iter_occ->next = new occur_node;
				iter_occ->next->next = NULL;
				iter_occ->next->character = second;
				iter_occ->next->occurence = 1;
			}
		}
	}
}


int vocab_list::get_occurence(char first) {
	vocab_node *iter = head;
	int count = 0;
	while (iter->character != first) {
		iter = iter->next;
	}
	occur_node *iter_occ = iter->list;
	while (iter_occ != NULL) {
		count += iter_occ->occurence;
		iter_occ = iter_occ->next;
	}
	return count;
}


int vocab_list::get_union_occurence(char first, char second) {
	vocab_node *iter = head;
	while (iter->character != first) {
		iter = iter->next;
	}
	occur_node *iter_occ = iter->list;
	if (iter_occ->character == second) {
		return iter_occ->occurence;
	}

	else {
		while (iter_occ != NULL) {
			if (iter_occ->character == second) {
				return iter_occ->occurence;
			}
			iter_occ = iter_occ->next;
		}
	}
	return 0;
}



void language_model::readData(const char *ftpr) {

	vocabularylist = new vocab_list;


	FILE *r = fopen(ftpr, "r");

	char lines[MAX_CHAR];

	vocabularylist->create();
	for (int i = 0; !feof(r); i++) {
		fgets(lines, MAX_CHAR, r);
		int j = 0;
		while (lines[j] != 0 && lines[j] != 10 && lines[j] != 13) {

			vocabularylist->add_char(lines[j]);
			j++;

		}

		j = 1;


		while ((lines[j - 1] != 0 && lines[j - 1] != 10 && lines[j - 1] != 13) && (lines[j] != 0 && lines[j] != 10 && lines[j] != 13)) {
			vocabularylist->add_occurence(lines[j - 1], lines[j]);
			j++;
		}
		vocabularylist->add_occurence(lines[j - 1], ' ');
	}
}


double language_model::calculateProbability(char first, char second) {
	return vocabularylist->get_union_occurence(first, second)*(1.0) / vocabularylist->get_occurence(first);
}





int main(int argc, char **argv) {

	language_data.readData(argv[1]);
	
	//3 input varsa olasilik hesapla 
	if (argc > 3) {
		char *one = argv[2];
		char *two = argv[3];
		
		if (one[0]=='<' && one[1]=='S') {
			cout << language_data.calculateProbability(' ',*two) << endl;
		}

		else if(two[0]=='<' && two[1]=='S'){
			cout << language_data.calculateProbability(*one,' ') << endl;
		}

		else if(one[0] == '<' && one[1] == 'S' && two[0] == '<' && two[1] == 'S'){
			cout << language_data.calculateProbability(' ',' ') << endl;
		}

		else {

			cout << language_data.calculateProbability(*one, *two) << endl;
		}
	}
	//3 input yoksa yazdir
	else {
		language_data.vocabularylist->print();
	}

	return 0;
}