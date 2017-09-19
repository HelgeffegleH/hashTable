#include "hash.h"
int __cdecl addNullDel(tableData** table, pfnLib lib, unsigned short* keys, unsigned short* vals, unsigned int n){
	unsigned int ko, ki;	// key offset, key index
	unsigned int vo, vi;	// val offset, val index
	
	unsigned int bucketN;	// bucket number from findKey
	unsigned int status;	// status, see findKey()
	unsigned int m,i;		// loop index, for finding/copying keys/values.
	
	node* foundNode;		// node placeholders
	node* newNode;
	unsigned short* newKey;	// key, value placeholders
	unsigned short* newVal;

	ko=0;					// init offsets and indices
	vo=0;
	ki=0;
	vi=0;
	
	int done=0;				// break main loop condition
	for (m=0;m<n;++m){ // main loop
		while (keys[ko+ki]!=0)						 								// find key - search for '\0' delimiter 
			++ki;
		ki = ki ? ki : 1;															// ki is at least 1 
		while (vals[vo+vi]!=0)							 							// find val - search for '\0' delimiter 
			++vi;
		vi = vi ? vi : 1;															// vi is at least 1 
		
		// ko is start of key, ki is length. same for val, vi vo
		newKey = lib->pmalloc((ki+1)*2);
		for (i=0; i<ki; ++i)														// copy key
			newKey[i]=keys[ko+i];                                                   
		newKey[ki]=0;																// null terminate string
		                                                                            
		newVal = lib->pmalloc((vi+1)*2);                                            
		for (i=0; i<vi; ++i)														// copy val
			newVal[i]=vals[vo+i];                                                   
		newVal[vi]=0;																// null terminate string
		                                                                            
		ko+=ki+1;																	// update offsets for next iteration of main loop
		vo+=vi+1;
		ki=0;
		vi=0;
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
		// Regarding the check for rehash.
		// This is not needed here becuase added check if rehash would be needed in script pre-calling this.
		// However, the bin-code in script includes this check. Change in future if rememeber. Might apply to other c-files to.
		if ( (double) (*table)->numkeys > (double) ((*table)->maxLoad*(*table)->length) ) 		// check if rehash
			(*table) = lib->prehash(table,lib);
		// continue main loop
	} // end main loop
	return 1;
}