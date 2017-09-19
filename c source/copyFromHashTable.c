#include "hash.h"
typedef int __cdecl (*put)(tableData**, pfnLib, unsigned short*, unsigned short*);
typedef struct copyParams {
	tableData** destTable;
	pfnLib 		lib;
	put 		pput;
} *pparams;
int __cdecl copyFromHashTable(unsigned short* key, unsigned short* val, unsigned int i, unsigned int cbid, unsigned int hash, pparams p){
	p->pput(p->destTable,p->lib,key,val);
	return 1;
}