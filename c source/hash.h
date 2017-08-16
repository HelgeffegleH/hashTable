typedef struct node node;
typedef struct tableData tableData;
// data structure
struct node {				// each key value pair is contained in a linked list, in their respective bucket (hash)
	node* next;
	unsigned short* key;
	unsigned short* val;
};
struct tableData {
	node** nodes;				// the buckets. Array of nodes. Nodes form a linked list.  All keys yielding the same hash are placed in the same linked list of nodes. I.e., placed in: nodes[hash]
	unsigned int* tableSizes;	// list of available array sizes (number of buckets)
	double maxLoad;				// Determines when to rehash, see hashPut
	unsigned int length;		// number of buckets (nodes in the array)
	unsigned int numkeys;		// the number of key val pairs in the table
	unsigned int nextLenInd;	// which size to expand to on the next rehash: newSize = tableSizes[nextLenInd]
};
// Function lib declarations
typedef void* __cdecl (*_malloc)(size_t);																// for allocating memory for data
typedef void __cdecl (*_free)(void*);																	// for freeing memory
typedef node* __cdecl (*findKey)(node**,unsigned short*,unsigned int,unsigned int*,unsigned int*);		// findKey()
typedef tableData* __cdecl (*_rehash)(tableData**, void*);												// rehash()
typedef tableData* __cdecl (*_newHashTable)(unsigned int,_malloc,unsigned int*,double,unsigned int);	// newHashTable()
//typedef void (*mb)(int,void*); 																			// db, typically a message box

typedef struct functionLib {
	_malloc					pmalloc;
	_free					pfree;
	findKey					pfindKey;
	_rehash					prehash;
	_newHashTable			pnewHashTable;
	//mb						pmb;	// db
} fnLib, *pfnLib;