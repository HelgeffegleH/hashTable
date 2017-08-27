#include "hash.h"

int __cdecl hashPut(tableData** table, pfnLib lib, unsigned short* key, unsigned short* val ){
	unsigned int bucketN;	// bucket number from findKey
	unsigned int status;	// status, see findKey()
	unsigned int i;			// loop index, for copying key / value.
	node* foundNode;
	unsigned int vallen=0;	// value string legngt
	while (val[vallen]!=0)	// calc value string length
		vallen++;
	unsigned short* newVal = lib->pmalloc( (vallen + 1) * 2);		// alloc memory for new value
	for (i=0; i<=vallen; ++i)																// copy the value
		newVal[i]=val[i];
	foundNode = lib->pfindKey((*table)->nodes,key,(*table)->length, &bucketN, &status);		// search for key.
	if (status==1){ 																		// Key found, just put value.
		lib->pfree(foundNode->val);
		foundNode->val=newVal;
		return 1;
	} 
	node* newNode = lib->pmalloc(sizeof(node));												// key not found, make a new node
	unsigned int keylen=0;																	// calculate key string length
	while (key[keylen]!=0)
		keylen++;
	unsigned short* newKey = lib->pmalloc((keylen + 1) * 2);								// alloc memory for new key
	for (i=0; i<=keylen; ++i)																// copy key
		newKey[i]=key[i];
	newNode->next=0;																		// Set up the new node
	newNode->key=newKey;
	newNode->val=newVal;
	if (status == 0) {																		// No node in bucket. newNode becomes the first node in the bucket
		(*table)->nodes[bucketN] = newNode;
	} else {
		foundNode->next = newNode;															// there are node(s) in the bucket, append new node to the last one
	}
	(*table)->numkeys++;																	// increment number of keys in table
	if ( (double) (*table)->numkeys > (double) ((*table)->maxLoad*(*table)->length) ) {		// check if rehash
		(*table) = lib->prehash(table,lib);
		return -1;																			// did rehash after adding new key value pair
	}
	return 0;																				// new key value pair added, no rehash
}