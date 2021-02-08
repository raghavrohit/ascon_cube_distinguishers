#include "main.h"
#define N 5

struct cmpBitsetN {
	bool operator()(const bitset<N>& a, const bitset<N>& b) const {
		for (int i = 0; i < N; i++) {
			if (a[i] < b[i])
				return true;
			else if (a[i] > b[i])
				return false;
		}
		return false;
	}
};

void display_result_anf(map< bitset<N>, int, cmpBitsetN >& countingBox){

	int i ;  
	auto it2 = countingBox.begin();

	while (it2 != countingBox.end()) {
		if (((*it2).second % 2) == 1) {
			
			cout << ((*it2).second % 2) << " | " << (*it2).second << "\t";
			bitset<N> tmp = (*it2).first;
			for (i = 0; i < N; i++) {
				if ((tmp[i] == 1)) cout << "x" << (i ) << " ";	
			}
			cout << endl;
		
		}
		it2++;
	}

}

void sbox_ascon(GRBModel& model, vector<GRBVar>& x, vector<GRBVar>& y) {
	
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

	

int term_enumuration_sbox(map<bitset<N>, int, cmpBitsetN>& countingBox, int target);

int term_enumuration_sbox(map<bitset<N>, int, cmpBitsetN>& countingBox, int target) {

	int i ; 
	
	try {
		
		GRBEnv env = GRBEnv();

		env.set(GRB_IntParam_LogToConsole, 0);
		//env.set(GRB_IntParam_Threads, threadNumber);
		env.set(GRB_IntParam_MIPFocus, GRB_MIPFOCUS_BESTBOUND);
		env.set(GRB_IntParam_PoolSearchMode, 2);
		env.set(GRB_IntParam_PoolSolutions, 2000000000);
		env.set(GRB_DoubleParam_PoolGap, GRB_INFINITY);
		
		GRBModel model = GRBModel(env);

		vector<vector<GRBVar>> s(2, vector<GRBVar>(N));
		
		for (i = 0; i < N; i++) {
			s[0][i] = model.addVar(0, 1, 0, GRB_BINARY);
			s[1][i] = model.addVar(0, 1, 0, GRB_BINARY);
		}
 		
 		sbox_ascon(model, s[0], s[1]) ; 
		
		GRBLinExpr ll = 0 ; 
		for(i = 0 ; i<N; i++)ll += s[1][i] ; 
		model.addConstr(ll == 1) ; 
		model.addConstr(s[1][target] == 1) ;

		model.optimize();
		
		int solCount = model.get(GRB_IntAttr_SolCount);

		for (i = 0; i < solCount; i++) {
			model.set(GRB_IntParam_SolutionNumber, i);
			bitset<N> tmp;
			for (int j = 0; j < N; j++) {
				if (round(s[0][j].get(GRB_DoubleAttr_Xn)) == 1) tmp[j] = 1;
				else tmp[j] = 0;
				}
				countingBox[tmp]++;
		}
	
		if (model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE) {
			return -1;
		}
		else if ((model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)) {
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


int sbox_anf() {
	
	for(int i = 0 ; i<N; i++){
  		map< bitset<N>, int, cmpBitsetN > countingBox;
  		term_enumuration_sbox(countingBox, i)	;
  		display_result_anf(countingBox) ;
  		cout << endl ; 
  	}
  	
  	return(0) ; 

}

  	
 


  





























