#ifndef HISTORY_H
#define HISTORY_H
#include "globals.h"
class History
{
	public:
		//constructor
		History(int nRows, int nCols);
		
		//accessor
		void display() const;
		
		//mutator
		bool record(int r, int c);
		
	private:
		int moveCounter[MAXROWS][MAXCOLS];
		int rows;
		int cols;
};
#endif
