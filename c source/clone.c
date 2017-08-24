#include <windows.h>
#include "hash.h"
tableData* __cdecl clone(tableData** table, pfnLib lib){
	// newTable is the clone
	tableData* newTable = lib->pnewHashTable((*table)->length,lib->pmalloc,(*table)->tableSizes,(*table)->maxLoad,(*table)->nextLenInd);	// set up the new table struct to be identical to the table being cloned.
	if (newTable==0)	// fail
		return 0;
	newTable->numkeys=(*table)->numkeys;	// copy number of keys
	unsigned int i,j;
	unsigned int ki,vi; // key/val strlen
	
	unsigned short* newKey;	// key, value placeholders
	unsigned short* newVal;
	
	node* newNode;	// node placeholders	
	node* oldNode;	
	node* nextNode;		
	for (i=0; i<(*table)->length; ++i){	// visit all nodes
		oldNode = (*table)->nodes[i];

		while (oldNode!=0) {
			newNode = (node*) lib->pmalloc(sizeof(node));	// make a new node 
			ki=0;
			vi=0;
			while(oldNode->key[ki++]);	//	calculate key/val strlen.
			while(oldNode->val[vi++]);
			newKey = lib->pmalloc(ki*2);                    
			newVal = lib->pmalloc(vi*2);                    
			for (j=0; j<ki; ++j)	// copy key
				newKey[j]=oldNode->key[j];
			for (j=0; j<vi; ++j)	// copy val
				newVal[j]=oldNode->val[j];
			newNode->next=0;									// copy old values to new node
			newNode->key=newKey;                          
			newNode->val=newVal;
			oldNode=oldNode->next;
					
			nextNode = newTable->nodes[i];					
			if (nextNode==0){					// place the new node first in the bucket if no nodes there...
				newTable->nodes[i]=newNode;
				continue;
			}
			while (nextNode->next!=0)			// ... else go to the end of bucket and place it there.
				nextNode=nextNode->next;
			nextNode->next=newNode;
			
		}
	}
	return newTable; // done
}