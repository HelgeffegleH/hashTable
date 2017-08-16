#include <windows.h>
#include "hash.h"

unsigned short* __cdecl findVal(tableData** table, pfnLib lib, unsigned short* val){
	unsigned int i,m;
	node* curNode;
	unsigned int k=0;
	while (val[k]!=0)	// strlen(val)
		k++;
	for (i=0; i<(*table)->length; ++i){					// goes through all nodes from hash = 0; 
		curNode = (*table)->nodes[i];			
		while (curNode!=0){								// visit all node->next
			for(m=0; m<k; ++m)							// check if values are equal
				if (curNode->val[m]!=val[m])				
					goto nomatch;				
			if (m==k && curNode->val[m]==0){			// match
				return curNode->key;
			}
			nomatch:;
			curNode=curNode->next;
		}
	}
	return 0;
}