#include "main.h"

int MAXSIZE = 0;
ofstream solution;
bool smaller(pair<char,int> l, pair<char,int> r){//return true if smaller
	if(l.second > r.second) return 1;
	if(l.second == r.second){
		if(isupper(l.first) && islower(r.first)) return 1;
		if(islower(l.first) && isupper(r.first)) return 0;
		if(l.first > r.first) return 1;
	}
	return 0;
}

class restaurant_Gojo{
	class Tree_BST;
private:
	vector<Tree_BST> areaTable;
public:
	restaurant_Gojo():areaTable(MAXSIZE + 1){}
	void insertAreaTable(int result){
		int ID = result % MAXSIZE + 1;
		areaTable[ID].insert(result);
	}

	void remove_KOKUSEN(){
		for(int i = 1; i < MAXSIZE + 1; i++) areaTable[i].remove();
 	}

	void print_LIMITLESS(int number){
		if(number <= 0 || number > MAXSIZE) return;
		areaTable[number].print();
	}
	~restaurant_Gojo(){}
private:
	class Tree_BST{
		class Node;
	private:
		Node* root;	//! cây của khách hàng vị trí khách hàng
		queue<int> queueTime; //! thời gian khách hàng đến có thể hiểu như là sổ ghi thông tin khách hàng
	public:
		Tree_BST():root(nullptr){}
		int size(){
			return queueTime.size();
		}

		Node* insertLoop(Node* node, int result){
			//TODO TODO TODO  TODO TODO TODO 			
			if(node == nullptr)	return new Node(result);
			else if(result < node->result) node->left = insertLoop(node->left, result);
			else node->right = insertLoop(node->right, result);
			return node;
		}
		void insert(int result){
			queueTime.push(result);
			root = insertLoop(root, result);
		}
		
		Node *minValueNode(Node *node){
    		Node *current = node;
    		while (current && current->left != nullptr) current = current->left;
    		return current;
		}
		int CountNode(Node* node)	{
			return node == NULL ? 0 : 1 + CountNode(node->left) + CountNode(node->right);
		}
		unsigned long long factorial(int num) {
    		if (num <= 0) return 1;        		
    		return num * factorial(num - 1);
		}
		unsigned long long combinationFomula(int n, int x)	{
			//!TODO TÍNH C(n,x)= n!/x!(n-x)! công thức tổ hợp
			unsigned long long result = 1;
			for(int i=x+1;i<=n;i++) result *= i;
			for(int i=2;i<=(n-x);i++) result /= i;
			result = (result == 0) ? 1 : result;
    		return result;
		}
		unsigned long long DFS(Node* node){
			if(node == NULL) return 1;
			int x = CountNode(node->left);
			int n = CountNode(node->right) + x;//cout<<x<<node->result<<n<<" ";
			return (combinationFomula(n, x) * DFS(node->left) * DFS(node->right));
		}
		Node* removeLoop(Node* node,int result){
			if (node == nullptr) return node;		        
    		if (result < node->result) node->left = removeLoop(node->left, result);
    		else if (result > node->result) node->right = removeLoop(node->right, result);	        	
    		else{
				if (node->left == nullptr){
	            	Node *temp = node->right;
            		delete node;
            		return temp;
        		}
        		else if (node->right == nullptr){
            		Node *temp = node->left;
            		delete node;
            		return temp;
        		}
        		Node *temp = minValueNode(node->right);
        		node->result = temp->result;
        		node->right = removeLoop(node->right, temp->result);
    		}
    		return node;
 		}
		void remove(){
			if(this->size() == 0) return;
			unsigned long long number = DFS(root);
			while(number != 0 && !queueTime.empty()){
				int temp = queueTime.front();
				queueTime.pop();
				root = removeLoop(root ,temp);
				number --;
			}
		}
		string print_recursive(Node* node){
			if(node == nullptr) return "NULL"; //! trường hợp dừng print

			string left = print_recursive(node->left);
			solution << node->result << " ";
			string right = print_recursive(node->right);

			if(node->left == nullptr && node->right == nullptr) return to_string(node->result); //! tr
			return to_string(node->result) + "("+left +","+right+")";
		}
		void print(){			
			if(this->size() == 0){
				solution << "EMPTY" ;
			}
			else {
				string s;
				solution<<"inoder: ";
				s = print_recursive(root);
				solution << "\nTree: "<<s;
			}			
		}

		void destruct(Node* node){
			if(node == nullptr) return;
			destruct(node->left);
			destruct(node->right);
			delete node;
		}
		~Tree_BST(){
			destruct(root);
		}
	private:
		class Node{
		private:
			int result;
			Node* left;
			Node* right;
			friend class Tree_BST;
		public:
			Node(int result) : result(result), left(NULL), right(NULL) {}
		};
	};
};

class restaurant_Sukuna{
	class Node;
private:
	vector<Node* > areaTable;
	list<Node* > LRU;
public:
	restaurant_Sukuna(){}
	void insertAreaTable(int result){
		int ID = result % MAXSIZE + 1;
		int index = -1;
		for(unsigned int i=0; i<areaTable.size();i++){//find index
			if(areaTable[i]->ID == ID){
				index = i;
				break;
			}
		}
		if(index == -1){
			areaTable.push_back(new Node(ID));
			index = areaTable.size() - 1;
			LRU.push_front(areaTable[index]);
			areaTable[index]->insert(result);
			this->ReHeap_up(index);
		}
		else{
			LRU.erase(find(LRU.begin(), LRU.end(), areaTable[index]));
			LRU.push_front(areaTable[index]);
			areaTable[index]->insert(result);
			this->ReHeap_down(index);
		}
	}
	void remove_KEITEIKEN(int number){
		if(areaTable.size() <= 0) return;
		int numberRemove = number;
		unsigned int count = 0;
		count += (areaTable.size() - count) > 0 ? number : areaTable.size();
		queue<Node*> q;
		vector<Node*> temp(areaTable.begin(), areaTable.end());
		while(count){
			q.push(areaTable[0]);
			swap(areaTable[0], areaTable[areaTable.size() - 1]);
			areaTable.pop_back();
			this->ReHeap_down(0);
			count--;
		}
		areaTable = temp;
		while(areaTable.size() != 0 && number-- > 0){
			Node* it = q.front();q.pop();
			solution << "remove customers in the area ID = " << it->ID << "(len="<< it->size() << "): ";
			it->remove(numberRemove);
			if(it->size() == 0){
				int i=0;
				while(areaTable[i]->ID != it->ID) i++;
				swap(areaTable[i], areaTable[areaTable.size() - 1]);
				LRU.erase(find(LRU.begin(), LRU.end(), areaTable[areaTable.size() - 1]));
				delete areaTable[areaTable.size() - 1];
				areaTable.pop_back();this->ReHeap_down(i);
			}
		}
 	}

	void print_pre_order(int index, int number)	{	
		if(index - areaTable.size() >= 0) return;

		this->areaTable[index]->print(number);
		print_pre_order(index * 2 + 1, number);
		print_pre_order(index * 2 + 2, number);
	}
	void print_CLEAVE(int number){
		if(number <= 0) return;
		/*
		solution << "Heap : ";
		for(auto it : this->areaTable){
			int order = 0;
			for (auto ix : LRU) {
				if (ix == it) break;
				++order;
			}
			solution << it->ID << "(len="<< it->size() <<",index=" << order <<")" << " ";
		}
		solution << "\n";
		*/
		solution << "Heap : ";
		for(auto it : areaTable) solution << it->ID  << " ";
		solution << "\n";

		solution << "list LRU : ";
		for(auto it : LRU) solution << it->ID << " ";
		solution << "\n";

		print_pre_order(0, number);
	}

	~restaurant_Sukuna(){
		for(auto x:areaTable) delete x;
		areaTable.clear();
		LRU.clear();
	}
private:

	bool checkMin(int index, int k){
		unsigned int indexSize = areaTable[index]->size(),
			kSize = areaTable[k]->size();
		if(indexSize < kSize) return 1;
		if(indexSize > kSize) return 0;
		for(auto x:LRU){
			if(x == areaTable[k]) return 1;
			else if(x == areaTable[index]) return 0;
		}
		return 0;
	}
	void ReHeap_down(int index){
		int size = areaTable.size();
		int k;//k = child, index = par;
    	if(index*2+1 > size-1) return;
    	else if(index*2+2 > size-1 || checkMin(index*2+1, index*2+2)) k = index*2+1;
    	else k = index*2+2;
		bool check = !checkMin(index,k);
    	if(check){// index > k
        	swap(areaTable[index], areaTable[k]);
        	ReHeap_down(k);
    	}
	}
	void ReHeap_up(int index){
		if(index == 0) return;
		int k = (index-1)/2; // father
		bool check = checkMin(index,k);
		if(check){ //index < k
			swap(areaTable[k], areaTable[index]);
			ReHeap_up(k);
		}
	}
private:
	class Node{
	private:
		int ID;
		list<int> head;
		friend class restaurant_Sukuna;
	public:
		Node(int ID) : ID(ID) {}
		int size() const { return head.size(); }
		void insert(int result){head.push_front(result);}
		void remove(int number)	{
			while(number-- && !head.empty()){
				solution<<head.back()<<" ";
				head.pop_back();				
			}
			solution<<endl;
		}
		void print(int number){
			solution << "customers in the area ID = " << ID << "(len="<< size() << "): ";
			for(list<int>::iterator it = head.begin(); number > 0 && it != head.end(); ++it, --number){
				solution << *it << " ";
			}
			solution << "\n";
		}

	};
};

class HuffTree_AVL{
private: 
	class Node{
	private:
		char data;
		int freq;
		Node* left;
		Node* right;
		friend class HuffTree_AVL;
	public:
		Node(char data, int freq, Node* left, Node* right) : data(data), freq(freq), left(left), right(right) {}
	};
	Node* recent;
public:
	void handloop(Node* node){
		if(node == nullptr) return;
		handloop(node->left);
		if(node->data == '\0') solution<<node->freq<<endl;
		else solution<<node->data<<endl;
		handloop(node->right);
	}
	void prHand(){
		handloop(recent);
	}
	void vtprint(vector<Node*> a){
		for(auto x:a){
			cout<<x->data<<" "<<x->freq<<" -> ";
		}
		cout<<endl;
	} 
	string print_recursive(Node *node){
			if (node == nullptr)
				return "NULL";		
			string left = print_recursive(node->left);		
			string right = print_recursive(node->right);

			if (node->left == nullptr && node->right == nullptr)
				return "[" + to_string(node->freq) + "," + node->data + "]";
			return "[" + to_string(node->freq) + "]" + "(" + left + "," + right + ")";
		}
	void printTree(Node* root){
		//solution << "inoder: ";
		string s = print_recursive(root);
		solution << "root : " << s;
		solution<<endl;
	}
	void print(vector<pair<char,int>> ans){
		solution << "freq     : {";
		solution << "{'" << ans[0].first << "'," << ans[0].second << "}";
    	for (unsigned int i = 1; i < ans.size(); i++){
        	solution << ",{'" << ans[i].first << "'," << ans[i].second << "}";
		}
		solution<<"}"<<endl;
	}
	
	bool condition(Node* l, Node* r){//return true if smaller
		if(l->freq < r->freq) return 1;
		if(l->freq == r->freq){
			if(r->data == '\0') return 1;
			if(isupper(l->data) && islower(r->data)) return 0;
			if(islower(l->data) && isupper(r->data)) return 1;
			if(l->data < r->data) return 1;
		}
		return 0;
	}
	vector<Node*> vtadd(vector<Node*> bin, Node* temp){
		bin.push_back(temp);
		unsigned int i = bin.size()-1;
		while(i>0 && condition(bin[i-1],temp)){
			bin[i] = bin[i-1];
			i--;
		}
		bin[i] = temp;
		return bin;
	}
	
	int height(Node* node){
		if (node == NULL) return 0;
        int lh = height(node->left);
        int rh = height(node->right);
        return max(lh,rh) + 1;
	}
	int weight(Node* node){
		if (node == NULL) return 0; 		
    	return height(node->right) - height(node->left);  
	}
	Node* rotateRight(Node* root) {
    	Node* p = root->left;
    	root->left = p->right;
    	p->right = root;
    	return p;
	}
	Node* rotateLeft(Node* root) {
    	Node* p = root->right;
    	root->right = p->left;
    	p->left = root;
		return p;
	}
	Node* balanceTree(Node* root, int count){
		if(count == 0 || root == nullptr) return root;
		int w = weight(root);
		if(abs(w) > 1){
			if(w < -1){
				if(weight(root->left) > 0 && count >= 2){
					root->left = rotateLeft(root->left);
					count--;
				}
				root = rotateRight(root);
				count--;
			}
			else{
				if(weight(root->right) < 0 && count >= 2){
					root->right = rotateRight(root->right);
					count--;
				}
				root = rotateLeft(root);
				count--;
			}
		}
		root->left = balanceTree(root->left,count);
		root->right = balanceTree(root->right,count);
		return root;
	}

	char encode(char data, int freq){
		freq = freq%26;
		int k = data + freq;
		if(isupper(data) && k > 'Z'){
			freq = k - 'Z' - 1;
			data = 'A';
		}
		else if(islower(data) && k > 'z'){
			freq = k - 'z' - 1;
			data = 'a';
		}
		data += freq;
		return data;
	}
	int connectBin(map<char,string> binary, string name){
		string partName,tenchar;
		unsigned int i = name.size()-1;
		while(i>=0 && partName.size() < 10){
			string s = binary[name[i]];
			partName += s;
			for(int j=s.size()-1;j>=0 && tenchar.size()<10;j--) tenchar += s[j];
			i--;
		}
		return stoi(tenchar, 0, 2);
	}
	void readNode(Node* node, string str, map<char,string> &binary){
		if (node == nullptr) return;
    	if (!node->left && !node->right){
			if(str.empty()) str = "0";
			binary[node->data] = str;
		}
        readNode(node->left, str + "0", binary);
    	readNode(node->right, str + "1", binary);
	}

	vector<pair<char,int>> string_Processing(string& name){
		vector<pair<char,int>> unSort;
		vector<pair<char,int>> newSort;
		vector<bool> done(name.size(),0);
		for(auto x: name){//convert name-> unsort vector
			unsigned int i=0;
			while(i<unSort.size() && x!=unSort[i].first) i++;
			if(i == unSort.size()) unSort.push_back({x,1});
			else unSort[i].second++;
		}
		for(auto x: unSort){//convert unsort vector-> newSort
			char charNew = encode(x.first,x.second);//encode ceasar
			
			for(unsigned int i=0;i<name.size();i++) if(name[i] == x.first && !done[i]){
				name[i] = charNew;
				done[i] = true;
			}
			//merge new vector
			unsigned int i=0;
			while(i<newSort.size() && charNew!=newSort[i].first) i++;//find
			if(i == newSort.size()) newSort.push_back({charNew, x.second});
			else newSort[i].second += x.second;
		}
		sort(newSort.begin(), newSort.end(), smaller);
		return newSort;
	}
	Node* buildHuffTree(vector<pair<char,int>> v){
		vector<Node*> bin;
		for(auto x:v) bin.push_back(new Node(x.first, x.second, nullptr, nullptr));
		while (bin.size() != 1) {
        	Node *left = bin.back(); bin.pop_back();
        	Node *right = bin.back(); bin.pop_back();
        	int sum = left->freq + right->freq;
			Node* newRoot = new Node('\0', sum, left, right);
			newRoot = balanceTree(newRoot,3);
        	bin = vtadd(bin, newRoot);
    	}
		Node* root = bin.back();
		return root;
	}
	int mainCaculate(string name){
		vector<pair<char,int>> newSort = this->string_Processing(name);
		if(newSort.size()<3) return -1;
		/////////
		Node* root = buildHuffTree(newSort);
		destruct(recent);
		recent = root;
		/////////
		map<char,string> binary;
		readNode(root, "", binary);
		int result = connectBin(binary, name);
		//////////
		this->print(newSort);
		printTree(root);
		solution<<"name   = "<<name<<endl;
		solution<<"result = "<<result<<endl;		
		return result;
	}

	void destruct(Node* node){
		if(node == nullptr) return;
		destruct(node->left);
		destruct(node->right);
		delete node;
	}
	~HuffTree_AVL(){
		destruct(recent);
	}
};

class restaurant_Op{
private:
	HuffTree_AVL cusnew;
	restaurant_Gojo hash;
	restaurant_Sukuna heap;
public:
	void LAPSE(string name)	{
		int result;
		if(!isdigit(name[0])) result = cusnew.mainCaculate(name);		
		else{
			result = stoi(name);//solution<<"result = "<<result<<endl;
		}
		if(result == -1) return;
		if(result % 2 == 1) hash.insertAreaTable(result);
		else heap.insertAreaTable(result);
	}	

	void KOKUSEN(){
		hash.remove_KOKUSEN();
	}
	void LIMITLESS(int num){
		hash.print_LIMITLESS(num);
	}

	void KEITEIKEN(int num){
		heap.remove_KEITEIKEN(num);
	}
	void CLEAVE(int num){
		heap.print_CLEAVE(num);
	}

	void HAND(){
		cusnew.prHand();
	}

};

void simulate_test_case(string filename){
	restaurant_Op* resOp;
	ifstream ss(filename);
	if (ss.fail()){
		cout << "ERROR: " << filename << endl;
		return;
	}
	string str, name;	
	int num;
	int i = 2;
	ss>>str;
	ss >> MAXSIZE;
	solution << "MAXSIZE : " << MAXSIZE << "\n";
	resOp = new restaurant_Op();
	solution.open("out.txt");
	while (ss >> str){
		if (str == "LAPSE"){ // LAPSE <NAME>		
			ss >> name;
			//solution << "LAPSE : LINE " << i << "\n";
			resOp->LAPSE(name);
		}
		else if (str == "KOKUSEN"){ // KOKUSEN
			//solution << "KOKUSEN : LINE " << i << "\n";
			resOp->KOKUSEN();
		}
		else if (str == "KEITEIKEN"){ // KEITEIKEN <NUM>		
			ss >> num;
			solution << "KEITEIKEN : LINE " << i << "\n";
			resOp->KEITEIKEN(num);
		}
		else if (str == "HAND"){ // HAND
			solution << "HAND : LINE " << i << "\n";		
			resOp->HAND();
		}		
		else if (str == "LIMITLESS"){ // LIMITLESS <NUM>		
			ss >> num;
			solution << "LIMITLESS " << num << " : LINE " << i << "\n";
			resOp->LIMITLESS(num);
			solution << '\n';
		}		
		else if (str == "CLEAVE"){ // CLEAVE <NUM>		
			ss >> num;
			solution << "CLEAVE : LINE " << i << "\n";
			resOp->CLEAVE(num);
		}				
		i++;
	}
	delete resOp;
}

void simulate(string filename){
	simulate_test_case(filename);
	return;

	ifstream ss(filename);
	string str, name;
	int num;
	ss >> str; ss >> MAXSIZE;
	restaurant_Op* resOp = new restaurant_Op();
	// //* xử lí file
	while (ss >> str){
		if (str == "LAPSE") // LAPSE <NAME>
		{
			ss >> name;
			resOp->LAPSE(name);
		}
		else if (str == "KOKUSEN") // KOKUSEN
		{
			resOp->KOKUSEN();
		}
		else if (str == "KEITEIKEN") // KEITEIKEN <NUM>
		{
			ss >> num;
			resOp->KEITEIKEN(num);
		}
		else if (str == "HAND") // HAND
		{
			resOp->HAND();
		}		
		else if (str == "LIMITLESS") // LIMITLESS <NUM>
		{
			ss >> num;
			resOp->LIMITLESS(num);
		}		
		else if (str == "CLEAVE") // CLEAVE <NUM>
		{
			ss >> num;
			resOp->CLEAVE(num);
		}				
	}
	delete resOp;
}