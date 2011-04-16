#include "dreg.h"
#include <math.h>
#include <stdlib.h>

int regcompare(regex *r, regex *s) {

	int ind, rs, ss;
	
	rs = !r->operands + !r->type;
	ss = !s->operands + !s->type;
	if(ss - rs)
		return ss - rs;
	switch(rs) {
		case 1:
			return strcmp(r->type, s->type);
		case 2:
			return r->capture - s->capture;
	}

	if(*r->type - *s->type)
		return *r->type - *s->type;

	for(ind = 0; r->operands[ind] || s->operands[ind]; ++ind) {
		if(!r->operands[ind])
			return -1;
		if(!s->operands[ind])
			return 1;
		
		rs = regcompare(r->operands[ind], s->operands[ind]);
		if(rs)
			return rs;
	}
	return 0;
}

int nullable(regex *r) {
	int ind, k = 1;
	if(!r->type)
		return r->capture;
	if(!r->operands)
		return 0;
	
	switch(*r->type) {
		case COMPL:
			return !nullable(r->operands[1]);
		case KLEENE:
			return 1;
		case OR:
			k = 0;
		case AND:
		case CONCAT:
			for(ind = 0; r->operands[ind]; ++ind)
				if(nullable(r->operands[ind]) - k)
					return !k;
			return k;
	}
	exit(1);
}






