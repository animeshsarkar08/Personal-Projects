#include "header.h"

int main(int argc,char* argv[]){
	
	Block* start = NULL; 
	Block* end = NULL;
	unsigned long difficulty = 0x0000ffffffffffff;
	int n = 1;
	while(n++ < 10){
		printf("\n\n\n#####################################  Mining [%d]  #########################################\n\n", n - 1);
		create_block(&start,&end,difficulty);
		print_blocks(start);

		//every 5 sec a new block is created
		usleep(5000000);
	}

	free_blocks(start); 
	return 0;
}


