#include "hash.h"
typedef node* __cdecl (*getFromHash)(node**, unsigned int, unsigned int);
//									(node** nodes, unsigned int hash, unsigned int n)
// This if for the ahk for loop. _newenum()
node* __cdecl next(tableData** table, unsigned int* hash, unsigned int* n, getFromHash pgetFromHash){
	// n is the n:th key/val pair to get from bucket hash. If not exist, set n to 1 and increment hash. Else, increment n and return foundNode.
	node* foundNode=0;
	hash-=1;	//	due to numput returning the "wrong" address. Doing it in compiled version is ugly but improves performance. Mind the input.
	n-=1;		//	-- "" --
	while (*hash<(*table)->length){
		foundNode = pgetFromHash((*table)->nodes,*hash,*n);
		if (foundNode == 0){
			*n=1;
			(*hash)++;
		} else {
			(*n)++;
			return foundNode;
		}
	}
	return 0;
}