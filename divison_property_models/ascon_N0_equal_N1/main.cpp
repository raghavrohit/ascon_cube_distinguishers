#include"main.h"

int main(int argc, char const* argv[]){
	
 int threadNumber = 1 ; 
int rounds = 1 ;  	

  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-r")) rounds = atoi(argv[i + 1]);
    if (!strcmp(argv[i], "-t")) threadNumber = atoi(argv[i + 1]);
	
	}
   
	rate_anf(rounds, threadNumber);
	
	return 0;
}


