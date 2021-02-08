#include "api.h"
#include "permutations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RATE (64 / 8)
#define PA_ROUNDS 12
#define PB_ROUNDS 6
#define IV                                                        \
  ((u64)(8 * (CRYPTO_KEYBYTES)) << 56 | (u64)(8 * (RATE)) << 48 | \
   (u64)(PA_ROUNDS) << 40 | (u64)(PB_ROUNDS) << 32)

int crypto_aead_encrypt(unsigned char* c, unsigned long long* clen,
                        const unsigned char* m, unsigned long long mlen,
                        const unsigned char* ad, unsigned long long adlen,
                        const unsigned char* nsec, const unsigned char* npub,
                        const unsigned char* k) {
  const u64 K0 = BYTES_TO_U64(k, 8);
  const u64 K1 = BYTES_TO_U64(k + 8, 8);
  const u64 N0 = BYTES_TO_U64(npub, 8);
  const u64 N1 = BYTES_TO_U64(npub + 8, 8);
  state s;
  (void)nsec;

  // set ciphertext size
  *clen = mlen + CRYPTO_ABYTES;

  // initialization
  s.x0 = IV;
  s.x1 = K0;
  s.x2 = K1;
  s.x3 = N0;
  s.x4 = N1;
  //printstate("initial value:", s);
  P12(&s);
  //s.x3 ^= K0;
  //s.x4 ^= K1;
  //printstate("initialization:", s);
  U64_TO_BYTES(c, s.x0, 8);						// Only first 64 output bits
  //U64_TO_BYTES(c + 8, s.x1, 8);
  //U64_TO_BYTES(c + 16, s.x2, 8);
  //U64_TO_BYTES(c + 24, s.x3, 8);
  //U64_TO_BYTES(c + 32, s.x4, 8);

  /*
  // process associated data
  if (adlen) {
    while (adlen >= RATE) {
      s.x0 ^= BYTES_TO_U64(ad, 8);
      P6(&s);
      adlen -= RATE;
      ad += RATE;
    }
    s.x0 ^= BYTES_TO_U64(ad, adlen);
    s.x0 ^= 0x80ull << (56 - 8 * adlen);
    P6(&s);
  }
  s.x4 ^= 1;
  printstate("process associated data:", s);

  // process plaintext
  while (mlen >= RATE) {
    s.x0 ^= BYTES_TO_U64(m, 8);
    U64_TO_BYTES(c, s.x0, 8);
    P6(&s);
    mlen -= RATE;
    m += RATE;
    c += RATE;
  }
  s.x0 ^= BYTES_TO_U64(m, mlen);
  s.x0 ^= 0x80ull << (56 - 8 * mlen);
  U64_TO_BYTES(c, s.x0, mlen);
  c += mlen;
  printstate("process plaintext:", s);

  // finalization
  s.x1 ^= K0;
  s.x2 ^= K1;
  P12(&s);
  s.x3 ^= K0;
  s.x4 ^= K1;
  printstate("finalization:", s);

  // set tag
  U64_TO_BYTES(c, s.x3, 8);
  U64_TO_BYTES(c + 8, s.x4, 8);
  */

  return 0;
}

/*********** Set cube parameters ********************/

// ********* ROUND = 4 ******************

/*
#define CS 8 
const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7} ; 


#define CS 5
const int CUBE[CS] = {0, 8, 16, 17, 34} ;
*/

// ********* ROUND = 5 ******************
/*
#define CS 16 
const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ; 
*/
// Note: Significant bias for CS = 12, 13

// ********* ROUND = 6 ******************

#define CS 31 
const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30} ; 


/***********************************************************/

void set_nonce(unsigned char* nonce, unsigned long long val){

	unsigned long long N = 0; 
	for(int i = 0 ; i<CS; i++){

		int t = (val>>i) & 1 ;
		
		if(t == 1) N |= (1ULL << (63 - CUBE[i])) ;  
		//printf("%d\t %016llx\n", t, N);

		
	}
	//printf("%d\t %016llx\n", val, N);
	
	U64_TO_BYTES(nonce, N, 8);
	//for(int i = 0 ; i<8; i++)printf("%02X ",nonce[i] );
	//printf("\n");	

  // Set N_0 = N_1
	for(int i = 0 ; i<8; i++)nonce[8+i] = nonce[i] ; 	
}


int main(int argc, char const *argv[])
{
 
    srand(time(NULL)); 
    unsigned long long clen ; 

    u8 key[16], nonce[16], ks[8], sum[8];
   

    unsigned long long exp, i, j, exp1; 

    int N1, N2;

    // Number of experiments 1<< N1
    N1 = 1<<3 ; 												
    
	// First, change number of rounds in "permutations.h" to either 4, 5 or 6
 	

	for(exp = 0 ; exp< N1 ; exp++){
		for(i = 0;  i<8 ; i++) sum[i] = 0 ; 
       	for(i = 0 ; i<16; i++)key[i] = rand() % 256 ; 
       	for(i = 0 ; i<16; i++)nonce[i] = 0 ;	// Set non-cube variables as zero

 		for(i = 0 ; i<((unsigned long long) 1<<(CS)); i++){
            set_nonce(nonce, i) ;        
            crypto_aead_encrypt(ks, &clen, NULL, 0, NULL, 0, NULL, nonce, key);
            for(j = 0; j<8; j++)sum[j] ^= ks[j] ; 
        }

        printf("Experiment: %llu | \t Cube sum: \t", exp);     
        for(i = 0 ; i<8; i++)printf("%02X ", sum[i]) ; 
        printf("\n");   
	}   

          
    

    
    
    


  


  return 0;
}