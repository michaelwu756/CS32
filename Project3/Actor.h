#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>
class StudentWorld;
class Player;
const int PLAYER_HP = 20;
const int GANGSTER_HP = 10;
const int BULLY_HP = 5;
const int ROBOT_HP = 50;
const int WATERPOOL_HP = 30;

const int GANGSTER_SCORE = 100;
const int BULLY_SCORE = 10;
const int ROBOT_SCORE = 3000;
const int EXIT_SCORE = 1500;
const int GATE_SCORE = 0;
const int HOSTAGE_SCORE = 0;
const int GOLD_SCORE = 100;
const int EXTRA_LIFE_SCORE = 150;
const int RESTORE_HEALTH_SCORE = 250;
const int AMMO_SCORE = 150;
const int FARPLANE_GUN_SCORE = 0;

const int AMMO_INCREASE = 25;
const int MIN_TICKS = 3;
const int BULLET_DAMAGE = 2;
const int FARPLANE_GUN_DAMAGE = 10;
const int BULLY_MAX_DISTANCE = 6;
const int FARPLANE_GUN_RANGE = 4;
const int BULLY_NEST_RANGE = 3;
const int BULLY_NEST_GEN_CHANCE = 50;

class Actor : public GraphObject
{
	public:
		Actor(int imageID, int startX, int startY, Direction dir, int sub_level);
		virtual void doSomething();
		virtual bool dead() const;
		virtual void damage(int dmg=BULLET_DAMAGE);
	private:
		virtual void uniqueAction();
};

class LiveActor : public Actor
{
	public:
		LiveActor(StudentWorld* w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int score=0);
		void setHP(int val);
		int getHP() const;
		int getScore() const;
		void moveCoordForward(int& x, int& y) const;
		StudentWorld* getWorld() const; 
		virtual bool dead() const;
	private:
		int hp;
		int m_score;
		StudentWorld* world;
};
class Consumable : public LiveActor
{
	public:
		Consumable(StudentWorld* w, int imageID, int startX, int startY, int sub_level, int score, int sound);
	private:
		int m_sound;
		
		bool checkPlayer();
		virtual void uniqueAction();
		virtual void consumeEffect() const;
};
class ShootingActor : public LiveActor
{
	public:
		ShootingActor(StudentWorld* w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int deathSound, int hitSound, int score=0);
		void fire() const;
		virtual void damage(int dmg=BULLET_DAMAGE);
	private:
		int m_deathSound;
		int m_hitSound;
	
		virtual void deathBehaviour() const;
};
class BadGuy : public ShootingActor
{
	public:
		BadGuy(StudentWorld* w, int imageID, int startX, int startY, Direction dir, int sub_level, int hitPoints, int score);
		
	private:
		int ticks;
		int tickCount;

		void addTick();
		bool resting() const;
		bool shootsPlayer() const;
		bool shootsPlayer(int x,int y) const;
		virtual void uniqueAction();
		virtual void moveBehaviour();
		virtual void deathBehaviour() const;
		virtual void dropObject() const=0;
};

class Wall : public Actor
{
	public:
		Wall(int startX, int startY, int sub_level);
};
class FakeWall : public Actor
{
	public:
		FakeWall(int startX, int startY, int sub_level);
};

class Waterpool : public LiveActor
{
	public:
		Waterpool(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void uniqueAction();
};
class BullyNest : public LiveActor
{
	public:
		BullyNest(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void uniqueAction();
};
class Bullet : public LiveActor
{
	public:
		Bullet(StudentWorld* w, int startX, int startY, Direction dir, int sub_level);

	private:
		bool checkHit();
		virtual void uniqueAction();
};

class Gold : public Consumable
{
	public:
		Gold(StudentWorld* w, int startX, int startY, int sub_level);
};
class Hostage : public Consumable
{
	public:
		Hostage(StudentWorld* w, int startX, int startY, int sub_level);
};
class AmmoGoodie : public Consumable
{
	public:
		AmmoGoodie(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void consumeEffect() const;
};
class ExtraLifeGoodie : public Consumable
{
	public:
		ExtraLifeGoodie(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void consumeEffect() const;
};
class RestoreHealthGoodie : public Consumable
{
	public:
		RestoreHealthGoodie(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void consumeEffect() const;
};
class FarplaneGun : public Consumable
{
	public:
		FarplaneGun(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		virtual void consumeEffect() const;
};
class Gate : public Consumable
{
	public:
		Gate(StudentWorld* w, int startX, int startY, int sub_level, int target);
	private:
		int m_target;
		
		virtual void consumeEffect() const;
};
class Exit : public Consumable
{
	public:
		Exit(StudentWorld* w, int startX, int startY, int sub_level);
		virtual void doSomething();
		void reveal();
		bool isRevealed() const;
	private:
		bool revealed;

		virtual void consumeEffect() const;
};

class Player : public ShootingActor
{
	public:
		Player(StudentWorld* w, int startX, int startY, int sub_level);
		void setAmmo(int toAdd);
		int getAmmo() const;
	private:
		int ammo;
		virtual void uniqueAction();
};

class Gangster : public BadGuy
{
	public:
		Gangster(StudentWorld* w, int startX, int startY, int sub_level, Direction d);
	private:
		virtual void dropObject() const;
};
class RobotBoss : public BadGuy
{
	public:
		RobotBoss(StudentWorld* w, int startX, int startY, int sub_level);
	private:	
		virtual void dropObject() const;
};
class Bully : public BadGuy
{
	public:
		Bully(StudentWorld* w, int startX, int startY, int sub_level);
	private:
		std::vector<int> pickedUpGoodies;
		int distanceBeforeTurning;
		int moveCount;
		
		virtual void moveBehaviour();
		virtual void dropObject() const;
		bool pickUpGoodies();
};

#endif // ACTOR_H_