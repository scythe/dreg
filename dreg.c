#include "dreg.h"
#include <math.h>
#include <stdlib.h>

int regcompare(regex *r, regex *s) {

	int ind, ret;
	
	if(!r || !s)
		exit(1);
	
	if(!r->operands && r->type) {
		if(!s->operands && s->type)
			return strcmp(r->type, s->type);
		else if (!s->type)
			return 1;
		else
			return -1;
	}
	if(!r->type) {
		if(s->type)
			return -1;
		else
			return r->capture - s->capture;
	}
	if(!s->operands)
		return 1;
	if(*r->type - *s->type)
		return *r->type - *s->type;

	/* At this point, we know r and s are of the same type.*/
	for(ind = 0; r->operands[ind] || s->operands[ind]; ++ind) {
		if(!r->operands[ind])
			return -1;
		if(!s->operands[ind])
			return 1;
		
		ret = regcompare(r->operands[ind], s->operands[ind]);
		if(ret)
			return ret;
	}
	return 0;
}






