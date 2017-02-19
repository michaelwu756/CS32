#include<string>
#include<vector>
#include<cstdlib>
#include<ctime>
#include "Player.h"
using namespace std;
Player::Player(const vector<string> &words)
{
	srand(time(0));
	for(int i=0;i<words.size();i++)
		wordList.push_back(words[i]);
}
Player::~Player(){}
string Player::generateProbeWord()
{
	int index = (rand()*(rand()+3))%wordList.size();//for even more randomness
	list<string>::const_iterator it = wordList.begin();
	for(int i=0;i<index;i++)
		it++;
	return *it;
}
void Player::learn(string probe, int nInRightPlace, int nInWrongPlace)
{
	int initialSize = wordList.size();
	list<string>::iterator it;
	for(it = wordList.begin();it!=wordList.end();)
	{
		int bulls;
		int cows;
		if(probe == *it || getBulls(probe,*it)!=nInRightPlace)
			it=wordList.erase(it);
		else if (initialSize<5000)//do not start checking for cows until the word list is small enough, for performance
		{
			getBullsAndCows(probe,*it,bulls,cows);
			if(bulls!=nInRightPlace&&cows!=nInWrongPlace)
				it=wordList.erase(it);
			else
				it++;
		}
		else it++;
	}
}
int Player::getBulls(string first, string second)
{
	int bulls=0;
	int smallestSize = first.length();
	if(second.length()<first.length())
		smallestSize=second.length();
	for(int i=0;i<smallestSize;i++)
		if(first[i]==second[i])
			bulls++;
	return bulls;
}
void Player::getBullsAndCows(string first, string second, int&bulls,int&cows)
{
	cows = 0;
	bulls = 0;
	int smallestSize = first.length();
	if(second.length()<first.length())
		smallestSize=second.length();
	for(int i=smallestSize-1;i>=0;i--)
		if(first[i]==second[i])
		{
			bulls++;
			second.erase(i,1);
			first.erase(i,1);
		}
	while(!first.empty() && !second.empty())
	{
		for(int i = 0; i<second.size();i++)
			if(first[0]==second[i])
			{	
				cows++;
				second.erase(i,1);
				break;
			}
		first.erase(0,1);
	}
}