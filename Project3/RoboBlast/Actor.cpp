#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;

Actor::Actor(int imageID, int startX, int startY, Direction dir, int sub_level)
	: GraphObject(imageID,startX,startY,dir,sub_level)
{
	setVisible(true);//actors are visible by default
}
void Actor::doSomething(){
	if (dead())//actors do nothing if they are dead
		return;
	uniqueAction();//calls a virtual private member function that base classes can override
}
void Actor::damage(int dmg) {}//each actor can be damaged, by default it does nothing
bool Actor::dead() const//actors are alive by default
{
	return false;
}
void Actor::uniqueAction() {}//actors do nothing by default

LiveActor::LiveActor(StudentWorld* w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int score)
	: Actor(imageID, startX, startY, dir, sub_level), hp(hitPoints), world(w), m_score(score) {}//live actors have hp and can interact with the world they are in, so everything other than a wall or fake wall
void LiveActor::setHP(int val)
{
	hp = val;
}
int LiveActor::getHP() const
{
	return hp;
}
int LiveActor::getScore() const//because badguys and consumables are live actors and they both have scores, every live actor has a score
{
	return m_score;
}
void LiveActor::moveCoordForward(int & x, int & y) const//moves x and y one position forward based on the direction the actor is facing
{
	switch (getDirection())
	{
		case right:
			x++;
			break;
		case down:
			y--;
			break;
		case left:
			x--;
			break;
		case up:
			y++;
			break;
	}
}
StudentWorld * LiveActor::getWorld() const
{
	return world;
}
bool LiveActor::dead() const
{
	return hp <= 0;
}

Consumable::Consumable(StudentWorld * w, int imageID, int startX, int startY, int sub_level, int score, int sound)
	: LiveActor(w, imageID, startX, startY, none, sub_level, 1, score), m_sound(sound) {}
bool Consumable::checkPlayer()//checks if player is on the square that the consumable is on
{
	if (getWorld()->isPlayerAt(getX(), getY()))
	{
		getWorld()->playSound(m_sound);//plays the consumable sound
		getWorld()->increaseScore(getScore());//increases score
		setHP(0);//sets the consumable to dead
		return true;//to indicate the consumable was taken
	}
	return false;
}
void Consumable::uniqueAction()
{
	if (checkPlayer())
		consumeEffect();//calls a virtual private member function that derived classes can override
}
void Consumable::consumeEffect() const {}

ShootingActor::ShootingActor(StudentWorld* w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int deathSound, int hitSound, int score)
	: LiveActor(w, imageID, startX, startY, dir, sub_level, hitPoints, score), m_deathSound(deathSound), m_hitSound(hitSound) {}//players and bad guys can shoot, so this is their base class
void ShootingActor::fire() const
{
	int x = getX();
	int y = getY();
	moveCoordForward(x, y);//get coordinate in front of actor, based on direction
	Actor* a = new Bullet(getWorld(), x, y, getDirection(), getWorld()->getCurrentSubLevel());//creates bullet at that location
	getWorld()->addActor(a);//adds the bullet to studentWorld's actors to add list
}
void ShootingActor::damage(int dmg)
{
	setHP(getHP() - dmg);//lowers hp by dmg, default value is BULLET_DAMAGE
	if (dead())
	{
		getWorld()->playSound(m_deathSound);
		deathBehaviour();//calls virtual private member function, because each shooting actor can do something different when it dies
	}
	else
		getWorld()->playSound(m_hitSound);
}
void ShootingActor::deathBehaviour() const {}//default death behaviour is nothing

BadGuy::BadGuy(StudentWorld * w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int score)
	: ShootingActor(w, imageID, startX, startY, dir, sub_level, hitPoints, SOUND_ENEMY_DIE, SOUND_ENEMY_FIRE, score), tickCount(0)
{
	ticks = (28 - getWorld()->getLevel()) / 4;//initializes how frequently bad guys act
	if (ticks < MIN_TICKS)
		ticks = MIN_TICKS;
}
void BadGuy::addTick()
{
	tickCount++;
	if (tickCount >= ticks)
		tickCount = 0;
}
bool BadGuy::resting() const//only acts when tickCount is 0, otherwise the bad guy rests
{
	if (tickCount != 0)
		return true;
	return false;
}
bool BadGuy::shootsPlayer() const//calls an overloaded function to see if the bad guy can shoot the player in the square in front of the bad guy
{
	int x = getX();
	int y = getY();
	moveCoordForward(x, y);
	return shootsPlayer(x, y);
}
bool BadGuy::shootsPlayer(int x, int y) const
{
	if (getWorld()->isFakeWallAt(x, y))//the fake wall hides the player, so the bad guy does not shoot
		return false;
	if (getWorld()->isPlayerAt(x, y))//the player is found
	{
		fire();//shoot and return true;
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return true;
	}
	if (getWorld()->moveBlocked(x, y))//other actors break the line of sight, so the bad guy does not shoot
		return false;
	moveCoordForward(x, y);//otherwise the bad guy can see the next coordinate forwards
	return shootsPlayer(x, y);//recursively check the next block until line of sight is broken
}
void BadGuy::uniqueAction()
{
	addTick();//bad guys always increase their tick count
	if (!resting())
		if (!shootsPlayer())
			moveBehaviour();//calls a virtual private member function to move if the bad guy is not resting and does not shoot
}
void BadGuy::moveBehaviour()
{
	int x = getX();
	int y = getY();
	moveCoordForward(x, y);//default move behaviour is to try to move forward one coordinate
	if (getWorld()->moveBlocked(x, y))//if it cannot move, reverse direction
		switch (getDirection())
		{
			case up:
				setDirection(down);
				break;
			case down:
				setDirection(up);
				break;
			case right:
				setDirection(left);
				break;
			case left:
				setDirection(right);
				break;
		}
	else
		moveTo(x, y);//otherwise move forward
}
void BadGuy::deathBehaviour() const
{
	dropObject();//all bad guys drop objects when they die, this calls a pure virtual private member fuction that they can override
	getWorld()->increaseScore(getScore());
}

Wall::Wall(int startX, int startY, int sub_level)//walls do nothing
	: Actor(IID_WALL, startX, startY, none, sub_level) {}

FakeWall::FakeWall(int startX, int startY, int sub_level)//fake walls do nothing
	: Actor(IID_FAKE_WALL, startX, startY, none, sub_level) {}

Waterpool::Waterpool(StudentWorld * w, int startX, int startY, int sub_level)
	: LiveActor(w, IID_WATERPOOL, startX, startY, none, sub_level, WATERPOOL_HP) {}
void Waterpool::uniqueAction()//waterpools do nothing except decrease their hp every tick
{
	setHP(getHP() - 1);
}

BullyNest::BullyNest(StudentWorld * w, int startX, int startY, int sub_level)
	: LiveActor(w, IID_BULLY_NEST, startX, startY, none, sub_level, 1) {}
void BullyNest::uniqueAction()
{
	if (getWorld()->bullyCount(getX(), getY(), BULLY_NEST_RANGE) < 3 && getWorld()->bullyCount(getX(), getY(), 0) == 0)//checks if there are less than 3 bullies around the nest and no bullies on the nest
	{
		int bullyGen = rand() % BULLY_NEST_GEN_CHANCE;//generates a random number from 0 to BULLY_NEST_GEN_CHANCE-1
		if (bullyGen == 0)//if the random number is 0
		{
			getWorld()->playSound(SOUND_BULLY_BORN);//create a bully and add it to the studentWorld
			Bully* newBully = new Bully(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());
			getWorld()->addActor(newBully);
		}
	}
}

Bullet::Bullet(StudentWorld* w, int startX, int startY, Direction dir, int sub_level)
	: LiveActor(w, IID_BULLET, startX, startY, dir, sub_level, 1) {}
bool Bullet::checkHit()//if the bullet hits something, it sets itself to dead and damages everything on that coordinate
{
	if (getWorld()->moveBlocked(getX(), getY()))
	{
		getWorld()->damageActors(getX(), getY());
		setHP(0);
		return true;
	}	
	return false;
}
void Bullet::uniqueAction()
{
	if (checkHit())//does nothing if it is already on something
		return;
	int x = getX();
	int y = getY();
	moveCoordForward(x, y);
	moveTo(x, y);//moves forward and checks again to see if it hits something
	checkHit();
}

Gold::Gold(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_GOLD, startX, startY, sub_level, GOLD_SCORE, SOUND_GOT_GOLD) {}//gold does the default consumable behaviour

Hostage::Hostage(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_HOSTAGE, startX, startY, sub_level, HOSTAGE_SCORE, SOUND_GOT_GOODIE) {}//hostages do the default consumable behaviour

AmmoGoodie::AmmoGoodie(StudentWorld* w, int startX, int startY, int sub_level)
	: Consumable(w, IID_AMMO, startX, startY, sub_level, AMMO_SCORE, SOUND_GOT_GOODIE) {}
void AmmoGoodie::consumeEffect() const//adds ammo to the player
{
	getWorld()->increasePlayerAmmo();
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_EXTRA_LIFE, startX, startY, sub_level, EXTRA_LIFE_SCORE, SOUND_GOT_GOODIE) {}
void ExtraLifeGoodie::consumeEffect() const//gives extra life to studentWorld
{
	getWorld()->incLives();
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_RESTORE_HEALTH, startX, startY, sub_level, RESTORE_HEALTH_SCORE, SOUND_GOT_GOODIE) {}
void RestoreHealthGoodie::consumeEffect() const//restores the player's hp to full
{
	getWorld()->restorePlayerHP();
}

FarplaneGun::FarplaneGun(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_FARPLANE_GUN, startX, startY, sub_level, FARPLANE_GUN_SCORE, SOUND_GOT_FARPLANE_GUN) {}
void FarplaneGun::consumeEffect() const//creates an explosion centered around the farplane gun's location
{
	getWorld()->farplaneGunExplosion(getX(), getY());
}

Gate::Gate(StudentWorld * w, int startX, int startY, int sub_level, int target)
	: Consumable(w, IID_GATE, startX, startY, sub_level, GATE_SCORE, SOUND_NONE), m_target(target) {}
void Gate::consumeEffect() const//changes the sublevel
{
	getWorld()->goToSubLevel(m_target);
}

Exit::Exit(StudentWorld * w, int startX, int startY, int sub_level)
	: Consumable(w, IID_EXIT, startX, startY, sub_level, EXIT_SCORE, SOUND_FINISHED_LEVEL), revealed(false)
{
	setVisible(false);
}
void Exit::doSomething()//exit does nothing if it is not visible
{
	if (isRevealed())
		Actor::doSomething();
}
void Exit::reveal()//reveal, called by studentWorld when no objectives are left
{
	setVisible(true);
	revealed = true;
	getWorld()->playSound(SOUND_REVEAL_EXIT);
}
bool Exit::isRevealed() const
{
	return revealed;
}
void Exit::consumeEffect() const//tells studentWorld the player has stepped on exit
{
	getWorld()->finishLevel();
}

Player::Player(StudentWorld* w, int startX, int startY, int sub_level)
	: ShootingActor(w, IID_PLAYER, startX, startY, right, sub_level, PLAYER_HP, SOUND_PLAYER_DIE, SOUND_PLAYER_IMPACT), ammo(0) {}
void Player::setAmmo(int newAmmo)
{
	ammo = newAmmo;
}
int Player::getAmmo() const
{
	return ammo;
}
void Player::uniqueAction()
{
	int ch;
	if (getWorld()->getKey(ch))// the player object recieves input
	{
		bool move = false;
		switch (ch)
		{
			case KEY_PRESS_ESCAPE://suicide
				setHP(0);
				break;
			case KEY_PRESS_SPACE://shoots
				if (ammo > 0)
				{
					getWorld()->playSound(SOUND_PLAYER_FIRE);
					fire();
					ammo--;
				}
				break;
			case KEY_PRESS_LEFT:
				move = true;
				setDirection(left);
				break;
			case KEY_PRESS_UP:
				move = true;
				setDirection(up);
				break;
			case KEY_PRESS_RIGHT:
				move = true;
				setDirection(right);
				break;
			case KEY_PRESS_DOWN:
				move = true;
				setDirection(down);
				break;
		}
		int x = getX();
		int y = getY();
		moveCoordForward(x, y);
		if (move && !getWorld()->moveBlocked(x, y))//if a move key was pressed, try to move in the direction the player faces
			moveTo(x,y);
	}
}

Gangster::Gangster(StudentWorld * w, int startX, int startY, int sub_level, Direction d)
	: BadGuy(w, IID_GANGSTER, startX, startY, d, sub_level, GANGSTER_HP, GANGSTER_SCORE) {}
void Gangster::dropObject() const
{
	Waterpool* newWaterpool = new Waterpool(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());//when gangsters die they add a waterpool
	getWorld()->addActor(newWaterpool);
}

RobotBoss::RobotBoss(StudentWorld * w, int startX, int startY, int sub_level)
	: BadGuy(w, IID_ROBOT_BOSS, startX, startY, right, sub_level, ROBOT_HP, ROBOT_SCORE) {}
void RobotBoss::dropObject() const
{
	Hostage* newHostage = new Hostage(getWorld(), getX(), getY(), getWorld()->getCurrentSubLevel());//when robot bosses die the add a hostage
	getWorld()->addActor(newHostage);
}

Bully::Bully(StudentWorld * w, int startX, int startY, int sub_level)
	: BadGuy(w, IID_BULLY, startX, startY, right, sub_level, BULLY_HP, BULLY_SCORE), moveCount(0)
{
	distanceBeforeTurning = rand() % BULLY_MAX_DISTANCE + 1;//randomly choose value for distance before turning
}
void Bully::moveBehaviour()
{
	if (!pickUpGoodies())//try to pick up goodies on the square the bully is on
	{
		int x = getX();//if no goodies were picked up, try to move
		int y = getY();
		moveCoordForward(x, y);
		if (moveCount == distanceBeforeTurning || getWorld()->moveBlocked(x, y))//if it cannot move, or if it has moved the distance before turning
		{
			moveCount = 0;//reset move count
			distanceBeforeTurning = rand() % BULLY_MAX_DISTANCE + 1;//choose another random distance before turning
			vector<Direction> directionsNotTried = { up,down,left,right };
			random_shuffle(directionsNotTried.begin(), directionsNotTried.end());//shuffle a direction list randomly
			Direction d = directionsNotTried.back();//save first random direction
			Direction dTry;
			bool moved = false;
			do {//repeatedly
				dTry = directionsNotTried.back();//pick a direction from the direction list
				x = getX();
				y = getY();
				setDirection(dTry);
				moveCoordForward(x, y);
				if (!getWorld()->moveBlocked(x, y))//try to move in that direction
				{
					moveTo(x, y);
					moveCount++;
					moved = true;
				}
				directionsNotTried.pop_back();//remove the direction tried from direction list
			} while (!directionsNotTried.empty() && !moved);//stop if move was sucessful or no more directions are left
			if (!moved)
				setDirection(d);//if the bully cannot move, set the direction to the first random direction tried
		}
		else//the bully can move, so move and increase move count
		{
			moveTo(x, y);
			moveCount++;
		}
	}
}
void Bully::dropObject() const
{
	StudentWorld* w = getWorld();
	vector<int>::const_iterator it;
	for (it = pickedUpGoodies.begin(); it != pickedUpGoodies.end(); it++)//for each goodie in the bully's picked up goodie list
	{
		Consumable* goodieToAdd = nullptr;
		int x = getX();
		int y = getY();
		int sub = w->getCurrentSubLevel();
		switch (*it)//create the appropriate goodie at the right location
		{
			case IID_AMMO:
				goodieToAdd = new AmmoGoodie(w, x, y, sub);
				break;
			case IID_RESTORE_HEALTH:
				goodieToAdd = new RestoreHealthGoodie(w, x, y, sub);
				break;
			case IID_EXTRA_LIFE:
				goodieToAdd = new ExtraLifeGoodie(w, x, y, sub);
				break;
		}
		w->addActor(goodieToAdd);//add the goodie to the studentWorld;
	}
}
bool Bully::pickUpGoodies()
{
	vector<int> goodies = getWorld()->consumeGoodies(getX(), getY());//gets all the goodies in the current square
	if (goodies.empty())//if there are no goodies return false
		return false;
	vector<int>::const_iterator it;
	for (it = goodies.begin(); it != goodies.end(); it++)//otherwise adds all goodies in square to its picked up goodies vector
		pickedUpGoodies.push_back(*it);
	getWorld()->playSound(SOUND_BULLY_MUNCH);//plays munching sound and return true
	return true;
}