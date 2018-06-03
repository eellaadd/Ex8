#include "Board.h"
#include "Point.h"
#include <iostream>
using namespace std;

Point Point::operator=(const char c) {

	if (c == '.' || c == 'X' || c == 'O')
		data = c;
	else {
		IllegalCharException ex{ c };
		throw ex;
	}
	return *this;
}

Point& Point::operator=(const Point& p) {
	data = p.data;
	return *this;
}

Point::operator char() {
	return data;
}

bool operator == (Point const & l, char const & r) {
	return l.data == r;
}