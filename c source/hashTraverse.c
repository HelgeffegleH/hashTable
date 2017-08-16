#include <windows.h>
#include "hash.h"
typedef int __cdecl (*callbackFn)(unsigned short*,unsigned short*,unsigned int,unsigned int);
int __cdecl traverse(tableData** table, pfnLib lib, callbackFn pcallbackFn,unsigned int cbid){
	unsigned int i;		// loop ind (hash number)
	int exit = 0;		// for exiting after removed key-val pair
	node* curNode;
	node* prevNode;
	node* tmpNode;
	
	for (i=0; i<(*table)->length; ++i){										// visit all buckets
		curNode = (*table)->nodes[i];
		prevNode=0;
		while (curNode!=0){													// Visit all nodes in each bucket
			switch (pcallbackFn(curNode->key,curNode->val,cbid,i)){
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
		}
	}
	return 1;
}