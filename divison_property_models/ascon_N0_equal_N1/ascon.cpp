#include "main.h"

struct cmpBitset192 {
	bool operator()(const bitset<192>& a, const bitset<192>& b) const {
		for (int i = 0; i < 192; i++) {
			if (a[i] < b[i])
				return true;
			else if (a[i] > b[i])
				return false;
		}
		return false;
	}
};

void display_result_anf(vector<int> cube, map< bitset<192>, int, cmpBitset192 >& countingBox){

	int i ;  
	auto it2 = countingBox.begin(); 
	while (it2 != countingBox.end()) {
		
		if (((*it2).second % 2) == 1) {
			cout << ((*it2).second % 2) << " | " << (*it2).second << "\t";
			bitset<192> tmp = (*it2).first;
			for (i = 0; i < 64; i++) if (tmp[i] == 1) cout << "k" << (i ) << " ";
			for (i = 0; i < 64; i++) if (tmp[64+i] == 1) cout << "k" << (i+64) << " ";
			for (i = 0; i < 64; i++) if (tmp[128 + i] == 1) cout << "v" << (i) << " ";
			
			cout << endl;
		}
		
		it2++;
	}

}

// [x0, x1, x2, x3, x4] --> [y0, y1, y2, y3, y4]
void ascon_sbox(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {

	int i ; 

	vector<GRBVar> p(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) p[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> q(vector<GRBVar>(12)) ;
	for( i = 0 ; i<12; i++) q[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> r(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) r[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> s(vector<GRBVar>(8)) ;
	for( i = 0 ; i<8; i++) s[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> t(vector<GRBVar>(8)) ;
	for( i = 0 ; i<8; i++) t[i] = model.addVar(0, 1, 0, GRB_BINARY);	

	vector<GRBVar> a(vector<GRBVar>(11)) ;
	for( i = 0 ; i<11; i++) a[i] = model.addVar(0, 1, 0, GRB_BINARY);	
	

	// COPYING
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[0] >= p[i]) ; 
	model.addConstr(p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] >= x[0])	; 
	
	for( i = 0 ; i< 12 ; i++)model.addConstr(x[1] >= q[i]) ; 
	model.addConstr(q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7] + q[8] + q[9] + q[10] + q[11] >= x[1]);
	
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[2] >= r[i]) ; 
	model.addConstr(r[0] + r[1] + r[2] + r[3] + r[4] + r[5] + r[6] >= x[2]);
	
	for( i = 0 ; i< 8 ; i++)model.addConstr(x[3] >= s[i]) ; 
	model.addConstr(s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] + s[7] >= x[3]);
	
	for( i = 0 ; i< 8 ; i++)model.addConstr(x[4] >= t[i]) ; 
	model.addConstr(t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7] >= x[4]);

		
	model.addConstr(a[0] == t[0]); model.addConstr(a[0] == q[0]); 		//	x4 * x1
	model.addConstr(a[1] == r[0]); model.addConstr(a[1] == q[1]);		//	x2 * x1
	model.addConstr(a[2] == p[0]); model.addConstr(a[2] == q[2]);		//	x0 * x1											
	model.addConstr(y[0] == a[0] + a[1] + a[2] + s[0] + r[1] + q[3] + p[1]) ; 

	model.addConstr(a[3] == s[1]); model.addConstr(a[3] == r[2]); 		// x3 * x2
	model.addConstr(a[4] == s[2]); model.addConstr(a[4] == q[4]);		// x3 * x1
	model.addConstr(a[5] == r[3]); model.addConstr(a[5] == q[5]);		// x2 * x1
	model.addConstr(y[1] == a[3] + a[4] + a[5] + t[1] + s[3] + r[4] + q[6] + p[2]);
			
	model.addConstr(a[6] == t[2]); model.addConstr(a[6] == s[4]); 		//	x4 * x3
	model.addConstr(y[2] >= a[6] + t[3] + r[5] + q[7]) ; 
			
	model.addConstr(a[7] == t[4]); model.addConstr(a[7] == p[3]); 		//	x4 * x0
	model.addConstr(a[8] == s[5]); model.addConstr(a[8] == p[4]); 		//	x3 * x0
	model.addConstr(y[3] == a[7] + a[8] + t[5] + s[6] + q[8] + r[6] + p[5]) ; 
			
	model.addConstr(a[9] == t[6]); model.addConstr(a[9] == q[9]); 		//	x4 * x1
	model.addConstr(a[10] == p[6]); model.addConstr(a[10] == q[10]); 	//	x0 * x1
	model.addConstr(y[4] == a[9] + a[10] + t[7] + s[7] + q[11] ); 

}

// [x0, x1, 1 + x2, x3, x4] --> [y0, y1, y2, y3, y4]
void ascon_sboxc(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {

	int i ; 

	vector<GRBVar> p(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) p[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> q(vector<GRBVar>(10)) ;
	for( i = 0 ; i<10; i++) q[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> r(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) r[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> s(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) s[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> t(vector<GRBVar>(8)) ;
	for( i = 0 ; i<8; i++) t[i] = model.addVar(0, 1, 0, GRB_BINARY);	

	vector<GRBVar> a(vector<GRBVar>(11)) ;
	for( i = 0 ; i<11; i++) a[i] = model.addVar(0, 1, 0, GRB_BINARY);	
	

	// COPYING
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[0] >= p[i]) ; 
	model.addConstr(p[0] + p[1] + p[2] + p[3] + p[4] + p[5] + p[6] >= x[0])	; 
	
	for( i = 0 ; i< 10 ; i++)model.addConstr(x[1] >= q[i]) ; 
	model.addConstr(q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7] + q[8] + q[9] >= x[1]);
	
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[2] >= r[i]) ; 
	model.addConstr(r[0] + r[1] + r[2] + r[3] + r[4] + r[5] + r[6] >= x[2]);
	
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[3] >= s[i]) ; 
	model.addConstr(s[0] + s[1] + s[2] + s[3] + s[4] + s[5] + s[6] >= x[3]);
	
	for( i = 0 ; i< 8 ; i++)model.addConstr(x[4] >= t[i]) ; 
	model.addConstr(t[0] + t[1] + t[2] + t[3] + t[4] + t[5] + t[6] + t[7] >= x[4]);

		
	model.addConstr(a[0] == p[0]); model.addConstr(a[0] == q[0]); 		//	x4 * x1
	model.addConstr(a[1] == q[1]); model.addConstr(a[1] == r[0]);		//	x2 * x1
	model.addConstr(a[2] == q[2]); model.addConstr(a[2] == t[0]);		//	x0 * x1											
	model.addConstr(y[0] >= a[0] + a[1] + a[2] + p[1] + r[1] + s[0]) ; 

	model.addConstr(a[3] == q[3]); model.addConstr(a[3] == r[2]); 		// x3 * x2
	model.addConstr(a[4] == q[4]); model.addConstr(a[4] == s[1]);		// x3 * x1
	model.addConstr(a[5] == r[3]); model.addConstr(a[5] == s[2]);		// x2 * x1
	model.addConstr(y[1] >= a[3] + a[4] + a[5] + p[2] + r[4] + t[1]);
			
	model.addConstr(a[6] == s[3]); model.addConstr(a[6] == t[2]); 		//	x4 * x3
	model.addConstr(y[2] == a[6] + q[5] + r[5] + t[3]) ; 
			
	model.addConstr(a[7] == p[3]); model.addConstr(a[7] == s[4]); 		//	x4 * x0
	model.addConstr(a[8] == p[4]); model.addConstr(a[8] == t[4]); 		//	x3 * x0
	model.addConstr(y[3] >= a[7] + a[8] + p[5] + q[6] + r[6] + s[5] + t[5]) ; 
			
	model.addConstr(a[9] == p[6]); model.addConstr(a[9] == q[7]); 		//	x4 * x1
	model.addConstr(a[10] == q[8]); model.addConstr(a[10] == t[6]); 	//	x0 * x1
	model.addConstr(y[4] == a[9] + a[10] + q[9] + s[6] + t[7] ); 

	
}

// [0, x1, x2, x3, x3] --> [y0, y1, y2, y3, y4]
void ascon_sbox_0eq(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {

	int i ; 

	vector<GRBVar> q(vector<GRBVar>(10)) ;
	for( i = 0 ; i<10; i++) q[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> r(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) r[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> s(vector<GRBVar>(5)) ;
	for( i = 0 ; i<5; i++) s[i] = model.addVar(0, 1, 0, GRB_BINARY);	

	vector<GRBVar> a(vector<GRBVar>(6)) ;
	for( i = 0 ; i<6; i++) a[i] = model.addVar(0, 1, 0, GRB_BINARY);	
	
	for( i = 0 ; i< 10 ; i++)model.addConstr(x[0] >= q[i]) ;
	model.addConstr(q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7] + q[8] + q[9] >= x[0]);
	
	for( i = 0 ; i< 7 ; i++)model.addConstr(x[1] >= r[i]) ;
	model.addConstr(r[0] + r[1] + r[2] + r[3] + r[4] + r[5] + r[6] >= x[1]);

	for( i = 0 ; i< 5 ; i++)model.addConstr(x[2] >= s[i]) ;
	model.addConstr(s[0] + s[1] + s[2] + s[3] + s[4] >= x[2]);

	model.addConstr(a[0] == q[0]); model.addConstr(a[0] == r[0]);		
	model.addConstr(a[1] == q[1]); model.addConstr(a[1] == s[0]);												
	model.addConstr(y[0] == a[0] + a[1] + q[2] + r[1] + s[1]) ; 

	model.addConstr(a[2] == q[3]); model.addConstr(a[2] == r[2]); 		
	model.addConstr(a[3] == q[4]); model.addConstr(a[3] == s[2]);	
	model.addConstr(a[4] == r[3]); model.addConstr(a[4] == s[3]);		
	model.addConstr(y[1] == a[2] + a[3] + a[4] + q[5] + r[4]);
	
	model.addConstr(y[2] >= q[6] + r[5]) ; 
	
	model.addConstr(y[3] == q[7] + r[6]) ; 
			
	model.addConstr(a[5] == q[8]); model.addConstr(a[5] == s[4]); 		
	model.addConstr(y[4] == a[5] + q[9] ); 

	
}

// [1, x1, x2, x3, x3] --> [y0, y1, y2, y3, y4]
void ascon_sbox_1eq(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {

	int i ; 

	vector<GRBVar> q(vector<GRBVar>(8)) ;
	for( i = 0 ; i<8; i++) q[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> r(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) r[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> s(vector<GRBVar>(5)) ;
	for( i = 0 ; i<5; i++) s[i] = model.addVar(0, 1, 0, GRB_BINARY);	

	vector<GRBVar> a(vector<GRBVar>(6)) ;
	for( i = 0 ; i<6; i++) a[i] = model.addVar(0, 1, 0, GRB_BINARY);	
	
	for( i = 0 ; i< 8; i++)model.addConstr(x[0] >= q[i]) ;
	model.addConstr(q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7] >= x[0]);
	
	for( i = 0 ; i< 7; i++)model.addConstr(x[1] >= r[i]) ;
	model.addConstr(r[0] + r[1] + r[2] + r[3] + r[4] + r[5] + r[6] >= x[1]);

	for( i = 0 ; i< 5; i++)model.addConstr(x[2] >= s[i]) ;
	model.addConstr(s[0] + s[1] + s[2] + s[3] + s[4] >= x[2]);

	model.addConstr(a[0] == q[0]); model.addConstr(a[0] == r[0]);		
	model.addConstr(a[1] == q[1]); model.addConstr(a[1] == s[0]);												
	model.addConstr(y[0] >= a[0] + a[1] + r[1] + s[1]) ; 

	model.addConstr(a[2] == q[2]); model.addConstr(a[2] == r[2]); 		
	model.addConstr(a[3] == q[3]); model.addConstr(a[3] == s[2]);	
	model.addConstr(a[4] == r[3]); model.addConstr(a[4] == s[3]);		
	model.addConstr(y[1] >= a[2] + a[3] + a[4] + q[4] + r[4]);
	
	model.addConstr(y[2] >= q[5] + r[5]) ; 
	
	model.addConstr(y[3] >= q[6] + r[6]) ; 
			
	model.addConstr(a[5] == q[7]); model.addConstr(a[5] == s[4]); 		
	model.addConstr(y[4] == a[5]); 

	
}

// [0, x1, 1 + x2, x3, x3] --> [y0, y1, y2, y3, y4]
void ascon_sboxc_0eq(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {

	int i ; 

	vector<GRBVar> q(vector<GRBVar>(8)) ;
	for( i = 0 ; i<8; i++) q[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> r(vector<GRBVar>(7)) ;
	for( i = 0 ; i<7; i++) r[i] = model.addVar(0, 1, 0, GRB_BINARY);

	vector<GRBVar> s(vector<GRBVar>(6)) ;
	for( i = 0 ; i<6; i++) s[i] = model.addVar(0, 1, 0, GRB_BINARY);	

	vector<GRBVar> a(vector<GRBVar>(6)) ;
	for( i = 0 ; i<6; i++) a[i] = model.addVar(0, 1, 0, GRB_BINARY);	
	
	for( i = 0 ; i< 8; i++)model.addConstr(x[0] >= q[i]) ;
	model.addConstr(q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6] + q[7] >= x[0]);

	for( i = 0 ; i< 7; i++)model.addConstr(x[1] >= r[i]) ;
	model.addConstr(r[0] + r[1] + r[2] + r[3] + r[4] + r[5] + r[6] >= x[1]);

	for( i = 0 ; i< 6; i++)model.addConstr(x[2] >= s[i]) ;
	model.addConstr(s[0] + s[1] + s[2] + s[3] + s[4] + s[5] >= x[2]);

	model.addConstr(a[0] == q[0]); model.addConstr(a[0] == r[0]);		
	model.addConstr(a[1] == q[1]); model.addConstr(a[1] == s[0]);												
	model.addConstr(y[0] >= a[0] + a[1] + r[1] + s[1]) ; 

	model.addConstr(a[2] == q[2]); model.addConstr(a[2] == r[2]); 		
	model.addConstr(a[3] == q[3]); model.addConstr(a[3] == s[2]);	
	model.addConstr(a[4] == r[3]); model.addConstr(a[4] == s[3]);		
	model.addConstr(y[1] >= a[2] + a[3] + a[4] + r[4] + s[4]);
	
	model.addConstr(y[2] == q[4] + r[5]) ; 
	
	model.addConstr(y[3] >= q[5] + r[6]) ; 
			
	model.addConstr(a[5] == q[6]); model.addConstr(a[5] == s[5]); 		
	model.addConstr(y[4] == a[5] + q[7]); 

	
}

void ascon_linear_layer(GRBModel& model, vector<GRBVar>& X, vector<GRBVar>& Y, const int rot1, const int rot2){

	int i;
	
	vector<vector<GRBVar>> A(3, vector<GRBVar>(64));
	
	for(i = 0; i<64;i++){
		A[0][i] = model.addVar(0, 1, 0, GRB_BINARY); 
		A[1][i] = model.addVar(0, 1, 0, GRB_BINARY);
		A[2][i] = model.addVar(0, 1, 0, GRB_BINARY);
	}

	for(i = 0; i<64; i++){
		model.addConstr(X[i] >= A[0][i]); 
		model.addConstr(X[i] >= A[1][i]);
		model.addConstr(X[i] >= A[2][i]);
		model.addConstr(A[0][i] + A[1][i] + A[2][i] >= X[i]) ; 
	}

	for(i = 0; i<64; i++){
		model.addConstr(Y[i] == A[0][i] + A[1][(64-rot1 + i) % 64] + A[2][(64- rot2 + i) % 64]) ;
	} 

}

void diffusion(GRBModel& model, vector<GRBVar>& Y, vector<GRBVar>& X) {

	vector<GRBVar> Y0 (vector<GRBVar>(64)) ;
	vector<GRBVar> Y1 (vector<GRBVar>(64)) ;
	vector<GRBVar> Y2 (vector<GRBVar>(64)) ;
	vector<GRBVar> Y3 (vector<GRBVar>(64)) ;
	vector<GRBVar> Y4 (vector<GRBVar>(64)) ;
	vector<GRBVar> X0 (vector<GRBVar>(64)) ;
	vector<GRBVar> X1 (vector<GRBVar>(64)) ;
	vector<GRBVar> X2 (vector<GRBVar>(64)) ;
	vector<GRBVar> X3 (vector<GRBVar>(64)) ;
	vector<GRBVar> X4 (vector<GRBVar>(64)) ;

	for(int i = 0 ; i<64; i++){
		Y0[i] = Y[i] ; 
		Y1[i] = Y[64 + i] ;
		Y2[i] = Y[128 + i] ;
		Y3[i] = Y[192 + i] ;
		Y4[i] = Y[256 + i] ;
		
		X0[i] = X[i] ; 
		X1[i] = X[64 + i] ;
		X2[i] = X[128 + i] ;
		X3[i] = X[192 + i] ;
		X4[i] = X[256 + i] ; 
	}

	ascon_linear_layer(model, Y0, X0, 19, 28) ;
	ascon_linear_layer(model, Y1, X1, 61, 39) ; 
	ascon_linear_layer(model, Y2, X2, 1, 6) ;  
	ascon_linear_layer(model, Y3, X3, 10, 17) ; 
	ascon_linear_layer(model, Y4, X4, 7, 41) ; 

}

void substitution(GRBModel& model, vector<GRBVar>& X, vector<GRBVar>& Y, const unsigned char rc) {

	for(int i = 0 ; i< 64; i++){

		vector<GRBVar> tmpx (vector<GRBVar>(5)) ;
		vector<GRBVar> tmpy (vector<GRBVar>(5)) ;

		tmpx[0] = X[i] ; 
		tmpx[1] = X[64 + i] ;
		tmpx[2] = X[128 + i] ;
		tmpx[3] = X[192 + i ] ;
		tmpx[4] = X[256 + i] ;

		tmpy[0] = Y[i] ; 
		tmpy[1] = Y[64 + i] ;
		tmpy[2] = Y[128 + i] ;
		tmpy[3] = Y[192 + i] ;
		tmpy[4] = Y[256 + i] ;
		
		if(i >= 56){
			int t = (rc >> (63-i) ) & 1 ; 
			if(t == 1) ascon_sboxc(model, tmpx, tmpy) ; 
			else ascon_sbox(model, tmpx, tmpy) ;
		}
		else{
				ascon_sbox(model, tmpx, tmpy) ; 
		}

	}

}

int term_enumuration(vector<int> cube, vector<int> &flag, map<bitset<192>, int, cmpBitset192>& countingBox, int target, int threadNumber, int rounds, vector<int> &degree);

int term_enumuration(vector<int> cube, vector<int> &flag, map<bitset<192>, int, cmpBitset192>& countingBox, int target, int threadNumber, int rounds, vector<int> &degree) {

	int i, j, r ; 
	
	try {
		
		GRBEnv env = GRBEnv();

		env.set(GRB_IntParam_LogToConsole, 0);
		env.set(GRB_IntParam_Threads, threadNumber);
		
		// Uncomment for the ANF
		/*
		env.set(GRB_IntParam_MIPFocus, GRB_MIPFOCUS_BESTBOUND);
		env.set(GRB_IntParam_PoolSearchMode, 2);
		env.set(GRB_IntParam_PoolSolutions, 2000000000);
		env.set(GRB_DoubleParam_PoolGap, GRB_INFINITY);
		*/
		
	
		
		GRBModel model = GRBModel(env);

		vector<GRBVar> S(192) ;
		vector<vector<GRBVar>> X(rounds, vector<GRBVar>(320));
		vector<vector<GRBVar>> Y(rounds, vector<GRBVar>(320));
		
		for(r = 0 ; r<rounds; r++){
			for(i = 0; i<320; i++){
				X[r][i] = model.addVar(0, 1, 0, GRB_BINARY); 
				Y[r][i] = model.addVar(0, 1, 0, GRB_BINARY);
			}
		}
		for(i = 0; i<192; i++) {
			S[i] = model.addVar(0, 1, 0, GRB_BINARY) ;
		}
		
		uint64_t constant = 0x80400C0600000000; 
		unsigned char RC[12] = {0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b};
		
		for(i = 0 ; i<64; i++) flag[i] = (constant >> (63 - i ) ) & 1 ; 
		
		// Round 0 	
		for(i = 0 ; i< 64; i++){

			vector<GRBVar> tmpx (3) ;
			vector<GRBVar> tmpy (5) ;

			tmpx[0] = S[i] ; 
			tmpx[1] = S[64 + i] ;
			tmpx[2] = S[128 + i] ;

			tmpy[0] = Y[0][i] ; 
			tmpy[1] = Y[0][64 + i] ;
			tmpy[2] = Y[0][128 + i] ;
			tmpy[3] = Y[0][192 + i] ;
			tmpy[4] = Y[0][256 + i] ;
		
			if(i >= 56){
				int t = (RC[0] >> (63-i) ) & 1 ; 
				if(t == 1) ascon_sboxc_0eq(model, tmpx, tmpy) ; 
				else ascon_sbox_0eq(model, tmpx, tmpy) ;				// bits 56-63 in word 0 are zero
			}
			
			else{
				
				if(flag[i] == 1) ascon_sbox_1eq(model, tmpx, tmpy) ; 
				else ascon_sbox_0eq(model, tmpx, tmpy) ; 
			}

		}	
		
		diffusion(model, Y[0], X[0]) ; 
		
		for(r = 1; r<rounds; r++){
			substitution(model, X[r-1], Y[r], RC[r]) ; 
			diffusion(model, Y[r], X[r]) ; 
		}
		
		
		GRBLinExpr ks_cube = 0;
 
		for(i = 0 ; i<64;i++){
			ks_cube += S[128+i] ; 
		}
		
		model.setObjective(ks_cube, GRB_MAXIMIZE);
		
		GRBLinExpr ks = 0 ; 
		for(i = 0 ; i<320; i++) ks += X[r-1][i] ; 
		model.addConstr(ks == 1) ; 
		model.addConstr(X[r-1][target] == 1) ; 	
		
		model.optimize();
		
		int obj = round(model.getObjective().getValue()) ;
		cout << obj <<endl ;  
		degree[target] = obj ; 
		
		
		// Uncomment for the ANF
		/*
		int solCount = model.get(GRB_IntAttr_SolCount);
		
		if (solCount >= 2000000000) {
			cerr << "Number of solutions is too large" << endl;
			exit(0);
		}

		// store the information about solutions
		
		
		for (i = 0; i < solCount; i++) {
			model.set(GRB_IntParam_SolutionNumber, i);
			bitset<192> tmp;
			

			for (j = 0; j < 192; j++) {
				if (round(S[j].get(GRB_DoubleAttr_Xn)) == 1) tmp[j] = 1;
				else tmp[j] = 0;
			}


			countingBox[tmp]++;
		}
		*/
		
		
		if (model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE) {
			cout << "Infeasible"<<endl;
			return -1;
		}
		else if ((model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)) {

			//cout << "Feasible" << endl ; 
			//int obj = round(model.getObjective().getValue()) ;
			//cout << obj <<endl ;  

			int upperBound = round(model.get(GRB_DoubleAttr_ObjVal));
			return upperBound;
		}
		else {
			cout << model.get(GRB_IntAttr_Status) << endl;
			return -2;
		}

		
	}
	
	catch (GRBException e) {
		cerr << "Error code = " << e.getErrorCode() << endl;
		cerr << e.getMessage() << endl;
	}
	
	catch (...) {
		cerr << "Exception during optimization" << endl;
	}

	return -1;
}

int rate_anf(int rounds, int threadNumber) {
	
	int i; 
	
	vector<int> flag(192, 2); 
	vector<int> cube(64, 0);			// as N0 = N1
	vector<int> degree(320, 0);

	int sbox;

	// i = 1 : Word 0
	// i = 2 : Words 0, 1
	// i = 3 : Words 0, 1, 2
	// i = 4 : Words 0, 1, 2, 3
	// i = 5 : words 0, 1, 2, 3, 4

	for(sbox = 0; sbox< 64 ; sbox++){
		cout << "Sbox | " << sbox << endl ; 
		for(i = 0 ; i< 1; i++){	
  			map< bitset<192>, int, cmpBitset192 > countingBox;
  			cout << i << " " ; 
  			term_enumuration(cube, flag, countingBox, 64*i + sbox, threadNumber, rounds, degree);
  			//display_result_anf(cube, countingBox) ;
  	
  		}	

  		cout << endl<<endl;
	}
	
  	for(i = 0 ; i<64; i++){
  		cout << "Bit position of word 0: " << i << "\tDegree UB:" << degree[i] <<endl; 
  	}
  	

  	//for(i = 0 ; i< 64; i++) cout << i << "\t" << degree[i] << "\t" << degree[64 + i] << "\t" << degree[128+i]<<"\t" <<degree[192+i]<<"\t"<<degree[256+i]<<endl ; 	
	
	return 0;
 }


