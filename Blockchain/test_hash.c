#include <stdio.h>
typedef struct Hash{

	//this is the Complete Hash
	char hash_128[34];

	//this is the Usable Hash for performing operations
	unsigned long hash_64;
}Hash;

void hash_it(unsigned long a,int* nonce, Hash* h);

int main()
{
   
    unsigned long a = 1823654586482643;
	int nonce = 0;
	Hash h;
	hash_it(a,&nonce,&h);
	printf("%s\n%d\n%016lx\n",h.hash_128,nonce,h.hash_64);

    return 0;
}
void hash_it(unsigned long a,int* n, Hash* h){
    unsigned long c = 0x0001000000000000;
    unsigned long d = 0x0000ffffffffffff;
    a = a * 31;
	// here the hash i.e. a <= d (difficulty)
    a %= c;
    a = a ^ (a >> 32);
    a = a ^ (a >> 16);
    
    //printf("real_hash = %016lx\n",a);
	unsigned long nonce_hash = d + (a % 65536);
	printf("%ld\n",a % 65536);
  	int nonce = 0;
    int s = 65536;
    unsigned long start_hash = d;
	 while(s-- > 0) {
        if((start_hash + nonce) == nonce_hash){
            //printf("nonce_hash= %016lx\n",nonce_hash);
			//printf("%d\n", nonce);
            break;
        }
        nonce++;
    }

	//integrating the two hashes
	//printf("%016lx\n",nonce_hash);
	nonce_hash = ~nonce_hash;
	//printf("%016lx\n",nonce_hash);
	nonce_hash = nonce_hash ^ (a << 32); 
	nonce_hash = nonce_hash ^ (nonce_hash >> 32);
	// snprintf automatically adds the null char
    snprintf(h->hash_128,33,"%016lx%016lx",a,nonce_hash);
	h->hash_64 = a ^ (nonce_hash >> 16);
	*n = nonce;
	//printf("nonce = %d\n",nonce);
	return;

}
