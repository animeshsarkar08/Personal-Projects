#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#define _XOPEN_SOURCE
// TX_NUM is the number of transactions included in a single block
// Always be power of 2
#define TX_NUM 4

// Structures

// This is a transaction block
typedef struct Tx{
	unsigned int id;
	float amt;
}Tx;

// Merkle_Node
typedef struct Merkle_Node{
	int num;
	unsigned long* hash_arr;
	struct Merkle_Node* prev;
}Mk_node;

// The Hash
typedef struct Hash{

	//this is the Complete Hash
	char hash_128[34];

	//this is the Usable Hash for performing operations
	unsigned long hash_64;
}Hash;

// Time Structure 
typedef struct Timestamp{
	char time_string[30];
	unsigned long time_int;
}Timestamp;


// Block Header
typedef struct Block{
	unsigned int block_num;
	Hash prev_hash;
	Hash merkle_hash;
	Timestamp timestamp;
	Tx tx[TX_NUM];
	int nonce;
	unsigned long difficulty;
	Hash block_hash;
	struct Block* next;
}Block;

// Function Signatures

unsigned long squash(unsigned long prev_hash,unsigned long merkle_hash,unsigned long timestamp);
//unsigned long squash(unsigned long prev_hash,unsigned long merkle_hash);

unsigned long hash_it(unsigned long actual_data,unsigned long difficulty_target,int* nonce, Hash* h);

Mk_node* merkle_tree(Mk_node* m);

void free_merkle(Mk_node* root);

Block* create_block(Block** start,Block** end, unsigned long difficulty);

void print_blocks(Block* start);

void free_blocks(Block* start);
