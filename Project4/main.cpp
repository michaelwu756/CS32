#include "Game.h"
#include "Player.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

const char* filename = "C:/Users/chees_000/Documents/Programs/CS32/Project4/wordlist.txt";
  // A Windows user might have the path be "C:/CS32/P4/wordlist.txt"
  // A Mac user might have it be "/Users/fred/CS32/P4/wordlist.txt"

int play(Game& g, Player &p)
{
	int secretLength = g.secretWordLength();
	int nTurns = 0;
	for(;;) 
	{
		int nInRightPlace, nInWrongPlace;
		string probe = p.generateProbeWord();
		
		g.probe(probe, nInRightPlace, nInWrongPlace);
		// repeat probes until word is guessed
		//cout<<probe<<" - "<<nInRightPlace<<" bulls, "<<nInWrongPlace<<" cows"<<endl;
		nTurns++;
		if (nInRightPlace == secretLength)
			break;
		p.learn(probe, nInRightPlace, nInWrongPlace);
	}
	
	cerr << "The Player probed " << nTurns << " times!" << endl;
	return nTurns;
}

int main()
{
	srand(time(0));
	ifstream wordlistfile(filename);
	if (!wordlistfile)
	{
		cerr << "Cannot open " << filename << "!" << endl;
		return 1;
	}
	vector<string> words;

	string w;
	while (wordlistfile >> w)
		words.push_back(w);
	int probes = 0;
	for(int k =0; k<100;k++)
	{
		Player player(words);
		Game g(words);
		int j = rand();
		int i = (j*(j+3))%words.size();
		//cout<<j<<" "<<i<<endl;
		string secretWord = words[i];
		cout<<secretWord<<endl;
		if (!g.setSecretWord(secretWord))  // Supply the secret word
		{
			cerr << "Secret word is not in word list!" << endl;
			return 1;
		}
		
		 probes +=play(g,player);
	}
	cout<<probes<<endl;
}
