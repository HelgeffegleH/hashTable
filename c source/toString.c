#include <stddef.h>
typedef void* __cdecl (*realloc)(void*,size_t); 
/*
void *memcpy(  
   void *dest,  
   const void *src,  
   size_t count   
);
*/
typedef void* __cdecl (*memcpy)(void*,const void*,size_t);  

typedef struct toStringParams {
	unsigned short*	buf;			// string buffer.
	unsigned short* 	del1;		// key val delimiter
	unsigned short* 	del2;		// pair delimiter
	realloc				prealloc;	// realloc fn
	memcpy				pmemcpy;	// memccpy fn
	unsigned int 		len;		// buffer length
	unsigned int 		o;			// buffer offset
	unsigned int 		dellen1;	// delimiter 1 length.  The delimiter lengths must be 0 if not used, see 'int what'. 
	unsigned int 		dellen2;	// delimiter 2 length.  That is, dellen2 is 0 if what is 1, and dellen1 is 0 if what is 2
	int					what;		// what to get, 0 indicates both key and val, 1, only key, 2, only val.
} *pparams;
int __cdecl toString(unsigned short* key, unsigned short* val, unsigned int i, unsigned int cbid, unsigned int hash, pparams p){
	unsigned short* reBuf;
	unsigned int keylen=0;
	unsigned int vallen=0;
	if (p->what<2) {
		while (key[keylen++]); 
		keylen--;
	}
	if (p->what!=1) {
		while (val[vallen++]); 
		vallen--;
	}
	int minlen = p->o+keylen+vallen+p->dellen1+p->dellen2;
	if (minlen >= p->len){
		p->len = minlen*2;
		reBuf = p->prealloc(p->buf, p->len*2); // alloc 2 x length in bytes.
		if (reBuf==0)
			return 0; 
		p->buf = reBuf;
	}
	if (p->what<2) {
		p->pmemcpy(p->buf+p->o,key,keylen*2);
		p->o+=keylen;
		p->pmemcpy(p->buf+p->o,p->del1,p->dellen1*2);
		p->o+=p->dellen1;
	}
	if (p->what!=1) {
		p->pmemcpy(p->buf+p->o,val,vallen*2);
		p->o+=vallen;
		p->pmemcpy(p->buf+p->o,p->del2,p->dellen2*2);
		p->o+=p->dellen2;
	}
	return 1;
}