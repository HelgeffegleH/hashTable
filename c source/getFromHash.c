#include "hash.h"
node* __cdecl getFromHash(node** nodes, unsigned int hash, unsigned int n){
	// returns the n:th node at nodes[hash], 0 if no node is found.
	// called by next for "regular" ahk for loop. _newenum()
	node* foundNode = nodes[hash];
	if (foundNode == 0 || n==1)
		return foundNode;
	int i=1; 
	while(foundNode->next != 0 && i<n) {
		foundNode=foundNode->next;
		i++;
	}
	if (i==n)
		return foundNode;
	return 0;
}
