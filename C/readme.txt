// *******************************************************************************
   -- For verification of cube distinguishers as given in Section 7 of our paper 
      "Misuse-Free Key-Recovery and Distinguishing Attacks on 7-Round Ascon" accepted 
      at IACR Transactions on Symmetric Cryptology (ToSC) Volume 2021, Issue I.    
   -- C code taken from https://ascon.iaik.tugraz.at/resources.html
// *******************************************************************************

1) Change and set cube parameters in encrypt.c file
   
   1.1) Set CS = 5/8, 16, 31 for 4, 5, and 6 rounds, respectively
   1.2) Manually change number of rounds in function "P12(state* s)" in permutation.h file
   1.3) Set CS cube indices in the array CUBE[CS]

   Current setting is for 6 rounds with 31 dimensional cube given by:
   const int CUBE[CS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30} ; 

2) To run: 2.1) Type gcc encrypt.c -O3  
           2.2) ./a.out 