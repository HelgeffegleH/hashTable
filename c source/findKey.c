typedef struct node node;
struct node {
	node* next;
	unsigned short* key;
	unsigned short* val;
};

node* __cdecl findKey(node** nodes, unsigned short* key,  unsigned int tableLength, unsigned int* bucketN, unsigned int* status){
	unsigned int k,m;		// k, key strlen, m loop ind
	unsigned int hash=0;	// ∑_i key[i]*pow(31,i), i ∈ [0,k-1] 
	unsigned int p=1;		// power factor, p=31^i, i ∈ [0,k-1]
	
	for (k=0; key[k]!=0;++k){	// calculate key hash (and key string length)
		hash+=key[k]*p;
		p*=31;
	}
	hash =  hash % tableLength;	// map to valid index range
	*bucketN = hash;			// bucket number, i.e., pos in nodes
	*status=0;					// 0, no match and no nodes in bucket. 1 if match, i.e., key is in the table. 2 if no match but bucket has nodes.
	node* outNode;
	outNode = nodes[hash];
	if (outNode==0)
		return 0;	
	while (1){															// not null
		for(m=0; m<k; ++m)
			if (outNode->key[m]!=key[m])				
				goto nomatch;				
		if (m==k && outNode->key[m]==0){								// match
			*status=1;
			return outNode;
		}
		nomatch:;
		if (outNode->next!=0) {
			outNode=outNode->next;
		} else {
			*status=2;
			return outNode;
		}
	}
	return 0; // no match
}