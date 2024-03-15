#include "knight2.h"

bool isPrime(int n)
{
    if (n < 2)
        return false;

    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

bool isDragon(int hp)
{
    if (hp < 100)
        return false;

    int third = hp % 10;
    hp /= 10;
    int second = hp % 10;
    hp /= 10;
    int first = hp;

    third *= third;
    second *= second;
    first *= first;

    if (first == second + third || second == first + third || third == first + second)
        return true;

    return false;
}

BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
{
    if (isPrime(maxhp))
        return new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if (maxhp == 888)
        return new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if (isDragon(maxhp))
        return new DragonKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else
        return new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);
}

string BaseKnight::toString() const
{
    string typeString[4] = { "PALADIN", "LANCELOT", "DRAGON", "NORMAL" };
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id) + ",hp:" + to_string(hp) + ",maxhp:" + to_string(maxhp) + ",level:" + to_string(level) + ",gil:" + to_string(gil) + "," + bag->toString() + ",knight_type:" + typeString[knightType] + "]";
    return s;
}

void BaseKnight::dropItem()
{
    bag->dropItem();
}

bool BaseKnight::insertItem(BaseItem* item)
{
    return bag->insertFirst(item);
}

bool BaseKnight::usePhoenix()
{
    BaseItem* phoenix = bag->get(P1);

    if (!phoenix)
        return false;

    phoenix->use(this);
    return true;
}

bool BaseKnight::useAntidote()
{
    BaseItem* ant = bag->get(ANT);

    if (!ant)
        return false;

    return true;
}

int BaseKnight::putMoney(int gil)
{
    int currentGil = getGil();

    setGil(std::min(currentGil + gil, 999));
    if (getGil() > 999)
        return gil + currentGil - 999;
    else { return 0; }
}

bool BaseKnight::recoverHP()
{
    if (hp <= 0)
    {
        bool usedPhoenix = usePhoenix();
        if (usedPhoenix)
            return true;

        if (gil < 100)
            return false;

        gil -= 100;
        hp = (int)maxhp / 2;

        return true;
    }
    else
    {
        bool usedPhoenix = usePhoenix();
        return true;
    }
}

ArmyKnights::ArmyKnights(const string& file_armyknights)
    : hasShield(false), hasSpear(false), hasHair(false), hasSword(false), defeatedOmega(false), defeatedHades(false)
{
    string line = "";
    ifstream file(file_armyknights);
    getline(file, line);
    numKnights = stoi(line);
    last = numKnights - 1;
    armyKnights = new BaseKnight * [numKnights];

    int id = 1;
    while (getline(file, line))
    {
        stringstream ss(line);
        int hp, level, phoenixdownI, gil, antidote;

        ss >> hp >> level >> phoenixdownI >> gil >> antidote;

        armyKnights[id - 1] = BaseKnight::create(id, hp, level, gil, antidote, phoenixdownI);
        id++;
    }
}

ArmyKnights::~ArmyKnights()
{
    for (int i = 0; i < numKnights; i++)
        delete armyKnights[i];
    delete[] armyKnights;
}

bool ArmyKnights::fight(BaseOpponent* opponent)
{
    int result = opponent->getFightResult();

    if (result == 1)
    {
        opponent->winEffect(this);
        return false;
    }

    else if (result == -1)
        return !opponent->loseEffect(this);

    return true;
}

//s? ki?n
bool ArmyKnights::adventure(Events* events)
{
    if (lastKnight()->getKnightType() == DRAGON)
    {
        while (lastKnight()->getBag()->transferANT() == 1)
        {
            Antidote* ant = new Antidote;
            passItemToFront(getPreviousKnight(lastKnight()), ant);
        }
    }
    for (int i = 0; i < events->count(); i++)
    {

        int code = events->get(i);
        BaseOpponent* opponent = nullptr;

        if (code == 1)
            opponent = new MadBear(lastKnight(), i, code);
        else if (code == 2)
            opponent = new Bandit(lastKnight(), i, code);
        else if (code == 3)
            opponent = new LordLupin(lastKnight(), i, code);
        else if (code == 4)
            opponent = new Elf(lastKnight(), i, code);
        else if (code == 5)
            opponent = new Troll(lastKnight(), i, code);
        else if (code == 6)
            opponent = new Tornbery(lastKnight(), i, code);
        else if (code == 7)
            opponent = new Queen(lastKnight(), i, code);
        else if (code == 8)
            opponent = new Nina(lastKnight(), i, code);
        else if (code == 9)
            opponent = new Durian(lastKnight(), i, code);
        else if (code == 10)
            opponent = new Omega(lastKnight(), i, code);
        else if (code == 11)
            opponent = new Hades(lastKnight(), i, code);
        else if (code == 112)
            opponent = new Event112(lastKnight(), i, code);
        else if (code == 113)
            opponent = new Event113(lastKnight(), i, code);
        else if (code == 114)
            opponent = new Event114(lastKnight(), i, code);
        else if (code == 95)
            opponent = new PickShield(lastKnight(), i, code);
        else if (code == 96)
            opponent = new PickSpear(lastKnight(), i, code);
        else if (code == 97)
            opponent = new PickHair(lastKnight(), i, code);
        else if (code == 98)
            opponent = new PickSword(lastKnight(), i, code);
        else if (code == 99)
            opponent = new Ultimecia(lastKnight(), i, code);
        bool hasFallen = fight(opponent);

        if (hasFallen)
            lastKnightHasDied();
        if (numKnights == 0)
        {
            printInfo();

            return false;
        }

        printInfo();
    }

    return true;
}

void ArmyKnights::printInfo() const
{
    cout << "No. knights: " << this->count();
    if (this->count() > 0)
    {
        BaseKnight* lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const
{
    cout << (win ? "WIN" : "LOSE") << endl;
}

void ArmyKnights::lastKnightHasDied()
{
    last--;
    numKnights--;
}

void ArmyKnights::passMoneyToFront(BaseKnight* knight, int gil)
{
    if (knight == nullptr)
        return;

    int left = knight->putMoney(gil);

    if (left <= 0)
        return;

    passMoneyToFront(getPreviousKnight(knight), left);
}

void ArmyKnights::passItemToFront(BaseKnight* knight, BaseItem* item)
{
    if (knight == nullptr || knight->insertItem(item))
        return;

    passItemToFront(getPreviousKnight(knight), item);
}

BaseKnight* ArmyKnights::getPreviousKnight(BaseKnight* knight)
{
    int id = knight->getId();

    if (id == 1)
        return nullptr;

    return this->armyKnights[id - 2];
}

KnightAdventure::KnightAdventure()
{
    armyKnights = nullptr;
    events = nullptr;
}

KnightAdventure::~KnightAdventure()
{
    delete armyKnights;
    delete events;
}

void KnightAdventure::loadArmyKnights(const string& file_armyknights)
{
    armyKnights = new ArmyKnights(file_armyknights);
}

void KnightAdventure::loadEvents(const string& file_events)
{
    events = new Events(file_events);
}

void KnightAdventure::run()
{
    armyKnights->printResult(armyKnights->adventure(events));
}

// START HERE

bool BaseBag::insertFirst(BaseItem* item)
{
    if (bagSize >= bagCapacity)
        return false;

    ItemNode* n = new ItemNode(item);
    n->next = head;
    head = n;
    bagSize++;
    return true;
}

BaseItem* BaseBag::get(ItemType itemType)
{
    if (itemType != ANT)
        return getFirstUsuablePhoenix();

    return getFirstAntidote(head);
}

string BaseBag::toString() const
{
    string init = "Bag[count=" + to_string(bagSize) + ";";

    return head ? getItemString(head, init) : (init + "]");
}

void BaseBag::dropItem()
{
    BaseItem* item = getFirstAndRemove();
}

BaseItem* BaseBag::getFirstAndRemove()
{
    if (!head)
        return NULL;

    ItemNode* n = head;
    head = head->next;
    n->next = nullptr;
    bagSize--;
    return n->baseItem;
}

BaseItem* BaseBag::getFirstUsuablePhoenix()
{
    return getFirstPhoenix(head);
}

void BaseBag::swapWithHead(ItemNode* itemNode)
{
    BaseItem* t = itemNode->baseItem;
    itemNode->baseItem = head->baseItem;
    head->baseItem = t;
}

BaseItem* BaseBag::getFirstAntidote(ItemNode* current)
{
    if (!current)
        return NULL;

    if (current->baseItem->itemType != ANT)
        return getFirstAntidote(current->next);

    swapWithHead(current);
    return getFirstAndRemove();
}

BaseItem* BaseBag::getFirstPhoenix(ItemNode* current)
{
    if (!current)
        return NULL;

    if (current->baseItem->itemType == ANT)
        return getFirstPhoenix(current->next);

    if (!(current->baseItem->canUse(knight)))
        return getFirstPhoenix(current->next);

    swapWithHead(current);
    return getFirstAndRemove();
}

string BaseBag::getItemString(ItemNode* current, string res) const
{
    string itemString[] = { "Antidote", "PhoenixI", "PhoenixII", "PhoenixIII", "PhoenixIV" };

    res += itemString[current->baseItem->itemType];

    if (current->next == nullptr)
        return res + "]";

    return getItemString(current->next, res + ",");
}

BaseBag::BaseBag(BaseKnight* knight, int cap, int ant, int ph)
    : knight(knight), bagCapacity(cap), bagSize(0), head(nullptr)
{
    while (ph-- > 0)
        insertFirst(new PhoenixI());
    while (ant-- > 0)
    {
        insertFirst(new Antidote());
    }
}

DragonBag::DragonBag(BaseKnight* knight, int ant, int ph)
    : BaseBag(knight, 14, ant, ph)
{

}

bool BaseBag::transferANT()
{
    ItemNode* currentNode = head;
    ItemNode* prevNode = nullptr;

    while (currentNode != nullptr)
    {
        if (currentNode->baseItem->itemType == ANT)
        {
            if (prevNode)
                prevNode->next = currentNode->next;
            else
                head = currentNode->next;

            delete currentNode->baseItem;
            delete currentNode;
            bagSize--;
            return true;
        }

        prevNode = currentNode;
        currentNode = currentNode->next;
    }

    return false;

}

bool DragonBag::insertFirst(BaseItem* item)
{
    if (item->itemType == ANT)
    {
        return false;
    }

    return BaseBag::insertFirst(item);
}

LancelotBag::LancelotBag(BaseKnight* knight, int ant, int ph)
    : BaseBag(knight, 16, ant, ph)
{
}

PaladinBag::PaladinBag(BaseKnight* knight, int ant, int ph)
    : BaseBag(knight, 2000000000, ant, ph)
{
}

KnightBag::KnightBag(BaseKnight* knight, int ant, int ph)
    : BaseBag(knight, 19, ant, ph)
{
}

Antidote::Antidote()
{
    itemType = ANT;
}

bool Antidote::canUse(BaseKnight* knight)
{
    return true;
}

void Antidote::use(BaseKnight* knight)
{
}

PhoenixI::PhoenixI()
{
    itemType = P1;
}

bool PhoenixI::canUse(BaseKnight* knight)
{
    return knight->getHp() <= 0;
}

void PhoenixI::use(BaseKnight* knight)
{
    knight->setHp(knight->getMaxHp());
}

PhoenixII::PhoenixII()
{
    itemType = P2;
}

bool PhoenixII::canUse(BaseKnight* knight)
{
    int hp = knight->getHp();
    int maxhp = knight->getMaxHp() / 4;

    return hp < maxhp;
}

void PhoenixII::use(BaseKnight* knight)
{
    knight->setHp(knight->getMaxHp());
}

PhoenixIII::PhoenixIII()
{
    itemType = P3;
}

bool PhoenixIII::canUse(BaseKnight* knight)
{
    int hp = knight->getHp();
    int maxhp = knight->getMaxHp() / 3;

    return hp < maxhp;
}

void PhoenixIII::use(BaseKnight* knight)
{
    int maxhp = knight->getMaxHp();
    int hp = knight->getHp();

    int newhp = hp <= 0 ? (int)maxhp / 3 : hp + (int)maxhp / 4;
    knight->setHp(newhp);
}

PhoenixIV::PhoenixIV()
{
    itemType = P4;
}

bool PhoenixIV::canUse(BaseKnight* knight)
{
    int hp = knight->getHp();
    int maxhp = knight->getMaxHp() / 2;

    return hp < maxhp;
}

void PhoenixIV::use(BaseKnight* knight)
{
    int maxhp = knight->getMaxHp();
    int hp = knight->getHp();

    int newhp = hp <= 0 ? (int)maxhp / 2 : hp + (int)maxhp / 5;
    knight->setHp(newhp);
}

DragonKnight::DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) : BaseKnight(id, maxhp, level, gil)
{
    bag = new DragonBag(this, antidote, phoenixdownI);
    knightType = DRAGON;
}

NormalKnight::NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    : BaseKnight(id, maxhp, level, gil)
{
    bag = new KnightBag(this, antidote, phoenixdownI);
    knightType = NORMAL;
}

LancelotKnight::LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    : BaseKnight(id, maxhp, level, gil)
{
    bag = new LancelotBag(this, antidote, phoenixdownI);
    knightType = LANCELOT;
}

PaladinKnight::PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
    : BaseKnight(id, maxhp, level, gil)
{
    bag = new PaladinBag(this, antidote, phoenixdownI);
    knightType = PALADIN;
}

BaseOpponent::BaseOpponent(BaseKnight* knight, int i, int e)
    : knight(knight), i(i), eventcode(e)
{
    level = knight->getLevel();
    levelO = (i + eventcode) % 10 + 1;
}

int BaseOpponent::getFightResult()
{
    if (level >= levelO)
        return 1;

    else
        return -1;
}

MadBear::MadBear(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

void MadBear::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, 100);
}

bool MadBear::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN || knight->getKnightType() == LANCELOT)
    {
        winEffect(armyKnights);
        return 1;
    }
    int hp = knight->getHp() - 10 * (levelO - level);

    knight->setHp(hp);

    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

Bandit::Bandit(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

void Bandit::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, 150);
}

bool Bandit::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN || knight->getKnightType() == LANCELOT)
    {
        winEffect(armyKnights);
        return 1;
    }
    int hp = knight->getHp() - 15 * (levelO - level);

    knight->setHp(hp);

    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

LordLupin::LordLupin(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

void LordLupin::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, 450);
}

bool LordLupin::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN || knight->getKnightType() == LANCELOT)
    {
        winEffect(armyKnights);
        return 1;
    }
    int hp = knight->getHp() - 45 * (levelO - level);

    knight->setHp(hp);


    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

Elf::Elf(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{

}

void Elf::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, 750);
}

bool Elf::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN || knight->getKnightType() == LANCELOT)
    {
        winEffect(armyKnights);
        return 1;
    }
    int hp = knight->getHp() - 75 * (levelO - level);

    knight->setHp(hp);


    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

Troll::Troll(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{

}

void Troll::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, 800);
}

bool Troll::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN || knight->getKnightType() == LANCELOT)
    {
        winEffect(armyKnights);
        return 1;
    }
    int hp = knight->getHp() - 95 * (levelO - level);

    knight->setHp(hp);


    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

Tornbery::Tornbery(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{

}

void Tornbery::winEffect(ArmyKnights* armyKnights)
{
    knight->setLevel(knight->getLevel() + 1);
}

bool Tornbery::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->useAntidote())
        return true;

    knight->dropItem();
    knight->dropItem();
    knight->dropItem();

    knight->setHp(knight->getHp() - 10);

    int hp = knight->getHp();
    bool canrecover = knight->recoverHP();
    if (hp <= 0)
        return canrecover;

    return true;
}

Queen::Queen(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

void Queen::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passMoneyToFront(knight, knight->getGil());
}

bool Queen::loseEffect(ArmyKnights* armyKnights)
{
    if (knight->getKnightType() == PALADIN)
    {
        return 1;
    }
    int gil = knight->getGil() / 2;
    knight->setGil(gil);

    return true;
}

Nina::Nina(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Nina::getFightResult()
{
    // not enough money & not paladin
    if (knight->getGil() < 50 && knight->getKnightType() != PALADIN)
        return -1;

    int hp = knight->getHp();
    int maxhp = knight->getMaxHp();

    // but not need more hp
    if (!(hp < (int)(maxhp / 3)))
        return -1;

    // paladin
    if (knight->getKnightType() == PALADIN)
        return 1;

    // other knights -50
    return 1;
}

void Nina::winEffect(ArmyKnights* armyKnights)
{
    int hp = knight->getHp();
    int maxhp = knight->getMaxHp();
    int gil = knight->getGil();

    knight->setHp(hp + (int)maxhp / 5);

    if (knight->getKnightType() != PALADIN)
        knight->setGil(gil - 50);
}

bool Nina::loseEffect(ArmyKnights* armyKnights)
{
    return true;
}

Durian::Durian(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Durian::getFightResult()
{
    return 1;
}

void Durian::winEffect(ArmyKnights* armyKnights)
{
    knight->setHp(knight->getMaxHp());
}

bool Durian::loseEffect(ArmyKnights* armyKnights)
{
    return true;
}

Omega::Omega(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Omega::getFightResult()
{
    int hp = knight->getHp();
    int maxhp = knight->getMaxHp();
    int level = knight->getLevel();
    KnightType knightType = knight->getKnightType();

    if ((hp == maxhp && level == 10) || knightType == DRAGON)
        return 1;

    return -1;
}

void Omega::winEffect(ArmyKnights* armyKnights)
{

    if (armyKnights->getDefeatedOmega())
        return;
    knight->setLevel(10);
    knight->setGil(999);
    armyKnights->setDefeatedOmega();
}

bool Omega::loseEffect(ArmyKnights* armyKnights)
{
    if (armyKnights->getDefeatedOmega())
        return true;
    knight->setHp(0);

    return knight->recoverHP();
}

Hades::Hades(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Hades::getFightResult()
{
    int level = knight->getLevel();
    KnightType knightType = knight->getKnightType();

    if (level == 10 || (knightType == PALADIN && level >= 8))
        return 1;

    return -1;
}

void Hades::winEffect(ArmyKnights* armyKnights)
{
    if (armyKnights->getDefeatedHades())
        return;
    armyKnights->setHasShield();
    armyKnights->setDefeatedHades();
}

bool Hades::loseEffect(ArmyKnights* armyKnights)
{
    if (armyKnights->getDefeatedHades())
        return true;
    knight->setHp(0);
    return knight->recoverHP();
}

Events::Events(const string& file_events)
{
    string line = "";
    ifstream file(file_events);

    if (!file)
        throw std::ios_base::failure("Cannot open file!");
    getline(file, line);
    numEvents = stoi(line);
    this->eventCodes = new int[this->numEvents];

    int index = 0;
    for (int i = 0; i < numEvents; i++)
        file >> eventCodes[index++];
}

Events::~Events()
{
    delete[] eventCodes;
}

int Events::count() const
{
    return numEvents;
}

int Events::get(int i) const
{
    return eventCodes[i];
}

Event112::Event112(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Event112::getFightResult()
{
    return 1;
}

void Event112::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passItemToFront(knight, new PhoenixII());
}

bool Event112::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

Event113::Event113(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Event113::getFightResult()
{
    return 1;
}

void Event113::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passItemToFront(knight, new PhoenixIII());
}

bool Event113::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

Event114::Event114(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Event114::getFightResult()
{
    return 1;
}

void Event114::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->passItemToFront(knight, new PhoenixIV());
}

bool Event114::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

PickShield::PickShield(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int PickShield::getFightResult()
{
    return 1;
}

void PickShield::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->setHasShield();
}

bool PickShield::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

PickHair::PickHair(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int PickHair::getFightResult()
{
    return 1;
}

void PickHair::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->setHasHair();
}

bool PickHair::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

PickSpear::PickSpear(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int PickSpear::getFightResult()
{
    return 1;
}

void PickSpear::winEffect(ArmyKnights* armyKnights)
{
    armyKnights->setHasSpear();
}

bool PickSpear::loseEffect(ArmyKnights* armyKnights)
{
    return false;
}

PickSword::PickSword(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int PickSword::getFightResult()
{
    return 1;
}

void PickSword::winEffect(ArmyKnights* armyKnights)
{
    if (armyKnights->hasPaladinShield() && armyKnights->hasLancelotSpear() && armyKnights->hasGuinevereHair())
        armyKnights->setHasSword();
}

bool PickSword::loseEffect(ArmyKnights* armyKnights)
{
    return true;
}

Ultimecia::Ultimecia(BaseKnight* knight, int i, int e)
    : BaseOpponent(knight, i, e)
{
}

int Ultimecia::getFightResult()
{
    return 1;
}

void Ultimecia::winEffect(ArmyKnights* armyKnights)
{
    if (armyKnights->hasExcaliburSword())
        return;
    if (!(armyKnights->hasGuinevereHair() && armyKnights->hasPaladinShield() && armyKnights->hasLancelotSpear()))
    {
        armyKnights->setNumKnights(0);
        return;
    }
    int BossHP = 5000;
    while (armyKnights->lastKnight())
    {
        BaseKnight* knight = armyKnights->lastKnight();
        if (knight->getKnightType() == NORMAL)
        {
            break;
        }
        int HP = knight->getHp();
        int level = knight->getLevel();
        double base[] = { 0.06,0.05,0.075 };
        int damage = static_cast<int>(static_cast<double>(HP) * static_cast<double>(level) * base[knight->getKnightType()]);
        BossHP -= damage;
        if (BossHP <= 0)
            return;
        armyKnights->lastKnightHasDied();
    }
}

bool Ultimecia::loseEffect(ArmyKnights* armyKnights)
{

    return false;
}
