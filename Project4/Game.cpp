#include<string>
#include<vector>
#include "Game.h"
using namespace std;
Game::Game(const vector<string> &words):wordList(words),secretWord(""){}
Game::~Game(){}
bool Game::setSecretWord(const string &sw)
{
	vector<string>::const_iterator wordIter;
	for(wordIter = wordList.begin();wordIter!=wordList.end();wordIter++)
		if(sw==*wordIter)
		{
			secretWord = sw;
			return true;
		}
	return false;
}
int Game::secretWordLength() const
{
	return secretWord.length();
}
void Game::probe(const string &probeWord, int &nInRightPlace, int &nInWrongPlace)
{
	nInRightPlace = 0;
	nInWrongPlace = 0;
	string tempProbe = probeWord;
	string tempSecret = secretWord;
	int smallestSize = probeWord.length();
	if(secretWord.length()<probeWord.length())
		smallestSize=secretWord.length();
	for(int i=smallestSize-1;i>=0;i--)
		if(tempProbe[i]==tempSecret[i])
		{
			nInRightPlace++;
			tempSecret.erase(i,1);
			tempProbe.erase(i,1);
		}
	while(!tempProbe.empty() && !tempSecret.empty())
	{
		for(int i = 0; i<tempSecret.size();i++)
			if(tempProbe[0]==tempSecret[i])
			{	
				nInWrongPlace++;
				tempSecret.erase(i,1);
				break;
			}
		tempProbe.erase(0,1);
	}
}