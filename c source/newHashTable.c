#include "hash.h"

tableData* _newHashTable(unsigned int len,_malloc pmalloc,unsigned int* tableSizes,double maxLoad,unsigned int nextLenInd){
	unsigned int i;
	tableData* table = pmalloc(sizeof(tableData));	// alloc memory for tableData struct
	if (table==0)									// fail
		return 0;
	table->nodes = pmalloc(sizeof(node*)*len);		// alloc memory for node pointer array
	if (table->nodes==0)							// fail
		return 0;
	for (i=0; i<len; ++i)							// set all buckets to 0
		table->nodes[i]=0;
	
	table->tableSizes=tableSizes;					// set up members
	table->maxLoad=maxLoad;
	table->length=len;
	table->numkeys=0;
	table->nextLenInd=nextLenInd;
	return table;
}