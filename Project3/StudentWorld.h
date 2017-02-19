#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Player;
const int START_TIME = 3000;
class StudentWorld : public GameWorld
{
	public:
		StudentWorld(std::string assetDir);
		~StudentWorld();
		virtual int init();
		virtual int move();
		virtual void cleanUp();
		virtual int getCurrentSubLevel();

		void addActor(Actor* a);
		void goToSubLevel(int lvl);
		void finishLevel();
		void increasePlayerAmmo();
		void restorePlayerHP();
		void farplaneGunExplosion(int x, int y);
		void damageActors(int x, int y);
		std::vector<int> consumeGoodies(int x, int y);
		
		int bullyCount(int x, int y, int d) const;
		bool moveBlocked(int x, int y) const;
		bool isPlayerAt(int x, int y) const;
		bool isFakeWallAt(int x, int y) const;

	private:
		std::vector<std::vector<Actor*>> actorList;
		std::vector<Actor*> actorsToAdd;
		Player* m_player;
		bool levelFinished;
		int objectives;
		int currentSubLevel;
		int nextSubLevel;
		int gameTime;
		
		bool loadLevel(int lvl);
		void changePlayer(int subLevel);
		void updateDisplay();
		void removeDead();
		void pushNewActors();
		void changeSubLevel();
		void showExit();
		std::vector<Actor*> getActorsAt(int x, int y, int d=0) const;
};

#endif // STUDENTWORLD_H_