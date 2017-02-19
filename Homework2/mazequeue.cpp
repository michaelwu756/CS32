#include <queue>
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
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc]='X';
	while(!coordQueue.empty())
	{
		Coord front=coordQueue.front();
		coordQueue.pop();
		if(front.r()==er && front.c()==ec)
			return true;
		else
		{
			if(front.r()-1>=0)
				if(maze[front.r()-1][front.c()]=='.')
				{
					coordQueue.push(Coord(front.r()-1,front.c()));
					maze[front.r()-1][front.c()]='X';
				}
			if(front.c()+1<nCols)
				if(maze[front.r()][front.c()+1]=='.')
				{
					coordQueue.push(Coord(front.r(),front.c()+1));
					maze[front.r()][front.c()+1]='X';
				}
			if(front.r()+1<nRows)
				if(maze[front.r()+1][front.c()]=='.')
				{
					coordQueue.push(Coord(front.r()+1,front.c()));
					maze[front.r()+1][front.c()]='X';
				}
			if(front.c()-1>=0)
				if(maze[front.r()][front.c()-1]=='.')
				{
					coordQueue.push(Coord(front.r(),front.c()-1));
					maze[front.r()][front.c()-1]='X';
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