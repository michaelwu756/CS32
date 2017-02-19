#ifndef GAME_H
#define GAME_H
#include<string>
#include<vector>
class Game
{
	public:	
		Game(const std::vector<std::string> &words);
		bool setSecretWord(const std::string &sw);
		int secretWordLength() const;
		void probe(const std::string &probeWord, int &nInRightPlace, int &nInWrongPlace);
		~Game();
	private:		
		Game(const Game&);
		Game& operator=(const Game&);
		std::vector<std::string> wordList;
		std::string secretWord;
};
#endif