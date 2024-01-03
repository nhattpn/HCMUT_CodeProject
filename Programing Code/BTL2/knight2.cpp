#include "knight2.h"
bool ArmyKnights::HadesMatch = 0;
bool ArmyKnights::OmegaMatch = 0;
bool ArmyKnights::PaladinShield = 0;
bool ArmyKnights::LancelotSpear = 0;
bool ArmyKnights::GuinevereHair = 0;
bool ArmyKnights::ExcaliburSword = 0;
/* * * BEGIN implementation of class BaseBag * * */
BaseBag::BaseBag(BaseKnight* knight){
    this->knight = knight;
    maxSize = 0;
    ownItems = 0;
    head = nullptr;    
}
bool BaseBag::insertFirst(BaseItem * item){
    if(ownItems == maxSize) return false;
    Baglink* temp = new Baglink();
    temp->data = item;
    temp->next = head;
    head = temp;
    ownItems ++;
    return true;
}
BaseItem * BaseBag::get(ItemType itemType){
    if(ownItems == 0) return nullptr;
    Baglink* temp = head;
    if(itemType == Antidote){        
        while(temp != nullptr && temp->data->type() != itemType) temp = temp->next;        
    }
    else{
        while(temp && !temp->data->canUse(knight)) temp = temp->next;        
    }

    if(temp != nullptr){
        swap(head->data, temp->data);
        ownItems--;
        temp = head;
        head = head->next;
        temp->data->use(knight);
        delete temp->data;
        delete temp;
        return nullptr;
    }
    return nullptr;
}
string BaseBag::toString() const{
    if(ownItems == 0) return "Bag[count=0;]";
    string result = "Bag[count=" + to_string(ownItems) + ";";
    string NameItem[] = {"Antidote", "PhoenixI", "PhoenixII", "PhoenixIII", "PhoenixIV"};
    Baglink* temp = head;
    while(temp){
        result += NameItem[temp->data->type()] + ",";
        temp = temp->next;
    }
    result[result.length() - 1] = ']';
    return result;    
}
void BaseBag::remove(){
    if(ownItems>0){        
        Baglink* temp = head;
        head = head->next;
        delete temp->data;
        delete temp;
        ownItems--;
    }
}
BaseBag::~BaseBag(){
    while(ownItems>0){        
        Baglink* temp = head;
        head = head->next;
        delete temp->data;
        delete temp;
        ownItems--;
    }
}

/* * * END implementation of class BaseBag * * */
/* * * BEGIN implementation of class BaseItem * * */
bool ItemAntidote::canUse(BaseKnight* knight)      {return knight->getPoisoned();}
void ItemAntidote::use(BaseKnight* knight)         {knight->setPoisoned(false); }

bool ItemPhoenixDownI::canUse(BaseKnight* knight)  {return (knight->getHp() <= 0); }
void ItemPhoenixDownI::use(BaseKnight* knight)     {knight->setHp(knight->getMaxHp()); }

bool ItemPhoenixDownII::canUse(BaseKnight* knight) {return knight->getHp() < knight->getMaxHp() / 4;}
void ItemPhoenixDownII::use(BaseKnight* knight)    {knight->setHp(knight->getMaxHp());}

bool ItemPhoenixDownIII::canUse(BaseKnight* knight){return knight->getHp() < knight->getMaxHp() / 3;}
void ItemPhoenixDownIII::use(BaseKnight* knight)   {
    if(knight->getHp() <= 0) knight->setHp(knight->getMaxHp()/3);
    else                     knight->setHp( knight->getMaxHp()/4 + knight->getHp() );
}
bool ItemPhoenixDownIV::canUse(BaseKnight* knight) {return knight->getHp() < knight->getMaxHp() / 2;}
void ItemPhoenixDownIV::use(BaseKnight* knight)    {
    if(knight->getHp() <= 0) knight->setHp(knight->getMaxHp()/2);
    else                     knight->setHp(knight->getMaxHp()/5 + knight->getHp());    
}
/* * * END implementation of class BaseItem * * */
/* * * BEGIN implementation of class BaseKnight * * */
BaseKnight *BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI){
    BaseKnight* knight;
    if(maxhp == 888){
        knight = new LancelotKnight();
        knight->bag = new BagLancelotKnight(knight);
    }
    else if(maxhp == 345 || maxhp == 354 || maxhp == 435 || maxhp == 453 || maxhp == 534 || maxhp == 543){
        knight = new DragonKnight();
        knight->bag = new BagDragonKnight(knight);
    }
    else{
        int primeCheck = 0;
        for(int i = 2; i <= sqrt(maxhp); i++) primeCheck += (maxhp % i == 0);
        if(primeCheck == 0){
            knight = new PaladinKnight();    
            knight->bag = new BagPaladinKnight(knight);   
        }  
        else{
            knight = new NormalKnight();
            knight->bag = new BagNormalKnight(knight);
        }
    }
    knight->id = id;
    knight->maxhp = maxhp;
    knight->hp = maxhp;
    knight->level = level;
    knight->gil = gil;
    while(phoenixdownI--){
        BaseItem* item = new ItemPhoenixDownI();
        if(!knight->bag->insertFirst(item)) delete item;
    }
    while(antidote-- && knight->knightType != DRAGON){
        BaseItem* item = new ItemAntidote();
        if(!knight->bag->insertFirst(item)) delete item;        
    }
    return knight;
}
bool BaseKnight::fight(BaseOpponent * opponent){
    int OppName = int(opponent->Type());
    int levelO = opponent->getLevelO();
    if(OppName >= 1 && OppName <=5){
        if(knightType == PALADIN || knightType == LANCELOT || levelO <= level) {
            switch(OppName){
                case 1: gil += 100; break;
                case 2: gil += 150; break;
                case 3: gil += 450; break;
                case 4: gil += 750; break;
                case 5: gil += 800; break;
            }
        }
        else{
            switch(OppName){
                case 1: hp -= 10*(levelO - level); break;
                case 2: hp -= 15*(levelO - level); break;
                case 3: hp -= 45*(levelO - level); break;
                case 4: hp -= 75*(levelO - level); break;
                case 5: hp -= 95*(levelO - level); break;
            }
            bag->get(PhoenixDownI);
        }
    }
    else if(OppName == 6){
        if(levelO <= level){
            if(level < 10) level ++;
        }
        else if(knightType != DRAGON){
            poison = true;
            bag->get(Antidote);
            if(poison){
                poison = false;
                bag->remove();
                bag->remove();
                bag->remove();
                hp -= 10;
                bag->get(PhoenixDownI);
            }
        }
    }
    else if(OppName == 7){
        if(levelO <= level) gil *= 2;
        else if(knightType != PALADIN) gil = (gil / 2 );
    }
    else if(OppName == 8 && hp < maxhp/3){
        if(knightType == PALADIN) hp += maxhp/5;
        else if(gil >= 50){
            gil -= 50;
            hp += maxhp/5;
        }
    }
    else if(OppName == 9) hp = maxhp;
    else if(OppName == 10 && !ArmyKnights::OmegaMatch){
        if((level == 10 && hp == maxhp) || knightType == DRAGON){
            level = 10;
            gil = 999;
            ArmyKnights::OmegaMatch = true;
        }
        else{
            hp = 0;
            bag->get(PhoenixDownI);  
        }     
    }
    else if(OppName == 11 && !ArmyKnights::HadesMatch){
        if(level == 10 || (knightType == PALADIN && level >= 8)){
            ArmyKnights::HadesMatch = true;
            ArmyKnights::PaladinShield = true;
        }
        else{
            hp = 0;
            bag->get(PhoenixDownI);  
        }     
    }
    if(hp <= 0 && gil >= 100){
        hp = maxhp / 2;
        gil -= 100;
    }
    return hp > 0;
}
string BaseKnight::toString() const {
    string KnightName[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    string s;
    s = "[Knight:id:" + to_string(id) 
        + ",hp:" + to_string(hp) 
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + KnightName[knightType]
        + "]";
    return s;
}

void BaseKnight::setHp(int hp){
    if(this->hp > hp){
        this->hp = hp;
        bag->get(PhoenixDownI);
        if(this->hp <= 0 && gil >= 100){
            this->hp = maxhp / 2;
            gil -= 100;
        }
    }
    else{
        this->hp = hp;
        if(hp > maxhp) this->hp = maxhp;
    }
}
void BaseKnight::setLevel(int level)      {this->level = (level > 10) ? 10 : level;}
void BaseKnight::setPoisoned(bool poison) {this->poison = poison;} 
void BaseKnight::setGil(int gil)          {this->gil = gil;}
bool BaseKnight::setBag(BaseItem* item)   {return bag->insertFirst(item);}

BaseKnight::~BaseKnight(){
    delete bag;
}
/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << count();
    if (count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}
void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}
ArmyKnights::ArmyKnights (const string & file_armyknights){
    ifstream input(file_armyknights);
    input >> amount;
    KnightList = new BaseKnight*[amount+1];
    int maxhp, level, phoenixdownI, gil, antidote;
    for(int id = 0; id < amount; id++){
        input >> maxhp >> level >> phoenixdownI >> gil >> antidote;
        KnightList[id] = BaseKnight::create(id+1, maxhp, level, gil,antidote, phoenixdownI);
    }    
}
bool ArmyKnights::fight(BaseOpponent * opponent){
    if(amount > 0){
        BaseKnight *lknight = lastKnight();
        if(lknight->fight(opponent)){
            for(int i = amount-1; i>=0 ; i--){
                if(KnightList[i]->getGil() <= 999) break;
                int exceed = KnightList[i]->getGil() - 999;
                KnightList[i]->setGil(999); 
                if(i > 0) KnightList[i-1] -> setGil(KnightList[i-1]->getGil() + exceed);                
            }
        }
        else{
            amount--;
            delete lknight;
        }
    }
    delete opponent;
    return amount;
}
bool ArmyKnights::adventure (Events * events){
    for(int idEvent = 0; idEvent < events->count(); idEvent++){
        int Event = events->get(idEvent);
        switch(Event){
            case 1: {BaseOpponent *opponent = new MadBearOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 2: {BaseOpponent *opponent = new BanditOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 3: {BaseOpponent *opponent = new LordLupinOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 4: {BaseOpponent *opponent = new ElfOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 5: {BaseOpponent *opponent = new TrollOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 6: {BaseOpponent *opponent = new TornberyOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 7: {BaseOpponent *opponent = new QueenOfCardsOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 8: {BaseOpponent *opponent = new NinaDeRingsOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 9: {BaseOpponent *opponent = new DurianGardenOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 10:{ BaseOpponent *opponent = new OmegaWeaponOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 11:{ BaseOpponent *opponent = new HadesOpponent(); 
                    opponent->setLevelO(idEvent);
                    fight(opponent);
                break;}
            case 95: PaladinShield = true; break;
            case 96: LancelotSpear = true; break;
            case 97: GuinevereHair = true; break;
            case 98: if(PaladinShield && LancelotSpear && GuinevereHair) ExcaliburSword = true; break;
            case 99: {
                if(ExcaliburSword){
                    printInfo();
                    return true;
                }
                else if(!GuinevereHair || !LancelotSpear || !PaladinShield){
                    for(int i = 0; i < amount ; i++) delete KnightList[i];
                    amount = 0;
                    printInfo();
                    return false;    
                }
                else{
                    int HpUltimecia = 5000;
                    BaseKnight * last = NULL;
                    int skipKnight = 0;
                    while(amount > 0){                        
                        BaseKnight * knight = lastKnight();
                        HpUltimecia -= knight->knightBaseDamage();
                        if(knight->knightBaseDamage() == 0){
                            skipKnight++;
                            if(last == NULL) last = knight;
                            else delete knight;
                        }
                        else{
                            if(HpUltimecia <= 0){
                                amount += skipKnight;
                                if(last != NULL) KnightList[amount - 1] = last;
                                printInfo();
                                if(last != NULL) delete last;
                                return true;
                            }
                            else delete knight;
                        }
                        amount--;
                    }
                    if(last != NULL) delete last;
                    printInfo();
                    return false;
                }
                break;
            }
        }
        if(Event == 112 || Event == 113 || Event == 114){
            BaseItem* item = nullptr;
            if(Event == 112) item = new ItemPhoenixDownII();  
            else if(Event == 113) item = new ItemPhoenixDownIII();
            else if(Event == 114) item = new ItemPhoenixDownIV();
            bool insert = false;
            for(int j = amount - 1; j >= 0; j--){
                if(KnightList[j]->setBag(item)) {
                    insert = true;
                    break;
                }
            }
            if(insert == false) delete item;
        }
        printInfo();
        if(amount == 0) return false;
    }
    return true;
};
ArmyKnights::~ArmyKnights(){
    for(int i = 0; i < amount; i++) delete KnightList[i];
    delete[] KnightList;
}
/* * * END implementation of class ArmyKnights * * */
Events::Events(string file_events){
    ifstream input(file_events);
    input >> n;
    EventList = new int[n];
    for(int i = 0; i < n; i++) input >> EventList[i];
}
/* * * BEGIN implementation of class KnightAdventure * * */

KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}
void KnightAdventure::loadArmyKnights(const string & file_armyknights){armyKnights = new ArmyKnights(file_armyknights);}
void KnightAdventure::loadEvents(const string & file_events){events = new Events(file_events);}
void KnightAdventure::run(){
    armyKnights->printResult(armyKnights->adventure(events));
}
KnightAdventure::~KnightAdventure(){
    delete armyKnights;
    delete events;
}
/* * * END implementation of class KnightAdventure * * */