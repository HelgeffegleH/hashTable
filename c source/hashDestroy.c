#include <windows.h>
#include "hash.h"
void __cdecl destroy(tableData** table, pfnLib lib){
	unsigned int i;	// loop ind.
	node* oldNode;	// node place holders
	node* nextNode;
	// free nodes, key val.
	for (i=0; i<(*table)->length; ++i){ // visit all nodes
		oldNode = (*table)->nodes[i]; 
		while (oldNode!=0){
			nextNode=oldNode->next;		// free key value and node
			lib->pfree(oldNode->key);
			lib->pfree(oldNode->val);
			lib->pfree(oldNode);
			oldNode=nextNode;
		}
	}
	// free the old table.
	lib->pfree((*table)->nodes);
	lib->pfree((*table));
}