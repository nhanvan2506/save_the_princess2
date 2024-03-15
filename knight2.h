#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG

class Events;
class BaseBag;
class BaseItem;
class BaseKnight;
class BaseOpponent;
class ArmyKnights;
class KnightAdventure;

class PaladinKnight;
class DragonKnight;
class LancelotKnight;
class NormalKnight;
class ItemList;

class DragonBag;
class LancelotBag;
class PaladinBag;
class KnightBag;

class Antidote;
class PhoenixI;
class PhoenixII;
class PhoenixIII;
class PhoenixIV;

class MadBear;
class Bandit;
class LordLupin;
class Elf;
class Troll;
class Tornbery;
class Queen;
class Nina;
class Durian;
class Omega;
class Hades;
class Event112;
class Event113;
class Event114;
class PickShield;
class PickHair;
class PickSpear;
class PickSword;
class Ultimecia;

enum ItemType
{
    ANT = 0,
    P1,
    P2,
    P3,
    P4
};

class ItemNode
{
public:
    BaseItem* baseItem;
    ItemNode* next;

    ItemNode(BaseItem* baseItem = nullptr, ItemNode* itemNode = nullptr) : baseItem(baseItem), next(itemNode) {}
};

class BaseBag
{
public:
    virtual bool insertFirst(BaseItem* item);
    virtual BaseItem* get(ItemType itemType);
    virtual string toString() const;
    virtual void dropItem();
    bool transferANT();
protected:
    int bagCapacity;
    int bagSize;
    ItemNode* head;
    BaseKnight* knight;

private:
    BaseItem* getFirstAndRemove();
    BaseItem* getFirstUsuablePhoenix();
    void swapWithHead(ItemNode* itemNode);

private:
    BaseItem* getFirstAntidote(ItemNode* current);
    BaseItem* getFirstPhoenix(ItemNode* current);
    string getItemString(ItemNode* current, string res) const;

public:
    BaseBag(BaseKnight* knight, int cap, int ant, int ph);
};

class DragonBag : public BaseBag
{
public:
    DragonBag(BaseKnight* knight, int ant, int ph);

    bool insertFirst(BaseItem* item) override;
    bool transferANT();
};

class LancelotBag : public BaseBag
{
public:
    LancelotBag(BaseKnight* knight, int ant, int ph);
};

class PaladinBag : public BaseBag
{
public:
    PaladinBag(BaseKnight* knight, int ant, int ph);
};

class KnightBag : public BaseBag
{
public:
    KnightBag(BaseKnight* knight, int ant, int ph);
};

enum KnightType
{
    PALADIN = 0,
    LANCELOT,
    DRAGON,
    NORMAL
};

class BaseKnight
{
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    // int antidote;
    BaseBag* bag;
    KnightType knightType;

public:
    BaseKnight(int id, int maxhp, int level, int gil) : id(id), hp(maxhp), maxhp(maxhp), gil(gil), level(level), bag(nullptr), knightType(knightType) {}

public:
    static BaseKnight* create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;

public:
    void dropItem();
    bool insertItem(BaseItem* item);
    bool usePhoenix();
    /* bool usePhoenix2();
     bool usePhoenix3();
     bool usePhoenix4();*/
    bool useAntidote();
    void abc();

public:
    int putMoney(int gil);
    bool recoverHP();
    void abc(ItemType itemType);
public:
    // Getters
    int getId() const { return id; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxhp; }
    int getLevel() const { return level; }
    int getGil() const { return gil; }
    KnightType getKnightType() const { return knightType; }
    // Setters
    void setHp(int newHp) { hp = min(newHp, 999); }
    void setLevel(int newLevel) { level = min(newLevel, 10); }
    void setGil(int newGil) { gil = min(newGil, 999); }
    BaseBag* getBag() { return bag; }
};

class PaladinKnight : public BaseKnight
{
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
};

class LancelotKnight : public BaseKnight
{
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
};

class NormalKnight : public BaseKnight
{
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
};

class DragonKnight : public BaseKnight
{
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
};

class BaseOpponent
{
public:
    BaseOpponent(BaseKnight* knight, int i, int e);

    virtual int getFightResult();

    virtual void winEffect(ArmyKnights* armyKnights) = 0;

    virtual bool loseEffect(ArmyKnights* armyKnights) = 0;

protected:
    int level;
    int levelO;
    int i;
    int eventcode;
    BaseKnight* knight;
};

class MadBear : public BaseOpponent
{
public:
    MadBear(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Bandit : public BaseOpponent
{
public:
    Bandit(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class LordLupin : public BaseOpponent
{
public:
    LordLupin(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Elf : public BaseOpponent
{
public:
    Elf(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Troll : public BaseOpponent
{
public:
    Troll(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Tornbery : public BaseOpponent
{
public:
    Tornbery(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Queen : public BaseOpponent
{
public:
    Queen(BaseKnight* knight, int i, int e);

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Nina : public BaseOpponent
{
public:
    Nina(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Durian : public BaseOpponent
{
public:
    Durian(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Omega : public BaseOpponent
{
public:
    Omega(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Hades : public BaseOpponent
{
public:
    Hades(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Event112 : public BaseOpponent
{
public:
    Event112(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Event113 : public BaseOpponent
{
public:
    Event113(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Event114 : public BaseOpponent
{
public:
    Event114(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class PickShield : public BaseOpponent
{
public:
    PickShield(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class PickHair : public BaseOpponent
{
public:
    PickHair(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class PickSpear : public BaseOpponent
{
public:
    PickSpear(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class PickSword : public BaseOpponent
{
public:
    PickSword(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class Ultimecia : public BaseOpponent
{
public:
    Ultimecia(BaseKnight* knight, int i, int e);

    int getFightResult() override;

    void winEffect(ArmyKnights* armyKnights) override;

    bool loseEffect(ArmyKnights* armyKnights) override;
};

class ArmyKnights
{
public:
    ArmyKnights(const string& file_armyknights);
    ~ArmyKnights();

    bool fight(BaseOpponent* opponent);
    bool adventure(Events* events);

    void printInfo() const;
    void printResult(bool win) const;

private:
    int numKnights;
    int last;
    BaseKnight** armyKnights;

    bool hasShield;
    bool hasSpear;
    bool hasHair;
    bool hasSword;

    bool defeatedHades;
    bool defeatedOmega;

public:
    // Getters
    int count() const { return numKnights; }

    BaseKnight* lastKnight() const { return last >= 0 && last < numKnights ? armyKnights[last] : nullptr; }

    int getNumKnights() const { return numKnights; }
    bool hasPaladinShield() const { return hasShield; }
    bool hasLancelotSpear() const { return hasSpear; }
    bool hasGuinevereHair() const { return hasHair; }
    bool hasExcaliburSword() const { return hasSword; }

    // Setters
    void setNumKnights(int newNumKnights) { numKnights = newNumKnights; }
    void setLast(int newLast) { last = newLast; }
    void setHasShield() { hasShield = true; }
    void setHasSpear() { hasSpear = true; }
    void setHasHair() { hasHair = true; }
    void setHasSword() { hasSword = true; }

    bool getDefeatedOmega() const { return defeatedOmega; }
    bool getDefeatedHades() const { return defeatedHades; }
    void setDefeatedOmega() { defeatedOmega = true; }
    void setDefeatedHades() { defeatedHades = true; }
public:
    void lastKnightHasDied();
    void passMoneyToFront(BaseKnight* knight, int gil);
    void passItemToFront(BaseKnight* knight, BaseItem* item);
    BaseKnight* getPreviousKnight(BaseKnight* knight);
};

class BaseItem
{
public:
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;

    ItemType itemType;
};

class Antidote : public BaseItem
{
public:
    Antidote();

    bool canUse(BaseKnight* knight) override;
    void use(BaseKnight* knight) override;
};

class PhoenixI : public BaseItem
{
public:
    PhoenixI();

    bool canUse(BaseKnight* knight) override;
    void use(BaseKnight* knight) override;
};

class PhoenixII : public BaseItem
{
public:
    PhoenixII();

    bool canUse(BaseKnight* knight) override;
    void use(BaseKnight* knight) override;
};

class PhoenixIII : public BaseItem
{
public:
    PhoenixIII();

    bool canUse(BaseKnight* knight) override;
    void use(BaseKnight* knight) override;
};

class PhoenixIV : public BaseItem
{
public:
    PhoenixIV();

    bool canUse(BaseKnight* knight) override;
    void use(BaseKnight* knight) override;
};

class Events
{
public:
    Events(const string&);
    ~Events();

public:
    int count() const;
    int get(int i) const;

private:
    int numEvents;
    int* eventCodes;

public:
    int getNumEvents() const { return numEvents; }
};

class KnightAdventure
{
private:
    ArmyKnights* armyKnights;
    Events* events;

public:
    KnightAdventure();
    ~KnightAdventure();

    void loadArmyKnights(const string&);
    void loadEvents(const string&);
    void run();
};

#endif // __KNIGHT2_H__
