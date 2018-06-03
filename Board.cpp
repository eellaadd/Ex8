#include "Board.h"
#include "Point.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sys/stat.h>
#include <unistd.h>
#include <string>
using namespace std;


string Board::draw(int imgSize) {
	int i = 0;
	string filename = "";
	filename += to_string(i);
	filename += ".ppm";


	while (fileExists(filename)) {
		i++;
		filename.clear();
		filename = "";
		filename += to_string(i);
		filename += ".ppm";
	}

	ofstream img(filename);

	int **finalMat = new int*[imgSize];
	for (int i = 0; i < imgSize; ++i) {
		finalMat[i] = new int[imgSize];
	}

	createBoard(finalMat, imgSize);

	img << "P3" << endl;
	img << imgSize << " " << imgSize << endl;
	img << 255 << endl;

	for (int y = 0; y < imgSize; y++) {
		for (int x = 0; x < imgSize; x++) {
			if (finalMat[y][x] == 1)
				img << "0 0 0" << endl;
			else if (finalMat[y][x] == 2)
				img << "29 29 189" << endl;
			else if (finalMat[y][x] == 3)
				img << "29 189 61" << endl;
			else
				img << "255 255 255" << endl;
		}
	}

	return filename;
}

void Board::createBoard(int** finalMat, int imgSize) {
	int gSize = imgSize / size;
	int** matX = makeX(imgSize);
	int** matO = makeO(imgSize);
	int** matWhite = makeWhite(imgSize);


	for (int i = 0; i < size; i++) { /// Go over grids

		for (int j = 0; j < size; j++) {

			for (int y = 0; y < gSize; y++) { /// Go over rows

				for (int x = 0; x < gSize; x++) { /// Go over columns
					if (pBoard[i][j].data == 'X') {
						if (matX[y][x] == 1) {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 1;
						}
						else if (matX[y][x] == 2) {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 2;
						}
						else {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 0;
						}
					}
					else if (pBoard[i][j].data == 'O') {
						if (matO[y][x] == 1) {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 1;
						}
						else if (matO[y][x] == 3) {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 3;
						}
						else {
							finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 0;
						}
					}
					else if (pBoard[i][j].data == '.') {
						finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 0;
					}
					if (y == gSize - 1 || x == gSize - 1 || y == 0 || x == 0) {
						finalMat[(y + i * gSize) % imgSize][(x + j * gSize) % imgSize] = 1;
					}
				}
			}
		}
	}
}

int** Board::makeX(int imgSize) {

	// Create matrix to contain pixels of X img
	int matSize = imgSize / size;
	int **mat = new int*[matSize];
	for (int i = 0; i < matSize; ++i) {
		mat[i] = new int[matSize];
	}

	int diff = matSize / 30;
	int margin = (int)(matSize / 7);

	// Fill matrix
	for (int y = margin; y < matSize - margin; y++) {
		for (int x = margin; x < matSize - margin; x++) {
			if (y <= x + diff && y >= x - diff) {
				mat[y][x] = 2;
			}
			else if (y <= matSize - x + diff && y >= matSize - x - diff) {
				mat[y][x] = 2;
			}
			else {
				mat[y][x] = 0;
			}
		}
	}

	return mat;
}

int** Board::makeO(int imgSize) {

	// Create matrix to contain pixels of O img
	int matSize = imgSize / size;
	int **mat = new int*[matSize];
	for (int i = 0; i < matSize; ++i) {
		mat[i] = new int[matSize];
	}

	int diff = matSize / 40;
	int margin = (int)(matSize / 8);
	int radius = matSize / 2 - margin;

	// Fill matrix
	for (int y = 0; y < matSize; y++) {
		for (int x = 0; x < matSize; x++) {
			if (floor(sqrt((x - matSize / 2)*(x - matSize / 2) + (y - matSize / 2)*(y - matSize / 2))) >= radius - diff
				&& floor(sqrt((x - matSize / 2)*(x - matSize / 2) + (y - matSize / 2)*(y - matSize / 2))) <= radius + diff) {
				mat[y][x] = 3;
			}
			else {
				mat[y][x] = 0;
			}
		}
	}
	return mat;

}

int** Board::makeWhite(int imgSize) {

	// Create matrix to contain white square
	int matSize = imgSize / size;
	int **mat = new int*[matSize];
	for (int i = 0; i < matSize; ++i) {
		mat[i] = new int[matSize];
	}

	// Fill matrix
	for (int y = 0; y < matSize; y++) {
		for (int x = 0; x < matSize; x++) {
			mat[y][x] = 0;
		}
	}
	return mat;
}

bool fileExists(const string& filename) {
	ifstream f(filename.c_str());
	return f.good();
}

Board& Board::operator=(const char w) {
	if (w == '.' || w == 'O' || w == 'X') {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				pBoard[i][j].data = w;
			}
		}
	}
	else {
		throw IllegalCharException(w);
	}
	return *this;
}

Board& Board::operator=(const Board& b) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			pBoard[i][j].data = b.pBoard[i][j].data;
		}
	}
	return *this;
}

bool Board::operator == (Board const & r) {
	if (size != r.size) return false;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (pBoard[i][j] != r.pBoard[i][j]) return false;
		}
	}
	return true;
}


ostream& operator<<(ostream& os, Board const& b) {
	for (int i = 0; i < b.size; i++) {
		for (int j = 0; j<b.size; j++) {
			os << b.pBoard[i][j].data;
		}
		os << endl;
	}
	return os;
}

istream& operator>>(istream & is, Board& b) {
	string s;
	is >> s;
	int nsize = s.length();

	Board temp(nsize);
	b.size = nsize;
	// b.pBoard = temp.pBoard;
	b.pBoard = temp.pBoard;

	for (int i = 0; i < nsize; i++) {
		b.pBoard[0][i].data = s[i];
	}
	for (int j = 1; j < nsize; j++) {
		is >> s;
		for (int i = 0; i < nsize; i++) {
			b.pBoard[j][i].data = s[i];
		}
	}
	return is;
}

Point& Board::operator[](Point p) {
	if (p.x < 0 || p.x >= size || p.y < 0 || p.y >= size) {
		throw IllegalCoordinateException(p.x, p.y);
	}
	else {
		return pBoard[p.x][p.y];
	}

}

const Point& Board::operator[](Point p) const {
	if (p.x < 0 || p.x >= size || p.y < 0 || p.y >= size) {
		throw IllegalCoordinateException(p.x, p.y);
	}
	else {
		return pBoard[p.x][p.y];
	}

}

void Board::clear() {
	for (int i = 0; i < size; i++)
		delete[] pBoard[i];
	delete[] pBoard;
}

Board::~Board() {
	// 	clear();
}