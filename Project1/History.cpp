#include <iostream>
#include "History.h"
using namespace std;

History::History(int nRows, int nCols)
{
	for (int i=0;i<MAXROWS;i++)//fill grid with -1 where it is out of bounds, 0 everywhere else
		for (int j=0;j<MAXCOLS;j++)
		{	
			if (i<nRows && j<nCols)
				moveCounter[i][j]=0;
			else
				moveCounter[i][j]= -1;
		}
	rows=nRows;
	cols=nCols;
}
void History::display() const
{   // Position (row,col) in the pit coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid
    for (r = 0; r <MAXROWS; r++)
        for (c = 0; c < MAXCOLS; c++)
			switch(moveCounter[r][c])
			{
				case -1:
					grid[r][c]=' ';
					break;
				case 0:
					grid[r][c]='.';
					break;
				case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:
				case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 20:
				case 21:case 22:case 23:case 24:case 25:
					grid[r][c]=64+moveCounter[r][c];
					break;
				default:
					grid[r][c]='Z';
			}
        // Draw the grid
    clearScreen();
    for (r = 0; r <rows; r++)
    {
        for (c = 0; c <cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}
bool History::record(int r, int c)
{
	if (r<1 || r>MAXROWS || c<1 || c>MAXCOLS)
		return false;
	else if (moveCounter[r-1][c-1]==-1)//don't record if out of grid
		return false;
	else
		moveCounter[r-1][c-1]++;
	return true;
}