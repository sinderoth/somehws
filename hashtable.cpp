#include "hashtable.h"

AccessControl::AccessControl(int table1Size, int table2Size){
	size1 = table1Size;
	size2 = table2Size;
	table = new std::pair<std::string, std::string> [size1];
    table2.resize(size2);
	n=0;
	n2=0;
	for (int i=0 ; i<size1 ; i++){
		table[i].first = EMPTY;
		table[i].second = EMPTY;
	}
	for (int i=0 ; i<size2 ; i++){
		table2[i]=EMPTY;
	}
}

AccessControl::~AccessControl(){
    delete [] table;
    table2.clear();
}

bool AccessControl::isPrime( int n ) const
{
    if( n%2 == 0 ) return false;
    for( int i=3; i*i<=n; i+=2 )
        if( n%i == 0 )
            return false;
    return true;
}

int AccessControl::nextPrimeAfter( int n ) const
{
    for( int i=n+1; ; i++ )
        if( isPrime(i) )
            return i;
}

int AccessControl::addUser(std::string username, std::string pass){

    int i = 0;
    int hashed=0;
    for (int j = 0 ; j<size1 ;j++ ){
        hashed = hashFunction(username,size1,i);
        if (table[hashed].first == EMPTY || table[hashed].first == DELETED){
            table[hashed].first = username;
            table[hashed].second = pass;
            n++;
            if ((double)n/(double)size1 > MAX_LOAD_FACTOR ){
                expand_table();
            }
            return 1;
        }
        else if (table[hashed].first == username){
        	return 0;
        }
        else i++;
    }

    return 0;

}

int AccessControl::addUsers(std::string filePath){
	std::ifstream t(filePath.c_str());
    std::string word;
    std::string word2;
    int i =0;
    while( t >> word ){
    	t>>word2;
    	if(addUser(word,word2))
        i++;
    }
    t.close();
    return i;
}



int AccessControl::login(std::string username, std::string pass){
	int i = 0;
    int hashed=0;
    int secondHash=0;
    std::string currentPass = "";
    /*find password*/
    for (int j = 0 ; j<size1 ;j++ ){
        hashed = hashFunction(username,size1,i);
        if (table[hashed].first == EMPTY){
            break;
        }
        else if (table[hashed].first == username){
        	currentPass = table[hashed].second;
        }
       	i++;
    }
    i=0;
    if (pass == currentPass){
    	for (int j = 0 ; j<size2 ; j++){
    		secondHash = hashFunction(username,size2,i);
    		if (table2[secondHash] == EMPTY || table2[secondHash] == DELETED){
    			table2[secondHash] = username;
    			n2++;
    			if ((double)n2/(double)size2 > MAX_LOAD_FACTOR ){
        			expand2();
    			}
            	return 1;
    		}
    		else if (table2[secondHash]== username){
    			return 0;
    		}
    		else i++;
    	}
        return 0;

    }
    return 0;
}




int AccessControl::logout(std::string username){
	int secondHash= 0;
	int i=0;
	for (int j = 0 ; j < size2 ;j++){
		secondHash = hashFunction(username,size2,i);
		if (table2[secondHash] == username){
			table2[secondHash] = DELETED;
			n2--;
			return 1;
		}
		i++;
	}
	return 0;
}




void AccessControl::expand_table(){
	int oldsize = size1;
    size1 = nextPrimeAfter(2*size1);
	std::pair<std::string,std::string>* zactable; //temporary table
    zactable = table;
    table = new std::pair<std::string,std::string> [size1];
    std::vector<std::string> checkervector;
    n=0;
    int probcount=0;
    int i = 0;
    int hashed=0;
    int sechas=0;
    int alreadyHashed = 0;
    /*initialize*/
    for (int k=0 ; k < size1 ; k++){
        table[k].first = EMPTY;
        table[k].second = EMPTY;
    }

    for (int k=0 ; k<oldsize ; k++) {
        if (zactable[k].first != EMPTY && zactable[k].first != DELETED){
        	alreadyHashed = 0;
        	/*for every element mark them or check already marked or not */
        	if (checkervector.empty());
        	else{
        		for(int index = 0 ; index < checkervector.size() ; index++){
        			if (checkervector[index] == zactable[k].first){
        				alreadyHashed=1;
        			}
        		}
        	}

        	/*if not marked insert them to new table*/
            i=0;
            if (alreadyHashed == 0){

                checkervector.push_back(zactable[k].first);
	            for (int j = 0 ; j < oldsize ; j++){
	                hashed = hashFunction(zactable[k].first,oldsize,i);
	                if (zactable[hashed].first == zactable[k].first){
	                    for (probcount = 0 ; probcount<size1; probcount++){

	                	        sechas = hashFunction(zactable[hashed].first,size1,probcount);
	                            if (table[sechas].first == EMPTY){
	                                table[sechas].first = zactable[hashed].first;
	                                table[sechas].second = zactable[hashed].second;
	                                n++;
	                                break;
	                            }
	                    }
	                }
	                else if (zactable[hashed].first == EMPTY) break;
	                i++;
	            }
	        }
        }
    }

    checkervector.clear();
    delete [] zactable;
}

void AccessControl::expand2(){
	int oldsize = size2;
	size2 = nextPrimeAfter(2*size2);
	std::vector<std::string> zactable;
	zactable = table2;

	int i;
	int secondHash = 0;
	n2 = 0;
	table2.resize(size2);
	for (int j=0 ; j<size2; j++){
		table2[j] = EMPTY;
	}
	for (int j =0 ; j < oldsize ; j++){
		if (zactable[j] != EMPTY && zactable[j] != DELETED){
			for (i=0; i< size2 ; i++){
				secondHash = hashFunction(zactable[j],size2,i);
				if (table2[secondHash] == EMPTY){
					table2[secondHash] = zactable[j];
					n2++;
					break;
				}
				else if (table2[secondHash] == zactable[j])
					break;
			}
		}
	}
	zactable.clear();
}


int AccessControl::delUser(std::string username, std::vector<std::string>& oldPasswords){
	if(username == "") return 0;
	int hashed;
	bool registered= false;
	for (int i=0; i<size1;i++){
		hashed = hashFunction(username,size1,i);
		if (table[hashed].first == username){
			oldPasswords.push_back(table[hashed].second);
			table[hashed].first = DELETED;
			table[hashed].second = DELETED;
			registered = true;
			n--;
		}
		else if (table[hashed].first == EMPTY) break; //finding an empty slot, stop searching
	}
	if (registered) return 1;
	return 0;
}


int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass){
	std::string currentPass = "";
	int hashed = 0;
	int i = 0;
	int probcount = 0;
	if (oldpass == newpass) return 0;
	if (username == "") return 0;

	/*findpassword*/
	for (int j = 0 ; j < size1 ; j++){
		hashed = hashFunction(username,size1,i);
		if (table[hashed].first == username){
			currentPass = table[hashed].second;
			probcount = i;
		}
		else if (table[hashed].first == EMPTY) break;
		i++;
	}

	if (oldpass == currentPass){
		probcount;
		for (int j = 0 ; j < size1 ; j++){
			hashed = hashFunction(username,size1,probcount);
			if (table[hashed].first == EMPTY || table[hashed].first == DELETED){
				table[hashed].first = username;
				table[hashed].second = newpass;
				n++;
				if ((double)n/(double)size1 > MAX_LOAD_FACTOR ){
        			expand_table();
    			}
				return 1;
			}
			probcount++;
		}

	}
	return 0;
}


float AccessControl::printPasswords(){
	for (int i=0 ; i<size1 ; i++){
		std::cout << table[i].first<< " " << table[i].second << std::endl;
	}

	return (double)(n)/(double)size1;
}


float AccessControl::printActiveUsers(){
	for (int i=0; i<size2 ; i++){
		std::cout << table2[i] << std::endl;
	}
	return (double)n2/(double)size2;
}
