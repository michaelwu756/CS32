#include <stack>
#include <string>
using namespace std;
class Coord
{
	public:
		Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
		int r() const { return m_r; }
		int c() const { return m_c; }
	private:
		int m_r;
		int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	maze[sr][sc]='X';
	while(!coordStack.empty())
	{
		Coord top=coordStack.top();
		coordStack.pop();
		if(top.r()==er && top.c()==ec)
			return true;
		else
		{
			if(top.r()-1>=0)
				if(maze[top.r()-1][top.c()]=='.')
				{
					coordStack.push(Coord(top.r()-1,top.c()));
					maze[top.r()-1][top.c()]='X';
				}
			if(top.c()+1<nCols)
				if(maze[top.r()][top.c()+1]=='.')
				{
					coordStack.push(Coord(top.r(),top.c()+1));
					maze[top.r()][top.c()+1]='X';
				}
			if(top.r()+1<nRows)
				if(maze[top.r()+1][top.c()]=='.')
				{
					coordStack.push(Coord(top.r()+1,top.c()));
					maze[top.r()+1][top.c()]='X';
				}
			if(top.c()-1>=0)
				if(maze[top.r()][top.c()-1]=='.')
				{
					coordStack.push(Coord(top.r(),top.c()-1));
					maze[top.r()][top.c()-1]='X';
				}
		}
	}
	return false;
}

/*
int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X.......@X",
		"XX@X@@.XXX",
		"X..X.X...X",
		"X..X...@.X",
		"X....XXX.X",
		"X@X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10,10, 6,4, 1,1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}
*/