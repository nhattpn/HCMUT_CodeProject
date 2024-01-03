#include "main.h"
#include <fstream>

class imp_res : public Restaurant
{
	private:
		int count = 0, queue = 0;
		customer* X;
		customer* headQueue;
		customer* tailQueue;
		customer* headList;
		customer* tailList;
	public:	
		imp_res() {};
		customer* find(string name){
			customer* temp = X;
			for(int i=0;i<count;i++){
				if(temp->name == name) return temp;
				temp = temp->next;
			}
			return nullptr;
		}
		customer* findqueue(string name){
			customer* temp = headQueue;
			for(int i=0;i<queue;i++){
				if(temp->name == name) return temp;
				temp = temp->next;
			}
			return nullptr;
		}
		void refill(){
			while(queue > 0 && count < MAXSIZE){
				customer* temp = headQueue;
				if(queue > 1){
					headQueue = headQueue->next;
					headQueue->prev = nullptr;
				}
				string data = temp->name;
				int val = temp->energy;
				delete temp;
				added(data, val);
				queue--;
			}
		}
		void added(string name, int energy){
			customer *cus = new customer (name, energy, nullptr, nullptr);
			//them vao table
			if(count == 0){
				X = cus;
				X->next = X;
				X->prev = X;
				count++;
			}
			else if(count < MAXSIZE/2){
				if(cus->energy < X->energy){
					X->prev->next = cus;
					cus->prev = X->prev;
					cus->next = X;										
					X->prev = cus;
				}
				else{
					cus->next = X->next;										
					X->next->prev = cus;
					X->next = cus;					
					cus->prev = X;					
				}
				X = cus;
				count++;
			}
			else if(count < MAXSIZE){
				customer* inRES = X;//indexRES co value lon nhat trong sumcircletable
				int d = 1;
				int RES = cus->energy - X->energy;
				int maxRES = RES;
				X = X->next;				
				while(d<count){//tim RES vs inRES
					RES = cus->energy - X->energy;
					if(abs(RES) > abs(maxRES)){
						inRES = X;
						maxRES = RES;
					}
					X = X->next;
					d++;
				}
				X = inRES;
				if(maxRES < 0){
					X->prev->next = cus;
					cus->prev = X->prev;
					cus->next = X;
					X->prev = cus;
				}
				else{															
					X->next->prev = cus;
					cus->next = X->next;
					X->next = cus;					
					cus->prev = X;
				}
				X = cus;
				count++;
			}
			else{ //them vao headQueue
				if(queue == 0)	headQueue = cus;
				else{
					tailQueue->next = cus;
					cus->prev = tailQueue;
				}
				tailQueue = cus;
				queue++;
			}
		}
		bool greater(customer* a, customer* b){
			int a1 = -1,b1 = -1;
			customer* temp = headList;
			if(abs(a->energy) > abs(b->energy)) return 1;
			else if(abs(a->energy) < abs(b->energy)) return 0;
			else{
				for(int i=0;i<queue + count && (a1 == -1 || b1 == -1);i++){
					if(temp->name == a->name) a1 = i;
					else if(temp->name == b->name) b1 = i;
					temp = temp->next;
				}
			}
			return a1 < b1 ? 1 : 0;
		}
		customer* moveIt(customer* cus, int k, bool way){
			if(!way){//move left
				for(int i=0;i<k;i++) cus = cus->prev;
			}
			else{//move right
				for(int i=0; i<k;i++) cus = cus->next;
			}
			return cus;	
		}
		int insort(customer* head, int n, int k){
			int d = 0;
			customer* prelist = head;
			customer* list = head;
			for(int i = k; i < n; i+=k){
				list = moveIt(list, k, 1);
				customer* pretemp = prelist;
				customer* temp = list;				
				for(int j = i; j>=k && greater(temp, pretemp); j-=k){
					swap(temp->energy, pretemp->energy);
					swap(temp->name, pretemp->name);
					temp = pretemp;
					if(j-k >= k) pretemp = moveIt(pretemp, k, 0);
					d++;
				}
				prelist = list;
			}
			return d;
		}
		void RED(string name, int energy)
		{
			//cout<<"RED "<<name<<" "<<energy<<endl;
			if(energy == 0) return;			
			if(queue >= MAXSIZE || find(name) != nullptr || findqueue(name) != nullptr)	return;
			//them vao headList
			customer* cus = new customer(name, energy, nullptr, nullptr);
			if(count == 0) headList = cus;
			else{
				tailList->next = cus;
				cus->prev = tailList;
			}
			tailList = cus;
			added(name, energy);
		}
		void BLUE(int num)
		{
			customer* temp = headList;
			int d = 0;
			for(int i=0;i<count+queue && d < num;i++){			
				customer* cus = find(temp->name);
				if(cus != nullptr){
					customer* del = temp;
					if(del == headList){
						headList = headList->next;
						if(headList != nullptr){headList->prev = nullptr;}
					}
					else if(del == tailList){
						tailList = tailList->prev;
						tailList->next == nullptr;
					}
					else{
						del->prev->next = del->next;
						del->next->prev = del->prev;
					}
					
					if(cus->energy < 0) X = cus->prev;
					else X = cus->next;
					cus->prev->next = cus->next;
					cus->next->prev = cus->prev;					
					delete cus;
					delete del;
					d++;
				}
				temp = temp->next;
			}
			count-= (num > count) ? count : num;
			refill();
		}
		void PURPLE()
		{
			//cout <<"PURPLE"<<endl;
			if(queue <= 1) return;
			int i, j, val, d=0;
			int indexMax = 0;
			int maxCus = headQueue->energy;
			customer* temp = headQueue;
			for(i = 1; i<queue; i++){
				temp = temp->next;
				if(abs(temp->energy) >= abs(maxCus)){
					indexMax = i;
					maxCus = temp->energy;
				}
			}
			
			for(int i=(indexMax+1)/2; i>2; i/=2){
				customer* list = headQueue;
				for(int j=0; j<i; j++){
					d+=insort(list, indexMax+1-j, i);
					list = list->next;
				}
			}
			d+=insort(headQueue, indexMax+1, 1);
			BLUE(d % MAXSIZE);
		}
		void REVERSAL()
		{
			//cout << "REVERSAL" << endl;			
			customer* itpos = nullptr;
			customer* itneg = nullptr;
			customer* temp = X;
			string data;
			if(count > 0) data = X->name;	
			for(int i=0;i<count;i++){//add neg and pos to 2 other list
				customer* node = new customer(temp->name, temp->energy, nullptr, nullptr);
				if(temp->energy > 0){
					if(itpos == nullptr) itpos = node;
					else{
						itpos->next = node;
						node->prev = itpos;
						itpos = itpos->next;
					}
				}
				else{
					if(itneg == nullptr) itneg = node;
					else{
						itneg->next = node;
						node->prev = itneg;
						itneg = itneg->next;
					}
				}
				temp = temp->prev;
			}			
			for(int i=0; i<count;i++){//reverse the tablelist
				customer* del;
				if(temp->energy > 0){
					temp->energy = itpos->energy;
					temp->name = itpos->name;
					del = itpos;
					itpos = itpos->prev;
				}
				else{
					temp->energy = itneg->energy;
					temp->name = itneg->name;
					del = itneg;
					itneg = itneg->prev;
				}
				if(temp->name == data) X = temp;
				temp = temp->prev;
				delete del;
			}
		}
		void UNLIMITED_VOID()
		{
			//cout << "UNLIMITED_VOID" << endl;			
			if(count < 4) return;
			int length, maxlength;
			int sum = 0;
			customer* pretable = X;
			customer* posttable = X;
			for(int i=0;i<4;i++){//create array with 4 customer final
				sum += posttable->energy;
				posttable = posttable->next;
			}//postemp = pretable + 4
			customer* sumcircle = new customer(pretable->name, sum, nullptr, nullptr);//final of listsum (= index X)
			customer* listsum = sumcircle;//save min sum energy of finalnode of array
			customer* headlistlength;
			customer* listlength; //save list of maxlength of little array

			//pretable = X; posttable = X+4;
			for(int i=1;i<count;i++){ //create listsum have 4 customer
				sum = sum - pretable->energy + posttable->energy;
				customer* t = new customer(pretable->next->name, sum, nullptr, nullptr);
				listsum->next = t;
				listsum = listsum->next;
				pretable = pretable->next;
				posttable = posttable->next;
			}
			listsum = sumcircle;
			posttable = posttable->next;//pretable = X;posttable = X+3+1;
			for(int i=0;i<count;i++){ //reload listsum to have maxlength, create listlength
				sum = listsum->energy;	
				maxlength = 4;length = 4;
				customer* it = posttable;
				posttable = posttable->next;
				for(int j=0;j<count-4;j++){ //reload listsum		
					sum += it->energy;
					length++;
					if(sum <= listsum->energy){
						listsum->energy = sum;
						maxlength = length;
					}
					it = it->next;
				}
				if(i == 0){//create listlength
					listlength = new customer(listsum->name, maxlength, nullptr, nullptr);
					headlistlength = listlength;
				}
				else{
					customer* t = new customer(listsum->name, maxlength, nullptr, nullptr);
					listlength->next = t;
					listlength = listlength->next;
				}
				listsum = listsum->next;
				//posttable = listsum + 4							
			}//pretable = X; posttable = X+4;			
			int minsum = sumcircle->energy;
			maxlength = headlistlength->energy;
			listsum = sumcircle->next;
			listlength = headlistlength->next;
			//pretable stand, posttable moving
			
			pretable = X;
			posttable = X->next; //pre = X;
			for(int i=1;i<count;i++){ //find pretable have min energy and maxlength
				if(listsum->energy < minsum){
					minsum = listsum->energy;
					maxlength = listlength->energy;
					pretable = posttable;//posttable 
				}
				else if(listsum->energy == minsum && listlength->energy >= maxlength){
					maxlength = listlength->energy;
					pretable = posttable;
				}
				listsum = listsum->next;
				listlength = listlength->next;
				posttable = posttable->next;
			}//pretable is final of array of min, postable = X;
			customer* smallestCus;
			posttable = pretable;
			sum = 0;//reuse sum as a count
			for(int i=0;i<maxlength;i++){//find smallest energy of customer in the finded list
				if(i == 0) smallestCus = pretable;
				else if(posttable->energy < smallestCus->energy){
					smallestCus = posttable;
					sum = i;
				}
				posttable = posttable->next;
			}//pretable is final of array of min
			for(int i=0;i<maxlength;i++){//print from the smallest customer
				smallestCus->print();
				if(sum == maxlength - 1) smallestCus = pretable;
				else if(smallestCus->next == nullptr) smallestCus = X;
				else smallestCus = smallestCus->next;
				sum++;
			}
			//delete node
			for(int i=0;i<count;i++){
				listsum = sumcircle;
				sumcircle = sumcircle->next;
				listlength = headlistlength;
				headlistlength = headlistlength->next;
				delete listsum;
				delete listlength;
			}
		}
		void DOMAIN_EXPANSION()
		{
			//cout << "DOMAIN_EXPANSION" << endl;
			int both = 0;
			customer* temp = headList;
			//check duoi khach am hay duong trong table va queue .....
			for(int i=0;i<count+queue;i++){
				both += temp->energy;
				temp = temp->next;
			}
			both = (both>=0) ? 1 : -1;
			temp = tailList;
			bool final = 0, direc = 0;
			for(int i = count+queue-1; i>=0; i--){
				if((temp->energy * both) < 0){
					customer* del = temp;del->print();
					if(i == 0){
						headList = headList->next;
						if(headList != nullptr) headList->prev = nullptr;
						else tailList = nullptr;
					}
					else if(i == count + queue - 1){
						tailList = tailList->prev;
						tailList->next = nullptr;
						temp = temp->prev;
					}
					else{
						del->prev->next = del->next;
						del->next->prev = del->prev;
						temp = temp->prev;
					}
					customer* delQueue = findqueue(del->name);
					if(delQueue != nullptr){// delete in queue						
						if(delQueue->prev == nullptr){// = headqueue
							headQueue = headQueue->next;
							if(headQueue != nullptr) headQueue->prev = nullptr;
							else tailQueue = nullptr;
						}
						else if(delQueue->next == nullptr){// = tailqueue
							tailQueue = tailQueue->prev;
							tailQueue->next = nullptr;
						}
						else{
							delQueue->prev->next = delQueue->next;
							delQueue->next->prev = delQueue->prev;
						}
						delete delQueue;
						queue--;
					}
					else{//delete in table
						customer* delTable = find(del->name);
						if(count>1) {
							delTable->prev->next = delTable->next;
							delTable->next->prev = delTable->prev;							
							if(final == 0){
								final = 1;
								direc = delTable->energy > 0 ? 1 : 0;
								X = direc ? delTable->next : delTable->prev;
							}
							if(delTable == X) X = direc ? delTable->next : delTable->prev;
						}
						else X = nullptr;
						count--;
					}
					delete del;
				}
				else temp = temp->prev;
			}
			refill();
		}
		void LIGHT(int num)
		{
			if(count == 0) return;
			customer* table = X;
			customer* hqueue = headQueue;
			if(num > 0){
				for(int i=0;i<count;i++){
					table->print();
					table = table->next;
				}
			}
			else if(num < 0){
				for(int i=0;i<count;i++){
					table->print();
					table = table->prev;
				}
			}
			else{
				hqueue = headQueue;
				for(int i=0;i<queue;i++){
					hqueue->print();
					hqueue = hqueue->next;
				}
			}
			
		}

		
};