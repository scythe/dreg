#include "dreg.h"
#include <math.h>
#include <stdlib.h>

int regcompare(regex *r, regex *s) {
	int ind, ret;
	
	if(ret = r->type - s->type)
		return ret;

	switch(r->type) {
		case STRING:
			if(ret = strcmp(r->string, s->string))
				return ret;
		case MAGIC:
			return r->capture - s->capture;
	}

	for(ind = 0; r->operands[ind] && s->operands[ind]; ++ind)
		if(ret = regcompare(r->operands[ind], s->operands[ind]))
			return ret;

	return (!s->operands[ind] - !r->operands[ind])<<1 + r->capture - s->capture;
}

int nullable(regex *r) {
	int ind, k = 1;

	switch(r->type) {
		case COMPL:
			return !nullable(r->operands[0]);
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
		case STRING:
			return !r->string;
		case MAGIC:
			return r->capture;
	}
	exit(1);
}

dfalist *dfalist_add(dfalist *list, dfastate *state) {
	dfalist *new = malloc(sizeof(dfalist));
	new->state = state;
	new->next = list->next;
	list->next = new;
	return new;
}




