#include<iostream>
#include<stdio.h>
#include<fstream>
#include<climits>
#include<cstring>
#include<string>
using namespace std;

class RBTree;
class Node{
	public:
		friend class RBTree;
		Node *parent;
		Node *left;
		Node *right;
		int key;
		bool color;
		//black:true; red:false

		Node():parent(0),left(0),right(0),color(false){};
		Node(int key):parent(0),left(0),right(0),key(key),color(false){};

};
class RBTree{

	public:
		Node *root;
		Node *neel;
		//initialize
		RBTree(){
			neel = new Node;
			neel->color = true;
			root = neel;
			root->parent = neel;
		};
		Node* search(int KEY){
			Node *current = root;
			while(current != neel && KEY != current->key){
				if(KEY < current->key){
					current = current->left;
				}else{
					current = current->right;
				}
			}
			return current;
		};
		Node* successor(Node* current){
			if(current->right != neel){
				current = current->right;
				while(current->left != neel){
					current = current->left;
				}
				return current;
			}

			Node *next = current->parent;
			while(next != neel && current != next->left){
				current = next;
				next = next->parent;
			}
			return next;
		};
		void leftRotation(Node *x){
			Node *y = x->right;
			x->right = y->left;
			if(y->left != neel){
				y->left->parent = x;
			}
			y->parent = x->parent;
			if(x->parent == neel){
				root = y;
			}
			else if(x == x->parent->left){
				x->parent->left = y;
			}
			else if(x == x->parent->right){
				x->parent->right = y;
			}
			y->left = x;
			x->parent = y;
		};
		void rightRotation(Node *x){
			Node *y = x->left;
			x->left = y->right;
			if(y->right != neel){
				y->right->parent = x;
			}
			y->parent = x->parent;
			if(x->parent == neel){
				root = y;
			}
			else if(x == x->parent->left){
				x->parent->left = y;
			}
			else if(x == x->parent->right){
				x->parent->right = y;
			}
			y->right = x;
			x->parent = y;
		};
		void insertRBTree(int key){
			Node *x = root;
			Node *y = neel;
			Node *insertNode = new Node(key);
			while(x != neel){
				y = x;
				if(insertNode->key < x->key){
					x = x->left;
				}else{
					x = x->right;
				}
			}
			insertNode->parent = y;
			if(y == neel){
				this->root = insertNode;
			}
			else if(insertNode->key < y->key){
				y->left = insertNode;
			}else{
				y->right = insertNode;
			}

			insertNode->left = neel;
			insertNode->right = neel;
			insert_fixedUp(insertNode);
		};
		void insert_fixedUp(Node *current){
			//case 0: when parent is black, that is OK
			while(current->parent->color == false){
				Node *uncle = new Node;
				if(current->parent == current->parent->parent->left){
					uncle = current->parent->parent->right;
				}else{
					uncle = current->parent->parent->left;
				}

				//case 1: when uncle is red, change color
				if(uncle->color == false){
					current->parent->color = true;
					current->parent->parent->color = false;
					uncle->color = true;
					current = current->parent->parent;
				}
				//case 2&3: uncle is black
				else{
					if(uncle == current->parent->parent->right){
						//case 2
						if(current == current->parent->right){
							current = current->parent;
							leftRotation(current);
						}
						//case 3
						current->parent->color = true;
						current->parent->parent->color = false;
						rightRotation(current->parent->parent);
					}else{
						//case 2
						if(current == current->parent->left){
							current = current->parent;
							rightRotation(current);
						}
						//case 3
						current->parent->color = true;
						current->parent->parent->color = false;
						leftRotation(current->parent->parent);
					}
				}
			}
			root->color = true;
		};
		void deleteRBTree(int KEY){
			Node *deleteNode = search(KEY);
			if(deleteNode == neel){
				return;
			}
			Node *y = 0;   //delete_node
			Node *x = 0;   //delete_node's child
			if(deleteNode->left == neel){
				y = deleteNode;
			}
			else if(deleteNode->right == neel){
				y = deleteNode;
			}else{
				y = successor(deleteNode);
			}

			if(y->left != neel){
				x = y->left;
			}else{
				x = y->right;
			}
			x->parent = y->parent;

			if(y->parent == neel){
				this->root = x;
			}
			else if(y == y->parent->left){
				y->parent->left = x;
			}else{
				y->parent->right = x;
			}

			//case 3:delete_node has two children
			if(y != deleteNode){
				deleteNode->key = y->key;
				//the node's color no change, since we just copy data
			}

			if(y->color == true){
				delete_fixedUp(x);
			}
		};
		void delete_fixedUp(Node *current){
			//case 0: if current is red, change its color to black
			while(current != root && current->color == true){
				//current is on the left
				if(current == current->parent->left){
					Node *sibling = current->parent->right;

					//case 1:when sibling is red
					if(sibling->color == false){
						sibling->color = true;
						sibling->parent->color = false;
						leftRotation(sibling->parent);
						sibling = current->parent->right;
					}
					//finishing case 1, it will enter case 2&3&4
					//case 2:when sibling is black and two children is black
					if(sibling->left->color == true && sibling->right->color == true){
						sibling->color = false;
						current = current->parent;
					}
					//finishing case 2, it will decide which case again(including case 1,2,3,4)
					else{
						//case 3:when sibling is black and leftchild is red
						if(sibling->right->color == true){
							sibling->left->color = true;
							sibling->color = false;
							rightRotation(sibling);
							sibling = current->parent->right;
						}
						//finishing case 3, it will change to case 4
						//case 4:when sibling is black and rightchild is red
						sibling->color = sibling->parent->color;
						sibling->parent->color = true;
						sibling->right->color = true;
						leftRotation(sibling->parent);
						current = root;
					}
				}
				//current is on the right
				else{
					Node *sibling = current->parent->left;

					//case 1:when sibling is red
					if(sibling->color == false){
						sibling->color = true;
						sibling->parent->color = false;
						rightRotation(sibling->parent);
						sibling = current->parent->left;
					}
					//finishing case 1, it will enter case 2&3&4
					//case 2:when sibling is black and two children is black
					if(sibling->left->color == true && sibling->right->color == true){
						sibling->color = false;
						current = current->parent;
					}
					//finishing case 2, it will decide which case again(including case 1,2,3,4)
					else{
						//case 3:when sibling is black and rightchild is red
						if(sibling->left->color == true){
							sibling->right->color = true;
							sibling->color = false;
							leftRotation(sibling);
							sibling = current->parent->left;
						}
						//finishing case 3, it will change to case 4
						//case 4:when sibling is black and rightchild is red
						sibling->color = sibling->parent->color;
						sibling->parent->color = true;
						sibling->left->color = true;
						rightRotation(sibling->parent);
						current = root;
					}
				}
			}
			current->color = true;
		};



};




int main(void){

	//initialiaze buffer of input data
	int tree_insert[105];
	memset(tree_insert,0,sizeof(tree_insert));
	int tree_delete[105];
	memset(tree_delete,0,sizeof(tree_delete));

	fstream infile;
	int times=0;
	int f=0;


	//write
	/*fstream file;
	file.open("output.txt", ios::out);
	if(file.fail()){
		cout<<"The file can not open!"<<endl;
	}*/ 

	//read
	infile.open("input.txt", ios::in);
	if(!infile){
		cout<<"The file can not open!"<<endl;
	}else{
		while(!infile.eof()){
			infile>>times;
			if(infile.fail()){
				break;
			}

			RBTree *RBT = new RBTree;

			for(int p=0;p<times;p++){
				infile>>f;                              //the number of function
				if(infile.fail()){
					break;
				}


				if(f==1){
					char c1;
					int k1=0;
					int t1=0;
					memset(tree_insert,0,sizeof(tree_insert));

					while(1){
						infile>>t1;
						if(infile.fail()){
							break;
						}
						infile.get(c1);                //read the space or endl
						if(!infile.fail()){
							if(c1=='\n'||c1=='\r'){
							    if(c1=='\r'){
                                    infile.get(c1);
                                }
								tree_insert[k1]=t1;
								k1++;
								t1=0;
								break;
							}else if(c1==' '){
								tree_insert[k1]=t1;
								k1++;
								t1=0;
							}
						}else{
							tree_insert[k1]=t1;
							k1++;
							break;
						}
					}

					cout<<"Insert: ";
					for(int i=0;i<k1-1;i++){
						cout<<tree_insert[i]<<", ";
						RBT->insertRBTree(tree_insert[i]);
					}
					cout<<tree_insert[k1-1]<<endl;
					RBT->insertRBTree(tree_insert[k1-1]);

					//print_inoder
					Node *current = new Node;
					current = RBT->root;
					while(current != RBT->neel && current->left != RBT->neel){
							current = current->left;
					}
					while(current != RBT->neel){
						cout<<"key: "<<current->key<<" parent: ";
						if(current->parent == RBT->neel){
							cout<<" "<<" color: ";
						}else{
							cout<<current->parent->key<<" color: ";
						}
						if(current->color==false){
							cout<<"red"<<endl;
						}else if(current->color==true){
							cout<<"black"<<endl;
						}
						current = RBT->successor(current);
					}
				}
				else if(f==2){
					char c2;
					int k2=0;
					int t2=0;
					memset(tree_delete,0,sizeof(tree_delete));

					while(1){
						infile>>t2;
						if(infile.fail()){
							break;
						}
						infile.get(c2);                //read the space or endl
						if(!infile.fail()){
							if(c2=='\n'||c2=='\r'){
                                if(c2=='\r'){
                                    infile.get(c2);
                                }
								tree_delete[k2]=t2;
								k2++;
								t2=0;
								break;
							}else if(c2==' '){
								tree_delete[k2]=t2;
								k2++;
								t2=0;
							}
						}else{
							tree_delete[k2]=t2;
							k2++;
							t2=0;
							break;
						}
					}

					cout<<"Delete: ";
					for(int i=0;i<k2-1;i++){
						cout<<tree_delete[i]<<", ";
						RBT->deleteRBTree(tree_delete[i]);
					}
					cout<<tree_delete[k2-1]<<endl;
					RBT->deleteRBTree(tree_delete[k2-1]);

					//print_inoder
					Node *current = new Node;
					current = RBT->root;
					while(current != RBT->neel && current->left != RBT->neel){
							current = current->left;
					}
					while(current != RBT->neel){
						cout<<"key: "<<current->key<<" parent: ";
						if(current->parent == RBT->neel){
							cout<<" "<<" color: ";
						}else{
							cout<<current->parent->key<<" color: ";
						}
						if(current->color==false){
							cout<<"red"<<endl;
						}else if(current->color==true){
							cout<<"black"<<endl;
						}
						current = RBT->successor(current);
					}
				}
			}

		}
		infile.close();
		//file.close();
	}

	return 0;

}



