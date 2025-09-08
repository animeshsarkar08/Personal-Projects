#include <stdio.h>


unsigned long hash_it(unsigned long a, unsigned long *n){
    unsigned long s = 999999999999965536;
    unsigned long difficulty = 0x00ffffffffffffff;
    unsigned long nonce = *n;

	unsigned long acutal_data = a;
	while(s-- > 0) {
	    unsigned long data = a;
	    a = a ^ nonce;
	    
        // the random hash
        a = a * 31;
    	// here the hash i.e. a <= d (difficulty)
        a = a ^ (a >> 32);
        a = a ^ (a >> 16);
        if(a < difficulty ){
			printf("\n\nThe plain data= %ld\n\n",acutal_data);
            printf("The encrypted data = %016lx\n\n",data);
            printf("The 64-bit hash= %016lx\n\n",a);
			printf("The nonce = %ld\n\n", nonce);
            printf("The difficulty is first 16 bits must be zero = %016lx\n\n",difficulty);
			*n = nonce;
            return data;
        }
        //if(s % 100 == 0){
		printf("Trying a new nonce = %ld\n and it's hash = %016lx\n",nonce,a);
	//	usleep(100);

		//}
        nonce++;
    }
}
int main()
{
    unsigned long a = 1823654586482644;
	a = 1000000000000000;// 000f
//	a = 10000000000000;// 0000f
	a = 10000000000000000;// 00f
    // the nonce 
    unsigned long nonce = 0;
    
    unsigned long data = hash_it(a,&nonce);

    return 0;
}
