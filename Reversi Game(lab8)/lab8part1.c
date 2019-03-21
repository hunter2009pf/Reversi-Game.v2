#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define true 1
#define false 0
typedef int bool;
int N;
char step[4], humanStep[3];
char computerRole, whoseTurn = 'B';
static char board[26][26];
int dx[8] = { 1,0,-1,0,1,1,-1,-1 };
int dy[8] = { 0,1,0,-1,1,-1,1,-1 };
typedef struct Node {
	int r, c;
	struct Node *Next;
} Node;
Node* creatNode(Node* rt, int row, int col) {
	rt = (Node*)malloc(sizeof(Node));
	rt->r = row;
	rt->c = col;
	rt->Next = NULL;
	return rt;
}
void initializeBoard(char board[][26], int n) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			board[i][j] = 'U';
	int mid = n / 2;
	board[mid - 1][mid - 1] = board[mid][mid] = 'W';
	board[mid - 1][mid] = board[mid][mid - 1] = 'B';
}
void printfBoard(char board[][26], int n) {
	printf(" ");
	for (int i = 0; i < n; i++)
		printf("%c", (char)(97 + i));
	printf("\n");
	for (int i = 0; i < n; i++) {
		printf("%c", (char)(97 + i));
		for (int j = 0; j < n; j++)
			printf("%c", board[i][j]);
		printf("\n");
	}
}
bool positionInBounds(int n, int row, int col) {
	return row >= 0 && row < n && col >= 0 && col < n;
}
void updateEightDirections(char color, int row, int col) {
	for (int i = 0; i < 8; i++) {
		int tempRow = row + dx[i];
		int tempCol = col + dy[i];
		Node *First = NULL;
		First = creatNode(First, row, col);
		Node *cur = First;
		bool updated = false;
		while (positionInBounds(N, tempRow, tempCol)) {
			if (board[tempRow][tempCol] == color) {
				updated = true; break;
			}
			else if (board[tempRow][tempCol] == 'U')break;
			else {
				cur->Next = creatNode(cur->Next, tempRow, tempCol);
				cur = cur->Next;
				tempRow += dx[i];
				tempCol += dy[i];
			}
		}
		if (updated) {
			while (First->Next != NULL) {
				First = First->Next;
				board[First->r][First->c] = color;
			}
		}
	}
}
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol,int* cnt) {
	bool foundAnother = false, foundColor = false;
	int tempRow = row + deltaRow;
	int tempCol = col + deltaCol;
	int ccnt = 0;
	while (positionInBounds(n, tempRow, tempCol)) {
		if (board[tempRow][tempCol] == 'U')break;
		else if (board[tempRow][tempCol] == colour) {
			foundColor = true;
			break;
		}
		else {
			foundAnother = true;
			ccnt++;
			tempRow += deltaRow;
			tempCol += deltaCol;
		}
	}
	if (foundAnother && foundColor) { (*cnt) += ccnt; return true; }
	return false;
}
Node* getAllAvailableMoves(char board[][26], char colour, int n,char* x,char* y) {
	int maxVal = 0;
	Node *root = NULL;
	root = creatNode(root, -1, -1);
	Node *temp = root;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			if (board[i][j] != 'U')continue;
			int tempVal = 0;
			int* ptrVal = &tempVal;
			bool isOk = false;
			for (int k = 0; k < 8; k++)
				if (checkLegalInDirection(board, n, i, j, colour, dx[k], dy[k],ptrVal)) {
					isOk = true;
				}
			if (isOk) {
				temp->Next = creatNode(temp->Next, i, j);
				if (tempVal > maxVal) {
					*x= (char)(temp->Next->r+97);
					*y = (char)(temp->Next->c + 97);
					maxVal = tempVal;
				}
				temp = temp->Next;
			}
		}
	return root->Next;
}
bool isValidMove(Node* legalMoves, int row, int col) {
	Node* temp = legalMoves;
	while (temp != NULL) {
		if (temp->r == row && temp->c == col)return true;
		temp = temp->Next;
	}
	return false;
}
void togglePlayer() {
	if (whoseTurn == 'W')whoseTurn = 'B';
	else whoseTurn = 'W';
}
int countTiles() {
	int wNum = 0, bNum = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] == 'W')wNum++;
			else if (board[i][j] == 'B')bNum++;
	if (wNum == bNum)return 0;
	else if (wNum > bNum)return 1;
	else return -1;
}

int main() {
	printf("Enter the board dimension: ");
	scanf_s("%d", &N);
	printf("Computer plays (B/W): ");
	char x;
	scanf_s("%c", &x, 1);
	scanf_s("%c", &computerRole, 1);
	initializeBoard(board, N);
	printfBoard(board, N);
	
	while (1) {         
		char wr = NULL, wc = NULL;
		char* ptrwr = &wr, *ptrwc = &wc;                                                                 //available moves for white
		Node *wwMoves = getAllAvailableMoves(board, 'W', N,ptrwr,ptrwc);
		
		char br = NULL, bc = NULL;
		char* ptrbr = &br, *ptrbc = &bc;
		Node *bbMoves = getAllAvailableMoves(board, 'B', N,ptrbr,ptrbc);                    //available moves for black 

		if (wwMoves == NULL && bbMoves == NULL) {
			int result = countTiles();
			if (result == 0)printf("Draw!\n");
			else if (result == 1)printf("W player wins.\n");
			else printf("B player wins.\n");
			break;
		}
		if (whoseTurn == 'W' && wwMoves == NULL) {
			printf("W player has no valid move.\n");
			togglePlayer();
			continue;
		}
		else if (whoseTurn == 'B' && bbMoves == NULL) {
			printf("B player has no valid move.\n");
			togglePlayer();
			continue;
		}
		if (whoseTurn == computerRole) {
			printf("Computer places %c at %c%c.\n", computerRole, whoseTurn == 'W' ? wr : br, whoseTurn == 'W' ? wc : bc);
			char xtemp = (computerRole == 'W' ? wr : br);
			char ytemp = (computerRole == 'W' ? wc : bc);
			board[xtemp-97][ytemp-97] = computerRole;
			updateEightDirections(computerRole, xtemp-97, ytemp-97);
			printfBoard(board, N);
			togglePlayer();
			continue;
		}
		printf("Enter move for colour %c (RowCol): ", whoseTurn);
		scanf_s("%s", humanStep, 3);

		int temp1 = humanStep[0] - 97;
		int temp2 = humanStep[1] - 97;
		if (board[temp1][temp2] != 'U') {
			printf("This tile has been already occupied.\n");
			printfBoard(board, N);
			continue;
		}
		if (computerRole == 'B') {
			if (isValidMove(wwMoves, temp1, temp2)) {
				board[temp1][temp2] = 'W';
				updateEightDirections('W', temp1, temp2);
			}
			else {
				printf("Invalid move.\n");
				int result = countTiles();
				if (result == 0)printf("Draw!\n");
				else if (result == 1)printf("W player wins.\n");
				else printf("B player wins.\n");
				break;
			}
		}else{
			if (isValidMove(bbMoves, temp1, temp2)) {
				board[temp1][temp2] = 'B';
				updateEightDirections('B', temp1, temp2);
			}
			else {
				printf("Invalid move.\n");
				int result = countTiles();
				if (result == 0)printf("Draw!\n");
				else if (result == 1)printf("W player wins.\n");
				else printf("B player wins.\n");
				break;
			}
		}
		togglePlayer();
		printfBoard(board, N);
	}
	getchar();
	getchar();
	return 0;
}