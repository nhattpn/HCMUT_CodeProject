#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__
#include "main.h"
// #define DEBUG
class BaseKnight;
class BaseItem;
class Events;
enum ItemType {Antidote = 0, PhoenixDownI, PhoenixDownII, PhoenixDownIII, PhoenixDownIV};
class Baglink{
public:
    BaseItem* data = nullptr;
    Baglink* next = nullptr;
};
class BaseBag {
protected:
    BaseKnight* knight;
    Baglink* head = nullptr;
    int maxSize = 0, ownItems = 0;
public:
    BaseBag(BaseKnight* knight);
    virtual bool insertFirst(BaseItem * item);
    virtual BaseItem * get(ItemType itemType);
    virtual string toString() const;    
    virtual void remove();
    ~BaseBag();
};
class BagPaladinKnight : public BaseBag{
public:
    BagPaladinKnight(BaseKnight* knight) : BaseBag(knight) {maxSize = -1;}
};
class BagLancelotKnight : public BaseBag{
public:
    BagLancelotKnight(BaseKnight* knight) : BaseBag(knight) {maxSize = 16;}
};
class BagDragonKnight : public BaseBag{
public:
    BagDragonKnight(BaseKnight* knight) : BaseBag(knight) {maxSize = 14;}
};
class BagNormalKnight : public BaseBag{
public:
    BagNormalKnight(BaseKnight* knight) : BaseBag(knight) {maxSize = 19;}
};
class BaseItem {
public:
    virtual bool canUse ( BaseKnight * knight ) = 0;
    virtual void use ( BaseKnight * knight ) = 0;
    virtual ItemType type() = 0;
    virtual string ItemString() = 0;
};
class ItemAntidote: public BaseItem{
public:
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
    ItemType type() { return Antidote;}
    string ItemString() { return "Antidote";}
};
class ItemPhoenixDownI: public BaseItem{
public:
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
    ItemType type() { return PhoenixDownI; }
    string ItemString() { return "PhoenixI"; }
};
class ItemPhoenixDownII: public BaseItem{
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
    ItemType type() { return PhoenixDownII;}
    string ItemString() { return "PhoenixII";}
};
class ItemPhoenixDownIII: public BaseItem{
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
    ItemType type() { return PhoenixDownIII;}
    string ItemString() { return "PhoenixDownIII";}
};
class ItemPhoenixDownIV: public BaseItem{
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
    ItemType type() {return PhoenixDownIV;}
    string ItemString() {return "PhoenixIV";} 
};
// Opponent
enum OpponentType {MadBear = 1, Bandit, LordLupin, Elf, Troll, Tornbery, 
QueenOfCards, NinaDeRings, DurianGarden, OmegaWeapon, Hades};
class BaseOpponent{
protected:
    int levelO;
public:
    int getLevelO(){return levelO;}
    void setLevelO(int idEvent){
        levelO = (idEvent + int(Type()))%10 + 1;
    };
    virtual OpponentType Type() = 0;
};
class MadBearOpponent: public BaseOpponent{
public:
    OpponentType Type() {return MadBear;}
    
};
class BanditOpponent: public BaseOpponent{
public:
    OpponentType Type() {return Bandit;}
};
class LordLupinOpponent: public BaseOpponent{
public:
    OpponentType Type() {return LordLupin;}
};
class ElfOpponent: public BaseOpponent{
public:
    OpponentType Type() {return Elf;}    
};
class TrollOpponent: public BaseOpponent{
public:
    OpponentType Type() {return Troll;}    
};
class TornberyOpponent: public BaseOpponent{
public:
    OpponentType Type() {return Tornbery;}    
};
class QueenOfCardsOpponent: public BaseOpponent{
public:
    OpponentType Type() {return QueenOfCards;}
    
};
class NinaDeRingsOpponent: public BaseOpponent{
public:
    OpponentType Type() {return NinaDeRings;}
    
};
class DurianGardenOpponent: public BaseOpponent{
public:
    OpponentType Type() {return DurianGarden;}
    
};
class OmegaWeaponOpponent: public BaseOpponent{
public:
    OpponentType Type() {return OmegaWeapon;}
    
};
class HadesOpponent: public BaseOpponent{
public:
    OpponentType Type() {return Hades;}
    
};
enum KnightType { PALADIN = 0, LANCELOT, DRAGON, NORMAL };
class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;

    BaseBag *bag;
    KnightType knightType;
    bool poison = 0;
public:
    int getHp() {return hp;}
    int getLevel() {return level;}
    int getMaxHp() {return maxhp;}
    bool getPoisoned() {return poison;}
    int getGil() {return gil;}
    
    void setHp(int hp);
    void setLevel(int level);
    void setPoisoned(bool poison);
    void setGil(int gil);
    bool setBag(BaseItem* item);
public:
    BaseKnight(){}
    static BaseKnight *create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    virtual bool fight(BaseOpponent *opponent);
    virtual int knightBaseDamage() = 0;
    string toString() const;
    ~BaseKnight();
};
class PaladinKnight : public BaseKnight{
public:
    PaladinKnight() {knightType = PALADIN;}
    int knightBaseDamage() {return 0.06*level *hp;}
};
class LancelotKnight : public BaseKnight{
public:
    LancelotKnight() {knightType = LANCELOT;}
    int knightBaseDamage(){return 0.05*level *hp;}
};
class DragonKnight : public BaseKnight{
public:
    DragonKnight() {knightType = DRAGON;}
    int knightBaseDamage(){return 0.075*level *hp;};
};
class NormalKnight : public BaseKnight{
public:
    NormalKnight() {knightType = NORMAL;}
    int knightBaseDamage(){return 0;};
};
class ArmyKnights {
private:
    BaseKnight** KnightList;
    int amount;    
public:
    ArmyKnights (const string & file_armyknights);
    ~ArmyKnights();
    static bool PaladinShield, LancelotSpear, GuinevereHair, ExcaliburSword;
    static bool OmegaMatch; 
    static bool HadesMatch;
    bool adventure (Events * events);    
    int count() const { return this->amount;}
    BaseKnight * lastKnight() const { return this->KnightList[amount - 1];}
    bool hasPaladinShield() const { return PaladinShield;}
    bool hasLancelotSpear() const { return LancelotSpear;}
    bool hasGuinevereHair() const { return GuinevereHair;}
    bool hasExcaliburSword() const { return ExcaliburSword;}
    bool fight(BaseOpponent * opponent);
    void printInfo() const;
    void printResult(bool win) const;
};
class Events {
private:
    int *EventList;
    int n;
public:
    Events(string file_events);
    int count() const { return n;}
    int get(int i) const { return EventList[i];}
    ~Events() { delete[] EventList;}
};
class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;
public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:    
    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

#endif // __KNIGHT2_H__