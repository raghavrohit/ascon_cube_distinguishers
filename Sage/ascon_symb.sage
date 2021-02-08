#!/usr/local/bin/sage
from sage.all import *

def get_bin(byte, N):
	X = []
	for i in range(N):
		t = (byte >> (N-1-i)) & 1
		X.append(t)
	return X

V = BooleanPolynomialRing(256,['k%d'%(i) for i in range(128)]+['v%d'%(i) for i in range(128)] )
V.inject_variables()

KEY = ['k%d'%i for i in range(128)]
NONCE = ['v%d'%i for i in range(128)]
IV = get_bin(0x80400c0600000000, 64)
X0 = IV 
X1 = [k0, k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k60, k61, k62, k63]
X2 = [k64, k65, k66, k67, k68, k69, k70, k71, k72, k73, k74, k75, k76, k77, k78, k79, k80, k81, k82, k83, k84, k85, k86, k87, k88, k89, k90, k91, k92, k93, k94, k95, k96, k97, k98, k99, k100, k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112, k113, k114, k115, k116, k117, k118, k119, k120, k121, k122, k123, k124, k125, k126, k127]
X3 = [v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49, v50, v51, v52, v53, v54, v55, v56, v57, v58, v59, v60, v61, v62, v63]
X4 = [v64, v65, v66, v67, v68, v69, v70, v71, v72, v73, v74, v75, v76, v77, v78, v79, v80, v81, v82, v83, v84, v85, v86, v87, v88, v89, v90, v91, v92, v93, v94, v95, v96, v97, v98, v99, v100, v101, v102, v103, v104, v105, v106, v107, v108, v109, v110, v111, v112, v113, v114, v115, v116, v117, v118, v119, v120, v121, v122, v123, v124, v125, v126, v127]
RC = [0xF0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a, 0x4b]


def permutation(X0, X1, X2, X3, X4, rounds):

	for r in range(rounds):

	# Addition of constant
		T = get_bin(RC[r], 8)
		for i in range(8):
			X2[56 + i] = X2[56 + i] + T[i]

	# Sbox layer
		for i in range(64):
			x0 = X0[i]
			x1 = X1[i]
			x2 = X2[i]
			x3 = X3[i]
			x4 = X4[i]
			X0[i] = x4*x1 + x3 + x2*x1 + x2 + x1*x0 + x1 + x0
			X1[i] = x4 + x3*x2 + x3*x1 + x3 + x2*x1 + x2 + x1 + x0
			X2[i] = x4*x3 + x4 + x2 + x1 + 1
			X3[i] = x4*x0 + x4 + x3*x0 + x3 + x2 + x1 + x0
			X4[i] = x4*x1 + x4 + x3 + x1*x0 + x1

	
		
		A0 = X0 
		A1 = X0[-19:] + X0[:-19]
		A2 = X0[-28:] + X0[:-28]

		B0 = X1 
		B1 = X1[-61:] + X1[:-61]
		B2 = X1[-39:] + X1[:-39]

		C0 = X2 
		C1 = X2[-1:] + X2[:-1]
		C2 = X2[-6:] + X2[:-6]

		D0 = X3 
		D1 = X3[-10:] + X3[:-10]
		D2 = X3[-17:] + X3[:-17]

		E0 = X4 
		E1 = X4[-7:] + X4[:-7]
		E2 = X4[-41:] + X4[:-41]

	
	

	# Linear layer
		
		for i in range(64):
			X0[i] = A0[i] + A1[i] + A2[i] 
			X1[i] = B0[i] + B1[i] + B2[i] 
			X2[i] = C0[i] + C1[i] + C2[i] 
			X3[i] = D0[i] + D1[i] + D2[i] 
			X4[i] = E0[i] + E1[i] + E2[i] 
		
			
	
	return (X0, X1, X2, X3, X4)

# Set X4 = X3
'''
for i in range(64):
	X4[i] = X3[i]
'''


(X0, X1, X2, X3, X4) = permutation(X0, X1, X2, X3, X4, 1)


print("\n")

for i in range(1):
	print(X0[i])
	print("\n")
	print(X1[i])
	print("\n")
	print(X2[i])
	print("\n")
	print(X3[i])
	print("\n")
	print(X4[i])
	

