#include "hash.h"
int __cdecl multPut(tableData** table, pfnLib lib, unsigned short* keys, unsigned short* vals, unsigned short* del){
	unsigned int ko, ki;	// key offset, key index
	unsigned int vo, vi;	// val offset, val index
	
	unsigned int bucketN;	// bucket number from findKey
	unsigned int status;	// status, see findKey()
	unsigned int i,j;		// loop index, for finding/copying keys/values.
	
	node* foundNode;		// node placeholders
	node* newNode;
	unsigned short* newKey;	// key, value placeholders
	unsigned short* newVal;
	
	unsigned int dellen=0;	// delimiter string length

	while(del[dellen++]);	// calc delimiter string length
	dellen--;				// exclude null terminator from strlen.
	
	ko=0;					// init offsets and indices
	vo=0;
	ki=0;
	vi=0;
	
	int done=0;				// break main loop condition
	do { // main loop
		while(1){ 																	// find key
			while (keys[ko+ki]!=0 && keys[ko+ki]!=del[0]) 							// search for delimiter
				++ki;
			for(j=0; keys[ko+ki+j]==del[j] && j<dellen && keys[ko+ki+j]!=0; ++j);	// potentially found delimiter
			if (j==dellen)
				break;																// delim found
			if (keys[ko+ki+j]==0){ 													// end of keys.
				ki+=j;
				done=1;
				break;
			}
			ki+= j == 0 ? 1 : j;
		}
		while(1){ 																	// find val
			while (vals[vo+vi]!=0 && vals[vo+vi]!=del[0]) 							// search for delimiter
				++vi;
			for(j=0; vals[vo+vi+j]==del[j] && j<dellen && vals[vo+vi+j]!=0; ++j);	// potentially found delimiter
			if (j==dellen)
				break;																// delim found
			if (vals[vo+vi+j]==0){ 													// end of vals.
				if (done) {
					vi+= j;
					break;
				}
				return 0; // error, less values than keys.
			}
			vi+= j == 0 ? 1 : j;
		}
		
		// ko is start of key, ki is length. same for val, vi vo
		newKey = lib->pmalloc((ki+1)*2);
		for (i=0; i<ki; ++i)																	// copy key
			newKey[i]=keys[ko+i];
		newKey[ki]=0;																			// null terminate string
		
		newVal = lib->pmalloc((vi+1)*2);
		for (i=0; i<vi; ++i)																	// copy val
			newVal[i]=vals[vo+i];
		newVal[vi]=0;																			// null terminate string
		
		ko+=ki+dellen;																			// update offsets for next iteration of main loop
		vo+=vi+dellen;
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
		if ( (double) (*table)->numkeys > (double) ((*table)->maxLoad*(*table)->length) ) 		// check if rehash
			(*table) = lib->prehash(table,lib);
		// continue main loop
		
	} while (!done); // end main loop
	return 1;
}