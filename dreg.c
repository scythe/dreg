#include "dreg.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int regcompare(regex *r, regex *s) {
	int ind, ret;
	
	if((ret = r->type - s->type))
		return ret;

	if(r->type == SET)
		return strcmp((char *) r->set->parts, (char *) s->set->parts);

	for(ind = 0; r->operands[ind] && s->operands[ind]; ++ind)
		if((ret = regcompare(r->operands[ind], s->operands[ind])))
			return ret;

	return ((!s->operands[ind] - !r->operands[ind])<<1) + r->capture - s->capture;
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
		case SET:
			return 0;
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
/*
regex *storeg(char *sreg, int len) {
	int ind, op;
	char escape = '\\';
	char *opers = "|&*!"
	regex *result = malloc(sizeof(regex));
	for op
	for(ind = 0; ind < len; ++ind) {
		if(sreg[ind] == escape) {
			++ind;
			continue;
		}
		if(sreg[ind] == or
*/





