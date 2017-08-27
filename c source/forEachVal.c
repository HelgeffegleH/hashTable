#include "hash.h"
typedef int __cdecl (*calloutFn)(unsigned short*,unsigned short*,unsigned int,unsigned int,unsigned int,void*);
int __cdecl forEachVal(tableData** table, pfnLib lib, unsigned short* val, calloutFn pcalloutFn,unsigned int cbid, void* uParams){
	unsigned int i,m;	// hash code, strcmp index.
	unsigned int j=0;	// iteration number
	int exit=0;			// return when 1. Set to 1 when calloutFn returns -2.
	node* curNode;		// node placeholders.
	node* prevNode;
	node* tmpNode;
	unsigned int k=0;
	while (val[k]!=0)	// strlen(val)
		k++;
	for (i=0; i<(*table)->length; ++i){					// goes through all nodes from hash = 0; 
		curNode = (*table)->nodes[i];			
		prevNode=0;
		while (curNode!=0){								// visit all node->next
			for(m=0; m<k; ++m)							// check if values are equal
				if (curNode->val[m]!=val[m])				
					goto nomatch;				
			if (m==k && curNode->val[m]==0){			// match
				// Switch from traverse.c
				switch (pcalloutFn(curNode->key,curNode->val,++j,cbid,i,uParams)){
					case  1:													// continue
						prevNode=curNode;
						curNode=curNode->next;
						break;
					case  0:													// stop
						return 0;
					case -2:													// remove and stop;
						exit=1;
					case -1:													// remove key - value
						tmpNode=curNode->next;
						lib->pfree(curNode->key);								// free the node which is being removed
						lib->pfree(curNode->val);
						lib->pfree(curNode);
						if (prevNode!=0)										// keep links
							prevNode->next=tmpNode;
						else
							(*table)->nodes[i]=tmpNode;
						(*table)->numkeys--;
						if (exit)
							return -2;
						curNode=tmpNode;
						break;	
				}
			} else {
				nomatch:;
				curNode=curNode->next;
			}
		}
	}
	return 1;
}