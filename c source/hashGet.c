#include <windows.h>
#include "hash.h"
unsigned short* __cdecl hashGet(tableData** table, pfnLib lib, unsigned short* key){
	unsigned int bucketN;
	unsigned int status;
	node* foundNode = lib->pfindKey((*table)->nodes,key,(*table)->length, &bucketN, &status); // see findKey()
	if (status == 1)
		return foundNode->val;	// return pointer to val if found
	return 0;
}