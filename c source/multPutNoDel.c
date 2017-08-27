#include "hash.h"
int __cdecl multPutNoDel(tableData** table, pfnLib lib, unsigned short* keys, unsigned short* vals, int constVal){
	unsigned int bucketN;	// bucket number from findKey
	unsigned int status;	// status, see findKey()
	unsigned int i,j;			// loop index, for finding/copying keys/values.
	
	node* foundNode;		// node placeholders
	node* newNode;
	unsigned short* newKey;	// key, value placeholders
	unsigned short* newVal;
	
	size_t valsize;
	unsigned int vallen;
	if (constVal){				// handle constant value
		vallen=0;
		while (vals[vallen++]); // value length including null terminator.
		valsize=vallen*2;
	} else {
		valsize=4;
	}
	
	i=0;
	while (keys[i]){
		if (constVal == 0 && vals[i]==0)
			return 0;
		newKey = lib->pmalloc(4);
		newVal = lib->pmalloc(valsize);
		newKey[0]=keys[i];						
		newKey[1]=0;
		if (constVal) {					// handle constant value
			for(j=0; j<vallen; ++j)
				newVal[j]=vals[j];		// copy val
		} else {
			newVal[0]=vals[i];						
			newVal[1]=0;
		}
		++i;
		foundNode = lib->pfindKey((*table)->nodes,newKey,(*table)->length, &bucketN, &status);	// search for key.
		if (status==1){ 																		// Key found, just put value.
			lib->pfree(foundNode->val);
			lib->pfree(newKey);
			foundNode->val=newVal;
			continue;
		} 
		newNode = lib->pmalloc(sizeof(node));													// key not found, make a new node
		newNode->next=0;																		// Set up the new node
		newNode->key=newKey;
		newNode->val=newVal;
		if (status == 0) {																		// No node in bucket. newNode becomes the first node in the bucket
			(*table)->nodes[bucketN] = newNode;
		} else {
			foundNode->next = newNode;															// there are node(s) in the bucket, append new node to the last one
		}
		(*table)->numkeys++;																	// increment number of keys in table
		if ( (double) (*table)->numkeys > (double) ((*table)->maxLoad*(*table)->length) ) 		// check if rehash
			(*table) = lib->prehash(table,lib);
		// continue main loop		
	}
	return 1;
}