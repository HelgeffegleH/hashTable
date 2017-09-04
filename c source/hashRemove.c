#include "hash.h"

int __cdecl hashRemove(tableData** table, pfnLib lib, unsigned short* key){
	unsigned int k,m;		// k, key strlen, m loop ind
	unsigned int hash=0;	// ∑_i key[i]*pow(31,i), i ∈ [0,k-1] 
	unsigned int p=1;		// power factor, p=31^i, i ∈ [0,k-1]
	
	for (k=0; key[k]!=0;++k){	// calculate key hash
		hash+=key[k]*p;
		p*=31;
	}
	
	hash =  hash % (*table)->length;			// map to valid index range
	node* prevNode=0;
	node* curNode = (*table)->nodes[hash];
	if (curNode==0)								
		return 0;								// no nodes in bucket. Didn't remove key value pair
	while (1){									
		for(m=0; m<k; ++m)						// search for key only in this bucket: (*table)->nodes[hash]
			if (curNode->key[m]!=key[m])		
				goto nomatch;				    
		if (m==k && curNode->key[m]==0){		// match
			if (prevNode!=0)					// there was a node in the bucket before the found match
				prevNode->next=curNode->next;
			else 
				(*table)->nodes[hash]=curNode->next;	// there was no previous node in this bucket
			lib->pfree(curNode->key);			// free the node and its values
			lib->pfree(curNode->val);
			lib->pfree(curNode);
			
			(*table)->numkeys--;				// decrement count
			return 1;							// removed the key value
		}
		nomatch:;
		if (curNode->next!=0) {					// continue with the next node if exist
			prevNode = curNode;
			curNode=curNode->next;
		} else {
			return 2;							// No key found in bucket.
		}
	}
	return 0; // not reachable
}