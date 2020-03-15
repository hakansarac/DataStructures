/*
Student Name:   Hakan SARAC
Student Number: 150140061
Date:           02 / 12 / 2018
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

#define MAX_CARD 1000


struct StackArray {
	int arr[MAX_CARD];
	int top;
	void push(int);
	int pop();
	bool isEmpty();
};   //elements of my stacks

struct Game {
	StackArray *Player1;
	StackArray *Player2;
	StackArray *Table;
	int bin_count; //creatte 0ladim
	void create(); //yaptim
	void ReadData(const char *);  //yaptim
	void CardGame(int); //flag alcak
};  //elements of my game

typedef Game Play;
Play GamePlay;


void StackArray::push(int value) {
	arr[top++] = value;
}   //push function

int StackArray::pop() {
	return arr[--top];
}   //pop function

bool StackArray::isEmpty() {
	return (top == 0);
}   //isEmpty funtion

void Game::create() {
	bin_count = 0;        //at first count of my bin under table is 0
	Player1->top = 0;   //at first element number of Player1 deck is 0
	Player2->top = 0;   //at first element number of Player2 deck is 0
	Table->top = 0;     //at first element number of table deck is 0
}

void Game::ReadData(const char *fptr) {
	FILE *r = fopen(fptr, "r");

	Player1 = new StackArray;
	Player2 = new StackArray;
	Table = new StackArray;
	create();

	int tableDeckCount, playerDeckCount;
	int value;

	fscanf(r, "%d %d\n", &tableDeckCount, &playerDeckCount);


	for (int i = 0; i < tableDeckCount; i++) {
		fscanf(r, "%d\n", &value);
		Table->push(value);
	}  //push tabledeck

	for (int i = 0; i < playerDeckCount; i++) {
		fscanf(r, "%d\n", &value);
		Player1->push(value);
	}  //push player1

	for (int i = 0; i < playerDeckCount; i++) {
		fscanf(r, "%d\n", &value);
		Player2->push(value);
	}  //push player2
}

void Game::CardGame(int flag) {
	int tablecard;

	while (!Table->isEmpty()) {
		tablecard = Table->pop();

		if (flag == 1) {      //its Player1 turn

			if (tablecard < 0) {  //player1 pop player2 push

				for (int i = 0; i < -(tablecard); i++) {

					if (Player1->arr[Player1->top - 1] > Player2->arr[Player2->top - 1]) { //player1's last card greater than player2's, player1 pop player2 push 
						Player2->push(Player1->pop());

						if (Player1->isEmpty()) {   //player1 wins game ends
							cout << bin_count;
							return;
						}
					}

					else {     //player2's last element greater than player1's, not push player2 but pop player1
						bin_count++;
						Player1->pop();

						if (Player1->isEmpty()) {  //player1 wins game ends
							cout << bin_count;
							return;
						}
					}
				}
			}

			else {         //player2 pop player1 push

				for (int i = 0; i < tablecard; i++) {

					if (Player2->arr[Player2->top - 1] > Player1->arr[Player1->top - 1]) { //player2's last card greater than player1's, player2 pop player1 push 
						Player1->push(Player2->pop());

						if (Player2->isEmpty()) {   //player2 wins game ends
							cout << bin_count;
							return;
						}
					}

					else {     //player1's last element greater than player2's, not push player1 but pop player2
						bin_count++;
						Player2->pop();

						if (Player2->isEmpty()) {
							cout << bin_count;
							return;
						}
					}
				}
			}
		}

		else if (flag == 2) {   //its Player2 turn

			if (tablecard > 0) {  //player1 pop player2 push

				for (int i = 0; i < tablecard; i++) {

					if (Player1->arr[Player1->top - 1] > Player2->arr[Player2->top - 1]) {  //player1's last card greater than player2's, player1 pop player2 push
						Player2->push(Player1->pop());

						if (Player1->isEmpty()) {   //player1 wins game ends
							cout << bin_count;
							return;
						}
					}

					else {       //player2's last element greater than player1's, not push player2 but pop player1
						bin_count++;
						Player1->pop();

						if (Player1->isEmpty()) {   //player1 wins game ends
							cout << bin_count;
							return;
						}
					}
				}
			}

			else {      //player2 pop player1 push

				for (int i = 0; i < -(tablecard); i++) {

					if (Player2->arr[Player2->top - 1] > Player1->arr[Player1->top - 1]) {     //player2's last card greater than player1's, player2 pop player1 push
						Player1->push(Player2->pop());

						if (Player2->isEmpty()) {   //player2 wins game ends
							cout << bin_count;
							return;
						}
					}

					else {       //player1's last element greater than player2's, not push player1 but pop player2
						bin_count++;
						Player2->pop();

						if (Player2->isEmpty()) {   //player2 wins game ends
							cout << bin_count;
							return;
						}
					}
				}
			}
		}
		if (flag == 1) flag = 2;
		else if (flag == 2) flag = 1;     //change of turn
	}
	cout << bin_count;     //if there is no card in tabledeck
}

int main(int argc, char **argv) {
	GamePlay.ReadData(argv[1]); //read .game documents   
	GamePlay.CardGame(1);        //play card game, 1. player fisrt turn

	argc++;
	return 0;
}