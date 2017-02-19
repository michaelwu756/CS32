#ifndef PLAYER_H
#define PLAYER_H
#include<string>
#include<vector>
#include<list>
class Player
{
	public:
		Player(const std::vector<std::string> &words);
		std::string generateProbeWord();
		void learn(std::string probe, int nInRightPlace, int nInWrongPlace);
		~Player();
	private:		
		Player(const Player&);
		Player& operator=(const Player&);
		int getBulls(std::string first, std::string second);
		void getBullsAndCows(std::string first, std::string second, int &bulls, int &cows);
		std::list<std::string> wordList;
};
#endif