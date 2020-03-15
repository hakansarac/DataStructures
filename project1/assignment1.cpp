#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

typedef struct Cells {
	int row;
	int col;
	char type;
	int size;
}Cells;   //merkez bilgileri


typedef struct Gridst {
	int row;
	int col;
	Cells **arr;
	FILE *fptr;
	FILE *fptr2;
}Gridst;	//grid bilgileri

void GridFile(Gridst *grid, char *gridname) {
	grid->fptr = fopen(gridname, "r");
	fscanf(grid->fptr, "%d %d\n", &grid->row, &grid->col);		//grid filelarin ilk satirinin okunmasi
	grid->arr = new Cells*[grid->row];
	for (int i = 0; i < grid->row; i++) {
		grid->arr[i] = new Cells[grid->col];					
	}
	for (int i = 0; i < grid->row; i++) {						//griddeki hucrelerin bilgilerinin merkezlerinin bilgisine atanmasi ve bos tutulmasi
		for (int j = 0; j < grid->col; j++) {
			grid->arr[i][j].type = ' ';
			grid->arr[i][j].row = -1;
			grid->arr[i][j].col = -1;
			grid->arr[i][j].size = 0;			
		}
	}
	cout << "A grid is created: " << grid->row << " " << grid->col << endl;
}



void ErrorMessage1(int messagetype,char shape,int size,int row,int col) {			//grid textlerin mesajlari
	if (messagetype == 0) {
		cout << "SUCCESS: Operator " << shape << " with size " << size << " is placed on (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 1) {
		cout << "BORDER ERROR: Operator " << shape << " with size " << size << " can not be placed on (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 2) {
		cout << "CONFLICT ERROR: Operator " << shape << " with size " << size << " can not be placed on (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 3) {
		cout << "BORDER ERROR: Operator " << shape << " with size " << size << " can not be placed on (" << row + 1 << "," << col + 1 << ")." << endl;
		cout << "CONFLICT ERROR: Operator " << shape << " with size " << size << " can not be placed on (" << row + 1 << "," << col + 1 << ")." << endl;
	}
}

void ErrorMessage2(int messagetype, char shape, int row, int col,int before_row,int before_col) {		//inst textlerin mesajlari
	if (messagetype == 0) {
		cout << "SUCCESS: " << shape << " moved from (" << before_row+1 << ","<< before_col+1 <<") to (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 1) {
		cout << "BORDER ERROR: " << shape << " can not be moved from (" << before_row+1 << ","<< before_col+1 <<") to (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 2) {
		cout << "CONFLICT ERROR: " << shape << " can not be moved from (" << before_row+1 << ","<< before_col+1 <<") to (" << row + 1 << "," << col + 1 << ")." << endl;
	}
	else if (messagetype == 3) {
		cout << "BORDER ERROR: " << shape << " can not be moved from (" << before_row+1 << ","<< before_col+1 <<") to (" << row + 1 << "," << col + 1 << ")." << endl;
		cout << "CONFLICT ERROR: " << shape << " can not be moved from (" << before_row+1 << ","<< before_col+1 <<") to (" << row + 1 << "," << col + 1 << ")." << endl;
	}
}

bool isBorder(Gridst *grid, int row, int col) {
	if (row < grid->row && col < grid->col && row >= 0 && col >= 0) {	//gridin disina tasip tasmama kontrolu
		return 0;
	}
	else {
		return 1;
	}

}

bool isConflict(Gridst *grid, int row, int col) {						//griddeki baska bi isaretle cakisip cakismama kontrolu
	if (grid->arr[row][col].type == ' ') {
		return 0;
	}
	else {
		return 1;
	}
}
//						 merkezrow,   col,     size,      shape
int Errors(Gridst *grid, int row, int col, int size, char shape,int Add_Move,int before_row,int before_col) {

	int flag_border = 0;					//border hatasi kontrolu
	int flag_conflict = 0;					//conflict hatasi kontrolu
	int messagetype;

	flag_border += isBorder(grid, row, col);
	flag_conflict += isConflict(grid, row, col);


	if (shape == '+') {

		for (int i = 1; i < size + 1; i++) {
			if (!isBorder(grid, row + i, col)) {
				flag_conflict += isConflict(grid, row + i, col);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row - i, col)) {
				flag_conflict += isConflict(grid, row - i, col);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row, col + i)) {
				flag_conflict += isConflict(grid, row, col + i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row, col - i)) {
				flag_conflict += isConflict(grid, row, col - i);
			}
			else {
				flag_border++;
			}
		}

	}

	else if (shape == '-') {
		for (int i = 1; i < size + 1; i++) {

			if (!isBorder(grid, row, col + i)) {
				flag_conflict += isConflict(grid, row, col + i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row, col - i)) {
				flag_conflict += isConflict(grid, row, col - i);
			}
			else {
				flag_border++;
			}
		}
	}

	else if (shape == 'x') {
		for (int i = 1; i < size + 1; i++) {
			if (!isBorder(grid, row + i, col + i)) {
				flag_conflict += isConflict(grid, row + i, col + i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row - i, col - i)) {
				flag_conflict += isConflict(grid, row - i, col - i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row - i, col + i)) {
				flag_conflict += isConflict(grid, row - i, col + i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row + i, col - i)) {
				flag_conflict += isConflict(grid, row + i, col - i);
			}
			else {
				flag_border++;
			}
		}
	}

	else if (shape == '/') {
		for (int i = 1; i < size + 1; i++) {
			if (!isBorder(grid, row - i, col + i)) {
				flag_conflict += isConflict(grid, row - i, col + i);
			}
			else {
				flag_border++;
			}
			if (!isBorder(grid, row + i, col - i)) {
				flag_conflict += isConflict(grid, row + i, col - i);
			}
			else {
				flag_border++;
			}
		}
	}
	//cout << flag_border << flag_conflict << endl;
	if (!Add_Move) {						//grid texttekileri ekliyorsak
		if (flag_border == 0 && flag_conflict == 0) {
			messagetype = 0;
			ErrorMessage1(messagetype, shape, size, row, col);
			return 0;
		}

		else if (flag_border > 0 && flag_conflict == 0) {
			messagetype = 1;
			ErrorMessage1(messagetype, shape, size, row, col);
			return 1;
		}

		else if (flag_border == 0 && flag_conflict > 0) {
			messagetype = 2;
			ErrorMessage1(messagetype, shape, size, row, col);
			return 1;
		}

		else if (flag_border > 0 && flag_conflict > 0) {
			messagetype = 3;
			ErrorMessage1(messagetype, shape, size, row, col);
			return 1;
		}
	}	

	else {									//move hareketi yapýyorsak
		if (flag_border == 0 && flag_conflict == 0) {
			messagetype = 0;
			ErrorMessage2(messagetype, shape, row, col,before_row,before_col);
			return 0;
		}

		else if (flag_border > 0 && flag_conflict == 0) {
			messagetype = 1;
			ErrorMessage2(messagetype, shape, row, col,before_row,before_col);
			return 1;
		}

		else if (flag_border == 0 && flag_conflict > 0) {
			messagetype = 2;
			ErrorMessage2(messagetype, shape, row, col,before_row,before_col);
			return 1;
		}

		else if (flag_border > 0 && flag_conflict > 0) {
			messagetype = 3;
			ErrorMessage2(messagetype, shape, row, col,before_row,before_col);
			return 1;
		}
	}
	return 0;
}

void InstFile(Gridst *grid, char *gridname) {
	grid->fptr2 = fopen(gridname, "r");
	char move_inst[4];
	while (!feof(grid->fptr2)) {
		int row_inst;
		int col_inst;
		int step;
		int row;
		int col;
		char shape;
		int size;
		int before_row;
		int before_col;
		fscanf(grid->fptr2, "%c%c%c %d %d %d\n", &move_inst[0], &move_inst[1], &move_inst[2], &row_inst, &col_inst, &step);
		row_inst--;
		col_inst--;

		row = grid->arr[row_inst][col_inst].row;
		col = grid->arr[row_inst][col_inst].col;
		shape = grid->arr[row_inst][col_inst].type;
		size = grid->arr[row_inst][col_inst].size;					//secilen hucredeki ifadenin merkez bilgilerini cagirma
		before_row = row;
		before_col = col;

		if (grid->arr[row_inst][col_inst].type == '+') {			//move yapmadan once hareket edecek olan ifadenin gridden silinmesi

			for (int i = row - size; i <= row + size; i++) {
				grid->arr[i][col].type = ' ';
				grid->arr[i][col].row = row;
				grid->arr[i][col].col = col;
				grid->arr[i][col].size = size;
			}
			for (int i = col - size; i <= col + size; i++) {
				grid->arr[row][i].type = ' ';
				grid->arr[row][i].row = row;
				grid->arr[row][i].col = col;
				grid->arr[row][i].size = size;
			}
		}
		else if (grid->arr[row_inst][col_inst].type == '-') {
			for (int i = col - size; i <= col + size; i++) {
				grid->arr[row][i].type = ' ';
				grid->arr[row][i].row = row;
				grid->arr[row][i].col = col;
				grid->arr[row][i].size = size;
			}
		}
		else if (grid->arr[row_inst][col_inst].type == 'x') {
			int j = col - size;
			for (int i = row - size; i <= row + size; i++) {
				grid->arr[i][j].type = ' ';
				grid->arr[i][j].row = row;
				grid->arr[i][j].col = col;
				grid->arr[i][j].size = size;
				j++;
			}
			j = col + size;
			for (int i = row - size; i <= row + size; i++) {
				grid->arr[i][j].type = ' ';
				grid->arr[i][j].row = row;
				grid->arr[i][j].col = col;
				grid->arr[i][j].size = size;
				j--;
			}
		}
		else if (grid->arr[row_inst][col_inst].type == '/') {
			int j = col + size;
			for (int i = row - size; i <= row + size; i++) {
				grid->arr[i][j].type = ' ';
				grid->arr[i][j].row = row;
				grid->arr[i][j].col = col;
				grid->arr[i][j].size = size;
				j--;
			}
		}

		//move 		

		if (move_inst[2] == 'L') {											//move yapilacak olan yonde merkezin kaydirilmasi

			col -= step;
		}
		else if (move_inst[2] == 'R') {
			col += step;
		}
		else if (move_inst[2] == 'D') {
			row += step;
		}
		else if (move_inst[2] == 'U') {
			row -= step;
		}

		if (!Errors(grid, row, col, size, shape, 1, before_row, before_col)) {				//hata yoksa move yapilan yere isaretin ilerletilmesi
			if (shape == '+') {
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][col].type = '+';
					grid->arr[i][col].row = row;
					grid->arr[i][col].col = col;
					grid->arr[i][col].size = size;
				}
				for (int i = col - size; i <= col + size; i++) {
					grid->arr[row][i].type = '+';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}

			else if (shape == '-') {

				for (int i = col - size; i <= col + size; i++) {
					grid->arr[row][i].type = '-';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}

			else if (shape == 'x') {
				int j = col - size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j++;
				}
				j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}

			else if (shape == '/') {
				int j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = '/';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}
		}

		else {																	//hata varsa merkez bilgilerinin eski yerine cekilip, ifadenin eski yerine tekrar yerlestirilmesi
			if (move_inst[2] == 'L') {
				col += step;
			}
			else if (move_inst[2] == 'R') {
				col -= step;
			}
			else if (move_inst[2] == 'D') {
				row -= step;
			}
			else if (move_inst[2] == 'U') {
				row += step;
			}

			if (shape == '+') {
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][col].type = '+';
					grid->arr[i][col].row = row;
					grid->arr[i][col].col = col;
					grid->arr[i][col].size = size;
				}
				for (int i = col - size; i <= col + size; i++) {
					grid->arr[row][i].type = '+';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}

			else if (shape == '-') {

				for (int i = col - size; i <= col + size; i++) {
					grid->arr[row][i].type = '-';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}

			else if (shape == 'x') {
				int j = col - size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j++;
				}
				j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}

			else if (shape == '/') {
				int j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = '/';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}
		}
	}
}

void AddtoGrid(Gridst *grid) {
	while (!feof(grid->fptr)) {
		int row;
		int col;
		int size;
		char shape;
		fscanf(grid->fptr, "%c %d %d %d\n", &shape, &row, &col, &size);
		row--; 
		col--; 
		if (!Errors(grid, row, col, size, shape,0,0,0)) {				//error kontrol yapip error cikmazsa grid texttekileri gride ekleme
			if (shape == '+') {												
				for (int i = row-size ; i<=row+size ; i++) {			//donguler icinde yerlestirdigim isaretlerin merkezlerinin bilgilerini tutmak icin bilgileri merkezlerine atadim
					grid->arr[i][col].type = '+';
					grid->arr[i][col].row = row;
					grid->arr[i][col].col = col;
					grid->arr[i][col].size = size;
				}
				for (int i = col - size ; i <= col + size ; i++) {
					grid->arr[row][i].type = '+';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}
			
			else if (shape == '-') {
				
				for (int i = col - size ; i <= col + size ; i++) {
					grid->arr[row][i].type = '-';
					grid->arr[row][i].row = row;
					grid->arr[row][i].col = col;
					grid->arr[row][i].size = size;
				}
			}
			
			else if (shape == 'x') {
				int j = col - size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j++;
				}
				j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = 'x';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}

			else if (shape == '/') {
				int j = col + size;
				for (int i = row - size; i <= row + size; i++) {
					grid->arr[i][j].type = '/';
					grid->arr[i][j].row = row;
					grid->arr[i][j].col = col;
					grid->arr[i][j].size = size;
					j--;
				}
			}
		}
	}
}

int main(int argc, char**argv) {

	Gridst MyGrid;
	GridFile(&MyGrid, argv[1]);							//grid textlerin okunmasi
	AddtoGrid(&MyGrid);		
	InstFile(&MyGrid, argv[2]);							//inst textlerin okunmasi	

	fclose(MyGrid.fptr);
	fclose(MyGrid.fptr2);

	for (int i = 0; i < MyGrid.row; i++) {
		delete[] MyGrid.arr[i];
	}
	delete[] MyGrid.arr;

	return 0;
}
