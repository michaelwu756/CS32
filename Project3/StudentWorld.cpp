#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"
#include "GameConstants.h"
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir), currentSubLevel(0), nextSubLevel(0), objectives(0), levelFinished(false), actorList(MAX_SUB_LEVEL), m_player(nullptr), gameTime(START_TIME)
{
	srand((int)time(0));
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}
int StudentWorld::init()
{
	gameTime = START_TIME;//initializes and resets member variables
	currentSubLevel = 0;
	nextSubLevel = 0;
	levelFinished = false;
	objectives = 0;

	if (getLevel() > 99)//checks if the player has completed all levels
		return GWSTATUS_PLAYER_WON;
	if (!loadLevel(getLevel()))//loads level, if it does not load it returns false
		return GWSTATUS_LEVEL_ERROR;
	changePlayer(currentSubLevel);//initializes pointer to player
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{	
	gameTime--;
	updateDisplay();
	vector<Actor*>::const_iterator it;
	for (it = actorList[currentSubLevel].begin(); it != actorList[currentSubLevel].end(); it++)
		{
			(*it)->doSomething();
			if (m_player->dead())//check	s after each call of doSomething() if the player is dead
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (levelFinished)//checks each doSomething() if level is finished
				return GWSTATUS_FINISHED_LEVEL;
		}
	pushNewActors();//adds any new actors created during this tick
	removeDead();//removes dead actors
	changeSubLevel();//changes sublevel if a gate is hit, otherwise does nothing
	if (objectives == 0)//objectives keeps track of number of robots, gold, and hostages
		showExit();
	
	if (gameTime <= 0)//checks if player is out of time
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()//deletes every actor and clears their pointers from the vectors
{
	vector<Actor*>::const_iterator it;
	for (int i = 0; i < MAX_SUB_LEVEL; i++)
		for (it = actorList[i].begin(); it != actorList[i].end(); it++)
			delete *it;
	for (it = actorsToAdd.begin(); it != actorsToAdd.end(); it++)
		delete *it;
	vector<vector<Actor*>>::iterator outerIt;
	for (outerIt = actorList.begin(); outerIt != actorList.end(); outerIt++)
		outerIt->clear();
	actorsToAdd.clear();
}
int StudentWorld::getCurrentSubLevel()
{
	return currentSubLevel;
}

void StudentWorld::addActor(Actor * a)
{
	switch (a->getID())//increases objective count when a hostage, gold, or robot is added
	{
		case IID_HOSTAGE:
		case IID_GOLD:
		case IID_ROBOT_BOSS:
			objectives++;
	}
	actorsToAdd.push_back(a);//pushes actors into a temporary storage container so they can be added after the current tick finishes
}
void StudentWorld::goToSubLevel(int lvl)
{
	nextSubLevel = lvl;//stores the next sublevel because sublevels change after tick is finished
}
void StudentWorld::finishLevel()
{
	increaseScore(gameTime);
	levelFinished = true;//changes boolean to check if level is finished, when it is true move() return GWSTATUS_LEVEL_FINISHED
}
void StudentWorld::increasePlayerAmmo()
{
	m_player->setAmmo(m_player->getAmmo() + AMMO_INCREASE);
}
void StudentWorld::restorePlayerHP()
{
	m_player->setHP(PLAYER_HP);
}
void StudentWorld::farplaneGunExplosion(int x, int y)
{
	m_player->damage(FARPLANE_GUN_DAMAGE);//decreases player hp
	vector<Actor*> actors = getActorsAt(x,y,FARPLANE_GUN_RANGE);//loops through actors in a 9x9 square around the farplane gun
	vector<Actor*>::const_iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
		switch ((*it)->getID())
		{
			case IID_GANGSTER:
			case IID_BULLY:
				(*it)->damage(GANGSTER_HP + BULLY_HP);//damages gangsters and bullies by both their hp's added together to make sure they die
		}
}
void StudentWorld::damageActors(int x, int y)//calls damage on every actor at x,y
{
	vector<Actor*> actors = getActorsAt(x, y);
	vector<Actor*>::const_iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
		(*it)->damage();//damage function does not do anything unless it is a bad guy or player
}
std::vector<int> StudentWorld::consumeGoodies(int x, int y)
{
	vector<int> goodies;
	vector<Actor*> actors = getActorsAt(x, y);
	vector<Actor*>::const_iterator it;
	for (it = actors.begin(); it != actors.end(); it++)//picks up all goodies in square during one tick
		switch ((*it)->getID())
		{
			case IID_AMMO:
			case IID_RESTORE_HEALTH:
			case IID_EXTRA_LIFE:
				goodies.push_back((*it)->getID());
				Consumable* goodie = dynamic_cast<Consumable*> (*it);
				goodie->setHP(0);
		}
	return goodies;
}

int StudentWorld::bullyCount(int x, int y, int d) const//counts bullies in a box extending distance d from x,y
{
	vector<Actor*> actors = getActorsAt(x,y,d);
	vector<Actor*>::const_iterator it;
	int bullyCount = 0;
	for (it = actors.begin(); it != actors.end(); it++)
		if ((*it)->getID() == IID_BULLY)
			bullyCount++;
	return bullyCount;
}
bool StudentWorld::moveBlocked(int x, int y) const//checks if movement is prevented onto a square x,y
{
	vector<Actor*> actors = getActorsAt(x, y);
	vector<Actor*>::const_iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
	{
		switch ((*it)->getID())
		{
			case IID_WALL:
			case IID_GANGSTER:
			case IID_BULLY:
			case IID_WATERPOOL:
			case IID_ROBOT_BOSS:
			case IID_BULLY_NEST:
			case IID_PLAYER:
				return true;
		}
	}
	return false;
}
bool StudentWorld::isPlayerAt(int x, int y) const
{
	if (m_player->getX() == x&&m_player->getY() == y)
		return true;
	return false;
}
bool StudentWorld::isFakeWallAt(int x, int y) const
{
	vector<Actor*> actors = getActorsAt(x, y);
	vector<Actor*>::const_iterator it;
	for (it = actors.begin(); it != actors.end(); it++)
		if ((*it)->getID() == IID_FAKE_WALL)
			return true;
	return false;
}
//private member functions
bool StudentWorld::loadLevel(int lvl)
{
	stringstream ss;
	ss << "level" << setfill('0')<<setw(2)<<lvl;
	string str = ss.str();
	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(str + ".dat");
	if (result == Level::load_fail_file_not_found || result == Level::load_fail_bad_format)
		return false;//if main level does not load, return false
	int i = 0;
	do//start loading sublevels starting from 1 to the max number of sublevels
	{
		i++;
		result = lev.loadLevel(str + "_" + to_string(i)+".dat", i);
		if (result == Level::load_sublevel_fail_bad_format)//if sublevel has bad format, return false
			return false;
	} while (result == Level::load_sublevel_success && i < MAX_SUB_LEVEL);//if the sublevel exists and loads correctly, load the next sublevel
	Level::MazeEntry item;//go through each sublevel and add actors to ActorList at the correct sublevel
	for (int subLevel=0;subLevel<i;subLevel++)
		for (int x = 0;x<VIEW_WIDTH;x++)
			for (int y = 0; y < VIEW_HEIGHT; y++)
			{
				item = lev.getContentsOf(x, y, subLevel);
				Actor* p=nullptr;
				switch (item)
				{
					case Level::wall:
						p = new Wall(x, y, subLevel);
						break;
					case Level::player:
						p = new Player(this, x, y, subLevel);
						break;
					case Level::fake_wall:
						p = new FakeWall(x, y, subLevel);
						break;
					case Level::exit:
						p = new Exit(this, x, y, subLevel);
						break;
					case Level::horiz_gangster:
						p = new Gangster(this, x, y, subLevel, GraphObject::right);
						break;
					case Level::vert_gangster:
						p = new Gangster(this, x, y, subLevel, GraphObject::down);
						break;
					case Level::bully_nest:
						p = new BullyNest(this, x, y, subLevel);
						break;
					case Level::hostage:
						objectives++;//hostages, gold, and robot bosses increase the remaining objective count
						p = new Hostage(this, x, y, subLevel);
						break;
					case Level::gold:
						objectives++;
						p = new Gold(this, x, y, subLevel);
						break;
					case Level:: restore_health:
						p = new RestoreHealthGoodie(this, x, y, subLevel);
						break;
					case Level::extra_life:
						p = new ExtraLifeGoodie(this, x, y, subLevel);
						break;
					case Level::ammo:
						p = new AmmoGoodie(this, x, y, subLevel);
						break;
					case Level::farplane_gun:
						p = new FarplaneGun(this, x, y, subLevel);
						break;
					case Level::robot_boss:
						objectives++;
						p = new RobotBoss(this, x, y, subLevel);
						break;
					case Level::gate0:
						p = new Gate(this, x, y, subLevel, 0);
						break;
					case Level::gate1:
						p = new Gate(this, x, y, subLevel, 1);
						break;
					case Level::gate2:
						p = new Gate(this, x, y, subLevel, 2);
						break;
					case Level::gate3:
						p = new Gate(this, x, y, subLevel, 3);
						break;
					case Level::gate4:
						p = new Gate(this, x, y, subLevel, 4);
						break;
					case Level::gate5:
						p = new Gate(this, x, y, subLevel, 5);
						break;
				}
				if (p!=nullptr)
					actorList[subLevel].push_back(p);
			}
	return true;
}
void StudentWorld::changePlayer(int subLevel)
{
	vector<Actor*>::const_iterator it;//changes player pointer to the player object in the next sublevel
	for (it = actorList[subLevel].begin(); it != actorList[subLevel].end(); it++)
	{
		if ((*it)->getID() == IID_PLAYER)
			m_player = dynamic_cast<Player*>(*it);
	}
}
void StudentWorld::updateDisplay()
{
	stringstream ss;//formats display text
	ss << "Score: " << setfill('0') << setw(7) << getScore()
		<< "  Level: " << setw(2) << getLevel() << "-" << setw(2) << getCurrentSubLevel()
		<< "  Lives: " << setfill(' ') << setw(2) << getLives()
		<< "  Health: " << setw(3) << (m_player->getHP() * 100) / 20
		<< "%  Ammo: " << setw(3) << m_player->getAmmo()
		<< "  Time Left: " << setw(4) << gameTime;
	setGameStatText(ss.str());
}
void StudentWorld::removeDead()
{
	vector<Actor*>::iterator it;//goes through current sublevel and deletes any dead actors, then erases the pointer from ActorList
	for (it = actorList[currentSubLevel].begin(); it != actorList[currentSubLevel].end();)
		if ((*it)->dead())
		{
			switch ((*it)->getID())//decreases objective count when a hostage, gold, or robot boss is deleted
			{
				case IID_HOSTAGE:
				case IID_GOLD:
				case IID_ROBOT_BOSS:
					objectives--;
			}
			delete *it;
			it = actorList[currentSubLevel].erase(it);
		}
		else
		{
			it++;
		}
}
void StudentWorld::pushNewActors()
{
	vector<Actor*>::const_iterator it;//pushes new actors to the front of the actor list at the current sublevel, to make sure bullets are animated first
	for (it = actorsToAdd.begin(); it != actorsToAdd.end(); it++)
		actorList[currentSubLevel].insert(actorList[currentSubLevel].begin(),*it);
	actorsToAdd.clear();//then clears the temporary actors to add list
}
void StudentWorld::changeSubLevel()
{
	if (currentSubLevel == nextSubLevel)//if the next sublevel is the same, don't do anything
		return;
	int hp = m_player->getHP();//otherwise store the player hp and ammo
	int ammo = m_player->getAmmo();
	changePlayer(nextSubLevel);//change player pointer to the player in the target sublevel
	m_player->setHP(hp);//update the player hp and ammo in the sublevel to match the previous hp and ammo
	m_player->setAmmo(ammo);
	currentSubLevel = nextSubLevel;//set current sublevel to next sublevel
}
void StudentWorld::showExit()
{
	vector<Actor*>::const_iterator it;//finds and reveals the exit
	for (it = actorList[0].begin(); it != actorList[0].end(); it++)
	{
		if ((*it)->getID() == IID_EXIT)
		{
			Exit* exit = dynamic_cast<Exit*>(*it);
			if(!exit->isRevealed())
				exit->reveal();
		}
	}
}

vector<Actor*> StudentWorld::getActorsAt(int x, int y, int d) const//retrieves actors in a box extending d squares from location x,y
{
	int lowX = x - d;
	int highX = x + d;
	int lowY = y - d;
	int highY = y + d;
	vector<Actor*> actors;
	vector<Actor*>::const_iterator it;
	for (it = actorList[currentSubLevel].begin(); it != actorList[currentSubLevel].end(); it++)
		if ((*it)->getX() >= lowX && (*it)->getX() <= highX && (*it)->getY() >= lowY && (*it)->getY() <= highY)
			actors.push_back(*it);
	return actors;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}