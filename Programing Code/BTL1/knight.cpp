#include "knight.h"
int old_lv, bad_effect=0, maxHP;
int a1[5];
bool tiny=0, frog=0, mely=0, ascl=0, lancer=0, arthur=0;
string s1,s2,s3, file1,file2,file3;
void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue) {
    cout << "HP=" << HP
        << ", level=" << level
        << ", remedy=" << remedy
        << ", maidenkiss=" << maidenkiss
        << ", phoenixdown=" << phoenixdown
        << ", rescue=" << rescue << endl;
}
void retiny(int &HP){
    if(tiny==1){
        HP = HP*5>maxHP ? maxHP : HP*5;
        tiny=0;
        bad_effect=0;
    }
}
void refrog(int &level){
    if(frog==1){
        level = old_lv;
        frog=0;
        bad_effect=0;
    }
}
bool revive(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue){
    if(phoenixdown>0){
        if(bad_effect>0){                    
            retiny(HP);
            refrog(level);
            }
        phoenixdown--;
        HP=maxHP;
        return 1;
    }
    else{
        rescue = 0;
        return 0;
    }
}
bool snt(int n){
    if (n < 2) {
        return false;
    }
    int limit = sqrt(n);
    for (int i = 2; i <= limit; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
void Shaman_6(int &HP, int &level, int &remedy, int pw){
    if(pw < level || lancer || arthur){
        level= level+2 > 10 ? 10 : level+2;
    }
    else if(pw > level){
        if(remedy>0) remedy--;
        else{
            HP = HP/5>1 ? HP/5:1;
            bad_effect=3;
            tiny=1;
        }
    }
}
void Vajsh_7(int &level, int &maidenkiss, int pw){
    if(pw < level || lancer || arthur){
        level= level+2 > 10 ? 10 : level+2;
    }
    else if(pw > level){
        if(maidenkiss>0) maidenkiss--;
        else{
            bad_effect=3;
            old_lv=level;
            level=1;
            frog=1;            
        }
    }
}
void Mario_11(int &HP, int level, int phoenixdown){
    int n1, s1=0;
    n1 = ((level + phoenixdown)%5 + 1) * 3;
    for(int i=0;i<n1;i++){
        s1+= (99-i*2);
    }
    HP+=(s1%100);
    if(HP%2==0) HP++;
    else HP+=2;
    while(!snt(HP)) HP+=2;
    if(HP>maxHP) HP=maxHP;
}
void Fibo_12(int &HP){
    int f1=1,f2=1,f3;
    if(HP>1){
        while(f1+f2<HP){
            f3=f1+f2;
            f1=f2;
            f2=f3;
        }
        if(HP!=2) HP=f3;
        else HP=1;
    }    
}
void Ghost_13(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue, int s){
    ifstream ghost(file1);
    string f1,f2,temp;
    getline(ghost,f1);
    getline(ghost,f2);
    int n=stoi(f1);   
    int a[n], p=0;
    stringstream ss(f2);
    while(getline(ss,temp,',')){
        a[p]=stoi(temp);
        p++;
    }
    string num13= to_string(s);
    int maxi=0, mini=0, maxi2=0, mini2=0, mtx, mti, max2_3x, max2_3i;
    int max, min, Arr[p];
    for(int j=0;j<p;j++){
        if(a[j]<0) Arr[j]=-a[j];
        else Arr[j]=a[j];
        Arr[j]=(17*Arr[j]+9)%257;
    }
    for(int i=2;i<num13.size();i++){        
        switch(num13[i]-'0'){
            case 1:{
                max=a[0];
                min=a[0];
                for(int j=0;j<p;j++){
                    if((a[j]>=max)){
                        max=a[j];
                        maxi=j;
                    }
                    if((a[j]<=min)){
                        min=a[j];
                        mini=j;                        
                    }
                }
                HP-=(maxi+mini);
                break;
            }
            case 2:{
                int j;j=2;
                mtx=a[0];mti=0;
                if(a[0]<a[1]){                                        
                    while(j<p){
                        if(a[j-1]<a[j]){
                            mtx=a[j];
                            mti=j;
                        }
                        else if(a[j-1]>a[j]) break;
                        else goto togo;
                        j++;
                    }
                    while(j<p){
                        if(a[j-1]<=a[j]) goto togo;
                        j++;
                    }  
                }
                else if(a[0]>a[1]){
                    while(j<p){
                        if(a[j-1]<=a[j]) goto togo;
                        j++;
                    }
                }
                else {
                    togo:
                    mtx = -2; mti = -3;
                }
                HP-=(mtx+mti);
                break;
            }
            case 3:{
                for(int j=0;j<p;j++){
                    if(a[j]<0) Arr[j]=-a[j];
                    else Arr[j]=a[j];
                    Arr[j]=(17*Arr[j]+9)%257;
                }
                max=Arr[0];min=Arr[0];
                for(int j=0;j<p;j++){
                    if(Arr[j]>max){
                        max=Arr[j];
                        maxi2=j;
                    }
                    if(Arr[j]<min){
                        min=Arr[j];
                        mini2=j;                        
                    }
                }
                HP-=(maxi2+mini2);
                break;
            }
            case 4:{           
                if(p>=3){
                    if(Arr[0]!=Arr[1] || Arr[1]!=Arr[2]){
                        int mx= Arr[0];
                        for(int j=1;j<3;j++){
                            if(mx<Arr[j]) mx=Arr[j];
                        }
                        max2_3i=-7;
                        for(int i=0;i<3;i++){
                            if(Arr[i]!=mx && (max2_3i==-7 || Arr[i]>Arr[max2_3i])){
                                max2_3i =i;
                            }
                        }
                        max2_3x=Arr[max2_3i];
                    }
                    else{
                        max2_3x=-5;
                        max2_3i=-7;
                    }
                }
                else{
                    if(p<2 || Arr[0]==Arr[1]){
                        max2_3x=-5;
                        max2_3i=-7;
                    }
                    else{
                        max2_3x = Arr[0] > Arr[1] ? Arr[1] : Arr[0];
                        max2_3i = Arr[0] > Arr[1] ? 1 : 0;
                    }
                }
                HP-=(max2_3i+max2_3x);
                break;}
            default: break; 
        }
        if(HP>maxHP) HP=maxHP;        
        if(HP<=0 && !revive(HP, level, remedy, maidenkiss, phoenixdown, rescue)) break;                    
    }
}
void Merlin_18(int &HP){
    ifstream mer(file3);
    string f1,f2,temp;
    getline(mer,f1);
    int n=stoi(f1);
    for(int i=0;i<n;i++){
        getline(mer,f2);
        int j=0;
        bool b[6];
        for(int i=0;i<6;i++){
            b[i]=0;
        }
        while(j<f2.size()){
            if(f2[j]=='M' || f2[j]=='m'){
                if(temp.empty()) temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[0]=1;
            }
            else if(f2[j]=='E' || f2[j]=='e'){
                if((temp.back()=='M' || temp.back()=='m') && f2[j]=='e') temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[1]=1;
            }
            else if(f2[j]=='R' || f2[j]=='r'){
                if(temp.back()=='e' && f2[j]=='r') temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[2]=1;
            }
            else if(f2[j]=='L' || f2[j]=='l'){
                if(temp.back()=='r' && f2[j]=='l') temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[3]=1;
            }
            else if(f2[j]=='I' || f2[j]=='i'){
                if(temp.back()=='l' && f2[j]=='i') temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[4]=1;
            }
            else if(f2[j]=='N' || f2[j]=='n'){
                if(temp.back()=='i' && f2[j]=='n') temp+=f2[j];
                else if(temp.size()==6) break;
                else temp.clear();
                b[5]=1;
            }
            else{
                if(temp.size()==6) break;
                temp.clear();                                                                                                                               
            }
            j++;
        }
        if(temp.size()==6){
            HP+=3;
            if(HP>maxHP) HP=maxHP;
        }
        else{
            bool m=1;
            for(int i=0;i<6;i++){
                if(b[i]==0){
                    m=0;
                    break;
                }
            }
            if(m==1){
                HP+=2;
                if(HP>maxHP) HP=maxHP;
            }
        }
        temp.clear();
    }
    mely=1;
}
void Asclepius_19(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown){
    ifstream Ascle(file2);
    int r1,c1,temp;
    Ascle>>r1;
    Ascle>>c1;
    for(int i=0;i<r1;i++){
        int num=0;       
        for(int j=0;j<c1;j++){
            Ascle>>temp;
            if(num==3) continue;
            switch(temp){
                case 16: if(remedy<99) remedy++;
                        num++;
                    break;
                case 17: if(maidenkiss<99) maidenkiss++;
                        num++;
                    break;
                case 18: if(phoenixdown<99) phoenixdown++;
                        num++;
                    break;
                default: break;
            }
        }
    }
    if(remedy>0) retiny(HP);
    if(maidenkiss>0) refrog(level);
    ascl=1;
}
void Combat(int &HP, int &level, int num_mons, int lv_mons){
    int b=lv_mons%10;
    int pw = lv_mons > 6?(b>5 ? b:5) : b;
    if(pw < level || lancer || arthur){
        level= level+1 > 10 ? 10 : level+1;
    }
    else if(pw > level){
        float dmg;
        switch(num_mons){
            case 1: dmg = 1; break;
            case 2: dmg = 1.5; break;
            case 3: dmg = 4.5; break;
            case 4: dmg = 7.5; break;
            case 5: dmg = 9.5; break;
            default: break;
        }
        dmg=dmg*pw*10;
        HP-=dmg;
    }
}
void big_mobs(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue, int num_mons, int lv_mons){
    int b=lv_mons%10;
    int pw = lv_mons > 6?(b>5 ? b:5) : b;
    switch(num_mons){
        case 6: if(bad_effect==0)
                Shaman_6(HP, level, remedy, pw);
            break;
        case 7: if(bad_effect==0)
                Vajsh_7(level, maidenkiss, pw);
            break;
        case 11: Mario_11(HP, level, phoenixdown);
            break;
        case 12: Fibo_12(HP);
            break;
        default:
            Ghost_13(HP, level, remedy, maidenkiss, phoenixdown, rescue, num_mons);
            break;
    }
}
void potion(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int num_mons){
    switch (num_mons){
        case 15: if(remedy<99) remedy++;
                if(bad_effect>0){
                    retiny(HP);
                    remedy=0;
                }
            break;
        case 16: if(maidenkiss<99) maidenkiss++;
                 if(bad_effect>0){
                    refrog(level);
                    maidenkiss=0;
                 }
            break;
        case 17: if(phoenixdown<99) phoenixdown++;
                 if(bad_effect>0){
                    phoenixdown=0;
                    retiny(HP);
                    refrog(level);
                    HP=maxHP;
                 }
            break;
        case 18: if(mely==0) Merlin_18(HP);            
            break;
        case 19: if(ascl==0) Asclepius_19(HP, level, remedy, maidenkiss, phoenixdown);
            break;
        default: break;
    }
}
void readline1(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown){
    int k=0;
    stringstream ss(s1);
    while(ss>>a1[k]) k++;
    HP=a1[0]; maxHP=HP;
	level=a1[1];
	remedy=a1[2];
	maidenkiss=a1[3];
	phoenixdown=a1[4];
    if(HP==999) arthur=1;
    else if(snt(HP)) lancer=1;
}
void readline2(int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue){
    int num_mons=0, lv_mons=1,temp, it=0, i=0;
    stringstream ss(s2),ss2(s2);
    while(ss2>>temp) it++;
    while(ss>>num_mons){
        if(num_mons == 0){
            rescue = 1;
            display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
            break;
        }
        else if(num_mons <= 5) Combat(HP, level, num_mons, lv_mons);
        else if(num_mons <= 12 || num_mons >130) big_mobs(HP, level, remedy, maidenkiss, phoenixdown, rescue, num_mons, lv_mons);
        else if(num_mons <=19) potion(HP, level, remedy, maidenkiss, phoenixdown, num_mons);
        else if(num_mons == 99){
            if(arthur) level=10;
            else if(lancer){
                if(level >=8 ) level=10;
                else{
                    rescue=0;
                    display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                    break;
                }
            }
            else if(level<10){
                rescue=0;
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                break;
            }
        }
        if(HP<=0){
            if(!revive(HP, level, remedy, maidenkiss, phoenixdown, rescue)){
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                break;
            }
        }
        if(bad_effect>0){
            bad_effect--;
        }
        else{
            retiny(HP);
            refrog(level);
        }
        i++;        
        if(i==it) rescue=1;
        display(HP, level, remedy, maidenkiss, phoenixdown, rescue);       
        lv_mons++;
        num_mons=0;
    }
}
void adventureToKoopa(string file_input, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue) {
    ifstream in(file_input);
    rescue=-1;	
    getline(in, s1);
    getline(in, s2);
    getline(in, s3);
    int i=0;
    stringstream ss(s3);
    getline(ss,file1,',');
    getline(ss,file2,',');  
    getline(ss,file3,',');  
    readline1(HP, level, remedy, maidenkiss, phoenixdown);
    readline2(HP, level, remedy, maidenkiss, phoenixdown, rescue);
}
