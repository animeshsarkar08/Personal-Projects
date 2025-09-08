#include "header.h"

/*unsigned long hash_it(int n){
	if(n > 0) n = -n;
	return (unsigned long)n;
}*/

unsigned long squash(unsigned long prev_hash,unsigned long merkle_hash,unsigned long timestamp){
//unsigned long squash(unsigned long prev_hash,unsigned long merkle_hash){
	
		return (prev_hash + merkle_hash + timestamp) % ULONG_MAX;
}
unsigned long hash_it(unsigned long actual_data,unsigned long difficulty_target,int* n, Hash* h){

	//parameters
	unsigned long max_try = 99999999999999999;
    unsigned long difficulty = difficulty_target;
	
	// if Hashing without difficulty target
	if(difficulty == 0) difficulty = 0x00ffffffffffffff;

    unsigned long nonce = 0;

    unsigned int prime = 31;

	unsigned long data = actual_data;
	
	// actual data bit mixed to encrypt actual data 
	data = data ^ nonce;
    data  = data  * prime;
	data  = data  ^ (data  >> 32);
	data  = data  ^ (data  >> 16);
	unsigned long ecrypted_data  = data;

	while(max_try-- > 0) {

	    ecrypted_data  = data;

	    data = data ^ nonce;
        data  = data  * prime;
        data  = data  ^ (data  >> 32);
        data  = data  ^ (data  >> 16);

		// meeting the exit condition of the loop
        if(data < difficulty ){
			if(n != NULL) *n = nonce;
			break;
        }
		//updating new nonce
        nonce++;
    }


	// This is the usable hash i.e it can be stored in the 64-bit architecture and can be used as a operand
	
	// Hashing without Hash-128 
	// returning the usable hash i.e. hash-64
	if(h == NULL) return data;

	
	h->hash_64 = data;
	
	//Generating the 128-bit hash 
	unsigned long prime_2 = 13;
	data  = data * prime_2;
	if(data > 0xffffffffffffffff){

		// This make sure that the hash is never greater than 0xffffffffffffffff 
		data = data % 0x0fffffffffffffff;

		// and this make sure that the hash is big enough
		data = data + 0x0fffffffffffffff;

		//apply modolus with least bound and then add least bound

		//For example my arch can store upto value 200
		//so I need a hash less than 200 but also big enough
		//I need a hash b/w 100 to 200
		//let say I got 499 so I applys modulus
		//699 % 100 = 99, then add 99 + 100 
		//this make sure the hash is b/w 100 and 200
	}

	data = data ^ (~data);
	data = data ^ (h->hash_64 << 32); 
	data = data ^ (data >> 32); 

	//integrating the two hashes
	
	//snprintf automatically adds the null char
	snprintf(h->hash_128,33,"%016lx%016lx",h->hash_64,data);

	// returning the encrypted data 
	return ecrypted_data;

}

Mk_node* merkle_tree(Mk_node* m){

	// This is this is the merkle root
	if(m->num == 1) return m;

	// Create a new merkle node
	Mk_node* temp = malloc(sizeof(*temp));

	temp->num = m->num/2;
	temp->hash_arr = malloc(sizeof(*temp->hash_arr) * temp->num);
	
	// hashing siblings
	int j;
	for(int i = 0; i < temp->num; i++){
		j = i*2;
		if(j + 1 < temp->num)
			temp->hash_arr[i] = hash_it(m->hash_arr[j],0,NULL,NULL) ^ hash_it(m->hash_arr[j + 1],0,NULL,NULL);
		else 
			temp->hash_arr[i] = hash_it(m->hash_arr[j],0,NULL,NULL);

	}

	// Assigning the prev node 
	temp->prev = m;
	
	// calling recursively
	return merkle_tree(temp);
}

void free_merkle(Mk_node* root){

	if(root == NULL) return;
	free(root->hash_arr);
	root->hash_arr = NULL;
	free_merkle(root->prev);
	free(root);
	root = NULL;
}
Block* create_block(Block** start,Block** end, unsigned long difficulty){
	
	srand(time(NULL));

	// This is the mempool with capactiy equal to (TX_NUM - 1)  + 1 for coinbase transaction
	Tx mpl[TX_NUM];

	// Filling the mempool
	for(int i=0; i < TX_NUM - 1;i++){
		// Tx_id of a user will be less than 100000 i.e 1 lakh
		mpl[i].id = rand() % 100000000000;

		// Transaction fee = 200;
		mpl[i].amt = (rand() % 1000000) - 200.00; 

		//incase the Transfer amount is negative it's not standard but I have to add this to maintain the integrity
		if(mpl[i].amt < 0) mpl[i].amt + 1000;
	}

	// Miners Tx_id will between 100000000000 to 20000000000
	// rand() % 100000000000 will always < 100000000000 but if we add 100000 to it the value will be greater than 100000000000  and the total value remains 100000000000
	mpl[TX_NUM - 1].id = (rand() % 100000000000) + 100000000000;
	mpl[TX_NUM - 1].amt = 600.00;

	// merkle leaves
	Mk_node* leaves = malloc(sizeof(*leaves));
	leaves->num = TX_NUM;
	leaves->hash_arr = malloc(sizeof(*leaves->hash_arr) * leaves->num);
	for(int i = 0; i < leaves->num; i++){
		leaves->hash_arr[i] = hash_it(mpl[i].id,0,NULL, NULL);
	}
	leaves->prev = NULL;
	
	// merkle tree
	Mk_node* mk_root = merkle_tree(leaves);
	Hash merkle_hash;
	hash_it(mk_root->hash_arr[0],0,NULL,&merkle_hash);
	free_merkle(mk_root);
	
	

	Block* b = malloc(sizeof(*b));
	if(b == NULL) return NULL;


		// Transactions
		for(int i=0; i < TX_NUM;i++)
			b->tx[i] = mpl[i];
		// assigining new block values
		b->merkle_hash = merkle_hash;

		// setting the time in string and int format
		time_t now = time(NULL);
		struct tm* t = localtime(&now);

		//for operand
		b->timestamp.time_int = now;

		//for printing
		strftime(b->timestamp.time_string, sizeof(b->timestamp.time_string), "%Y-%m-%d %H:%M:%S", t);
		b->difficulty = difficulty;

		int nonce = -1;



	// creating the first block
	if(*start == NULL){
		b->block_num = 0;
		strncpy(b->prev_hash.hash_128,"0x00000000000000000000000000000000",34);
		b->prev_hash.hash_64 = 0;

		//unsigned long a = squash(b->prev_hash.hash_64, b->merkle_hash.hash_64, b->timestamp);
		unsigned long a = squash(b->prev_hash.hash_64, b->merkle_hash.hash_64,b->timestamp.time_int);

		hash_it(a,difficulty,&nonce,&b->block_hash);
		b->nonce = nonce;
		b->next = NULL;
		*start = b;
		*end = b;
		return *start;
		
	}
	
		Block* last = *end;
		b->block_num = last->block_num + 1;
		strncpy(b->prev_hash.hash_128,last->block_hash.hash_128,strlen(last->block_hash.hash_128));

		b->prev_hash.hash_64 = last->block_hash.hash_64;

	
		//unsigned long a = squash(b->prev_hash.hash_64, b->merkle_hash.hash_64, b->timestamp);
		unsigned long a = squash(b->prev_hash.hash_64, b->merkle_hash.hash_64,b->timestamp.time_int);

		hash_it(a,difficulty,&nonce,&b->block_hash);
		b->nonce = nonce;
		b->next = NULL;
		last->next = b;
		*end = b;
		return *end;
}
void print_separator() {
    printf("+-----------------------+------------------------------------------------------------------+\n");
}
void print_blocks(Block *start){
	unsigned long n = 0;
	while(start != NULL){
    printf("+------------------------------------------------------------------------------------------+\n");
        printf("| %-22s| #00%-62u|\n","Block Number",start->block_num);
        print_separator();
        
        // Previous Hash
        printf("| %-22s| %-65s|\n", "Previous Block Hash", start->prev_hash.hash_128);
        print_separator();

        // Merkle Root Hash
        printf("| %-22s| %-65s|\n", "Merkle Root Hash",start->merkle_hash.hash_128);
        print_separator();

        // Timestamp
        printf("| %-22s| %-65s|\n","Timestamp",start->timestamp.time_string);
        print_separator();

        // Transactions
		char temp_str[65];
		for(int i=0; i < TX_NUM; i++){
			snprintf(temp_str,sizeof(temp_str),"Transaction Id: %u, Transferred amount: %.2f BTC",start->tx[i].id,start->tx[i].amt);
			
			if(i == 0 ) printf("| %-22s| %-65s|\n","Transactions",temp_str);
			else if(i == TX_NUM - 1)
				printf("| %-22s| %-65s|\n","Coinbase Transaction",temp_str);
			else 
				printf("| %-22s| %-65s|\n"," ",temp_str);
		}
        print_separator();

        // Nonce
        printf("| %-22s| %-65d|\n", "Nonce",start->nonce);
        print_separator();


        // Difficulty
		n = start->difficulty;
		snprintf(temp_str,sizeof(temp_str),"%016lXFFFFFFFFFFFFFFFF",start->difficulty);
        printf("| %-22s| %-65s|\n","Difficulty",temp_str);
        print_separator();
        
        // Block Hash
        printf("| %-22s| %-65s|\n","Block Hash",start->block_hash.hash_128);
    printf("+------------------------------------------------------------------------------------------+\n");	if(start->next != NULL){
		printf("%44s\n","||");
		printf("%44s\n","||");
		printf("%44s\n","\\/");
	}
		
		start = start->next;
	}

}

void free_blocks(Block* start){
	Block* tmp;
	while(start != NULL){
		tmp = start;
		start = start->next;
		free(tmp);
	}
}


// Test Codes 

/*
void print_blocks(Block* start){
	int n = 0;
	while(start != NULL){
		n = printf("Block Number #01%d\n",start->block_num);

		n = printf("Previous Block Hash: %s\n",start->prev_hash.hash_128);

		n = printf("Merkle Root Hash: %s\n",start->merkle_hash.hash_128);
		
		n = printf("Timestamp: %s\n",start->timestamp.time_string);

		printf("Transactions:\n");
		for(int i = 0; i < TX_NUM; i++){
			if(i == TX_NUM - 1) printf("Conibase Transaction by the miner\n");
			n = printf("Transaction Id: %d, Transferred amount: %.2f BTC\n\n",start->tx[i].id,start->tx[i].amt);
		}

		n = printf("Nonce: %d\n",start->nonce);

		n = printf("Difficulty: %016lxffffffffffffffff\n",start->difficulty);

		n = printf("Block Hash: %s\n",start->block_hash.hash_128);

		start = start->next;
	}
}
*/

/* HASH before
	unsigned long hash_it(unsigned long a,unsigned long difficulty_target,int* nonce, Hash* h){
    unsigned long d = difficulty_target;

	// if Hashing without difficulty target
	if(d == 0)
		d = 0x0000ffffffffffff;
    
	unsigned long c = d + 1;

    a = a * 31;
	// here the hash i.e. a <= d (difficulty)
    a %= c;
    a = a ^ (a >> 32);
    a = a ^ (a >> 16);
    
	unsigned long nonce_hash = d + (a % 65536);
	unsigned long start_hash = d;


	// Hashing with nonce
	// nonce will also be positive
	// and if it has some positive value means verification has been initiated 
	if(nonce != NULL && *nonce < 0){
		int nonce_counter = 0;
		int s = 65536;

		 while(s-- > 0) {
			if((start_hash + nonce_counter) == nonce_hash){
				break;
			}
			nonce_counter++;
		}
		*nonce = nonce_counter;
	}
	else if(nonce != NULL && *nonce > 0){
		nonce_hash = start_hash + *nonce;
	}

	//integrating the two hashes
	nonce_hash = ~nonce_hash;
	nonce_hash = nonce_hash ^ (a << 32); 
	nonce_hash = nonce_hash ^ (nonce_hash >> 32);

	// The usable Hash-64
	//a = a ^ (nonce_hash >> 16);
	unsigned long usable_hash64 = a ^ nonce_hash;

	// Hashing without Hash-128 
	if(h != NULL){
		// snprintf automatically adds the null char
	    snprintf(h->hash_128,33,"%016lx%016lx",a,nonce_hash);
		h->hash_64 = usable_hash64 ;
	}

	//returning the Hash-64
	return usable_hash64 ;

}*/

