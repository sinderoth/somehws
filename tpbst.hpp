#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.

template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);	

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);
    void printHelper(PrimaryNode* &myroot,std::string a);
    void printHelperSec(SecondaryNode* &myrootsec,std::string a);
    SecondaryNode* findparent(SecondaryNode* &rootsec,const std::string &secondaryKey);

private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {
    /* IMPLEMENT THIS */
    PrimaryNode* ptr = root;
    SecondaryNode* sptr;
    SecondaryNode* newsec;
    PrimaryNode* newnode;
    if (ptr == NULL){
    	newsec = new SecondaryNode(secondaryKey,data,NULL,NULL);
    	newnode = new PrimaryNode(primaryKey,NULL,NULL,newsec);
    	root = newnode;
  
    }
    else {
    	while (ptr != NULL){
    		if (ptr->key == primaryKey){
    			newsec = new SecondaryNode(secondaryKey,data,NULL,NULL);
    			sptr = ptr->rootSecondaryNode;
    			if (sptr == NULL){
    				ptr->rootSecondaryNode = newsec;
    				return *this;
    			}
    			while (sptr){
    				if (secondaryKey > sptr->key){
    					if (sptr->right == NULL){
    						sptr->right = newsec;
    						return *this;
    					}
    					sptr = sptr->right;
    				}
    				if (secondaryKey < sptr->key){
    					if (sptr->left == NULL){
    						sptr->left = newsec;
    						return *this;
    					}
    					sptr = sptr->left;
    				}
    			}
    		}
    		if (ptr->key > primaryKey){
    			if (ptr->left !=NULL){
    				ptr = ptr->left;
    			}
    			else{
    				newnode = new PrimaryNode(primaryKey,NULL,NULL,NULL);
    				ptr->left = newnode;
    			}
    		}
    		if (ptr->key < primaryKey){
    			if (ptr->right !=NULL){
    				ptr = ptr->right;
    			}
    			else {
    				newnode = new PrimaryNode(primaryKey,NULL,NULL,NULL);
    				ptr->right = newnode;

    			}
    		}
    	}
    	return *this;
    }
    return *this;


}


template <class T>
typename TwoPhaseBST<T>::SecondaryNode* TwoPhaseBST<T>::findparent(SecondaryNode* &rootsec,const std::string &secondaryKey){
	if (rootsec == NULL) return NULL;
	if (rootsec->left == NULL && rootsec->right == NULL) return NULL;
	if( (rootsec->left != NULL && rootsec->left->key == secondaryKey)
        || (rootsec->right != NULL && rootsec->right->key == secondaryKey))
       return rootsec;
	if (rootsec->key > secondaryKey){
		return findparent(rootsec->left,secondaryKey);
	}
	if (rootsec->key < secondaryKey){
		return findparent(rootsec->right,secondaryKey);
	}
	return NULL;

}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode* ptr = root;
    SecondaryNode* sec;
    SecondaryNode* parent;
    SecondaryNode* temp;
    if (ptr == NULL) return *this;
    while (ptr){
    	if (ptr->key == primaryKey){
    		sec = ptr->rootSecondaryNode;
    		if (sec == NULL) return *this;
    		if (sec->right == NULL && sec->left == NULL){
    			ptr->rootSecondaryNode = NULL;
    			delete sec;
    			return *this;    			
    		}
    		while(sec){
    			if (sec->key == secondaryKey){
    				/*leaf node case*/
    				if (sec->left == NULL && sec->right == NULL){
    					parent = findparent(ptr->rootSecondaryNode,secondaryKey);
    					if (parent->left == sec){
    						parent->left = NULL;
    					}
    					else if (parent->right == sec){
    						parent->right = NULL;
    					}
    					delete sec;
    					return *this;
    				}
    				/*one child*/
    				else if (sec->left==NULL || sec->right==NULL){    			    					
    					if(sec->left){
    						parent=findparent(ptr->rootSecondaryNode,sec->key);
    						if (parent == NULL){
    							temp = sec->left;
    							delete sec;
    							ptr->rootSecondaryNode = temp;
    							return *this;
    						}
    						if(parent->right==sec){
    							parent->right = sec->left;
    						}
    						else parent->left = sec->left;
    						delete sec;
    						return *this;
    					}
    					else if (sec->right){
    						parent =findparent(ptr->rootSecondaryNode,sec->key);
    						if (parent == NULL){
    							temp = sec->right;
    							delete sec;
    							ptr->rootSecondaryNode = temp;
    							return *this;
    						}
    						if(parent->right==sec){
    							parent->right = sec->right;
    						}
    						else parent->left = sec->right;
    						delete sec;
    						return *this;
    					}
    				}
    				else if (sec->left !=NULL && sec->right != NULL){

    					if(sec==ptr->rootSecondaryNode){

    						temp = sec;
    						sec = sec->left;
    						if (sec->right){
    							while (sec->right){
    								sec = sec->right;
    							}
    							parent = findparent(ptr->rootSecondaryNode,sec->key);
    							/*check hereontheleft*/
    							parent->right = sec->left;
    							sec->left = temp->left;
    							sec->right = temp->right;
    							delete temp;
    							ptr->rootSecondaryNode= sec;
    							return *this;
    						}
    						else{
    							sec->right = temp->right;
    							delete temp;
    							ptr->rootSecondaryNode = sec;
    							return *this;
    						}
    					}
    					else {
    						temp = sec;
    						sec = sec->left;
    						if (sec->right==NULL){
    							sec->right = temp->right;
    							parent = findparent(ptr->rootSecondaryNode,temp->key);
    							if (parent->right == temp){
    								parent->right = sec;
    							}
    							else parent->left = sec;
    							delete temp;
    							return *this;
    						}
    						while (sec->right){
    							sec = sec->right;
    						}
    						parent = findparent(ptr->rootSecondaryNode,sec->key);
    						parent->right = sec->left;
    						parent = findparent(ptr->rootSecondaryNode,temp->key);
    						sec->left = temp->left;
    						sec->right = temp->right;
    						if(parent->right == temp){
    							parent->right = sec;
    						}
    						else parent ->left =sec;
    						delete temp;
    						return *this;


    					}
    				}
    				
    			}
    			else if (sec->key < secondaryKey){
    				sec = sec->right;
    			}
    			else if (sec->key > secondaryKey){
    				sec = sec->left;
    			}
    		}
    		return *this;
    	}
    	if (ptr->key < primaryKey){
    		ptr = ptr->right;
    	}
    	if (ptr->key > primaryKey){
    		ptr = ptr->left;
    	}
    }
    if (ptr == NULL) return *this;
    return *this;
}

template <class T>
void TwoPhaseBST<T>::printHelperSec(SecondaryNode* &myrootsec,std::string a){
	if(myrootsec==NULL) {
            
        return;
    }
	printHelperSec(myrootsec->left,a);
	if (myrootsec->key != a){
		std::cout  << "\""<< myrootsec->key<< "\"" << " : " << "\"" << myrootsec->data << "\"" <<", " ;}
	else {
		std::cout << "\""<< myrootsec->key<< "\"" << " : " << "\"" << myrootsec->data << "\"";
	}
	printHelperSec(myrootsec->right,a);

}

template <class T>
void TwoPhaseBST<T>::printHelper(PrimaryNode* &myrootsec,std::string rootk){
	SecondaryNode* jordan;
	if(myrootsec==NULL) {   
        return;
    }
	printHelper(myrootsec->left,rootk);
	if (myrootsec->key ==rootk){
		std::cout <<"\""<< myrootsec->key<< "\"" << " : " ;
	}
	else
	std::cout << ", \""<< myrootsec->key<< "\"" << " : " ;
	jordan = myrootsec->rootSecondaryNode;
	if (jordan == NULL) {
		std::cout<<"{}" ;
		return;

	}
	while (jordan->right){
		jordan = jordan->right;
	}
    std::cout<<"{";
	printHelperSec(myrootsec->rootSecondaryNode,jordan->key);
    std::cout<<"}";
	printHelper(myrootsec->right,rootk);
}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    //std::cout <<444444<<std::endl;
    PrimaryNode* ptr = root;
    PrimaryNode* temp = root;
    SecondaryNode* ptr2;
    if(ptr == NULL){
    	std::cout << "{}"<< std::endl;
        //std::cout <<111111<<std::endl;
    	return *this;
    }
    if (primaryKey=="" && secondaryKey==""){
    	/*full*/
        //std::cout <<222<<std::endl;
        while (temp->left){
        	temp=temp->left;
        }
        std::cout<<"{";
    	printHelper(ptr,temp->key);
        std::cout<<"}";
        std::cout<<std::endl;
    	return *this;

    }
    else if (primaryKey !="" && secondaryKey==""){
    	//std::cout <<33333<<std::endl;
        while (ptr){
    		if (ptr->key == primaryKey)
    			break;
    		else if (ptr->key < primaryKey){
    			ptr = ptr-> right;
    		}
    		else if (ptr->key > primaryKey){
    			ptr = ptr->left;
    		}
    	}
        if(ptr==NULL){
            std::cout<<"{}"<<std::endl;
            return *this;
        }
        
    	ptr2 = ptr->rootSecondaryNode;
        if(ptr2==NULL){
            std::cout<<"{\""<<primaryKey<<"\" : "<<"{}}"<<std::endl;
            return *this;
        }
    	while(ptr2->right){
    		ptr2= ptr2->right;
    	}

    	std::cout <<"{\"" <<primaryKey << "\" : {" ;
    	printHelperSec(ptr->rootSecondaryNode,ptr2->key);
    	std::cout << "}}" << std::endl;
    }
    else{
    	
    	while (ptr){
    		if (ptr->key == primaryKey)
    			break;
    		else if (ptr->key < primaryKey){
    			ptr = ptr-> right;
    		}
    		else if (ptr->key > primaryKey){
    			ptr = ptr->left;
    		}
    	}
        if(ptr==NULL){
            std::cout<<"{}"<<std::endl;
            return *this;
        }
    	ptr2 = ptr->rootSecondaryNode;

        if(ptr2==NULL){
        	/*std::cout<<"joker1" <<std::endl;
            std::cout<<"{\""<<primaryKey<<"\" : {}}"<<std::endl;*/
            std::cout << "{}" << std::endl;
            return *this;
        }

    	while (ptr2){

    		if (ptr2->key == secondaryKey)
    			break;
    		else if (ptr2->key < secondaryKey){
    			ptr2 = ptr2-> right;
    		}
    		else if (ptr2->key > secondaryKey){
    			ptr2 = ptr2->left;
    		}
    	}
    	if (ptr2 == NULL){
    		std::cout << "{}"<< std::endl;
    		return *this;
    	}
    	//std::cout<<"joker2" <<std::endl;
    	std::cout <<"{\""<<primaryKey<<"\" : {"<<"\"" <<ptr2->key <<"\"" <<" : "<<"\""<<ptr2->data<<"\"}}"<<std::endl;
   
    	}
    
    return *this;
}

template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
    /* IMPLEMENT THIS */
    PrimaryNode* ptr = root;
    SecondaryNode* sec;
    if (ptr == NULL) return NULL;
    while (ptr){
    	if (ptr->key == primaryKey){
    		sec = ptr->rootSecondaryNode;
    		if (sec == NULL) return NULL;
    		while (sec){
    			if (sec->key == secondaryKey){
    				return &(sec->data);
    			}
    			else if (sec->key < secondaryKey){
    				sec = sec->right;
    			}
    			else if (sec->key > secondaryKey){
    				sec = sec->left;
    			}
    		}
    		return NULL;
    	}
    	else if (ptr->key < primaryKey){
    		ptr = ptr->right;
    	}
    	else if (ptr->key > primaryKey){
    		ptr = ptr->left;
    	}
    }
    return NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

#endif //TWOPHASEBST_H
