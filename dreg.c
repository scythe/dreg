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

int reglen(void **parray) {
	int ind;
	for(ind = 0; parray[ind]; ++ind);
	return ind;
}

regex *deriv(regex *r, char a, charset *s) {
	int ind, len, fuck;
	charset working, next;
	regex *res = malloc(sizeof(regex));
	regex **derivs;
	static uint32_t empty[1] = {-1};
	static charset empty_charset = {empty};
	static regex empty_set_regex = {SET, NULL, &empty_charset, 0};
	static regex *empty_string_operands[2] = {&empty_set_regex, NULL};
	static regex empty_string_regex = {KLEENE, empty_string_operands, NULL, 0};

	if(s->parts)
		free(s->parts);

	res->set = NULL;
	res->capture = r->capture;
	res->type = (char) 10;
	switch(r->type) {
		case COMPL:
			res->operands = malloc(sizeof(regex *) * 2);
			res->operands[0] = deriv(r->operands[0], a, s);
			res->operands[1] = NULL;
			res->type = COMPL;
			return res;

		case KLEENE:
			res->type = CONCAT;
			res->operands = malloc(sizeof(regex *) * 3);
			res->operands[0] = deriv(r->operands[0], a, s);
			res->operands[1] = r;
			res->operands[2] = NULL;
			return res;

		case OR:
			res->type = OR;
		case AND:
			if(res->type == 10)
				res->type = AND;

			res->operands = malloc(sizeof(regex *) * reglen((void **) r->operands));
			s->parts = malloc(sizeof(uint32_t) * 2);
			s->parts[0] = 0;
			s->parts[1] = -1; /*works just fine thank you very much*/

			for(ind = 0; r->operands[ind]; ++ind) {

				res->operands[ind] = deriv(r->operands[ind], a, &working);
				next = csintersection(*s, working);

				free(s->parts);
				s->parts = next.parts;
			}
			free(working.parts);
			return res;

		case CONCAT:

			s->parts = malloc(sizeof(uint32_t) * 2);
			s->parts[0] = 0;
			s->parts[1] = -1; /*works just fine thank you very much*/
			derivs = malloc(sizeof(regex *));

			for(ind = 0; r->operands[ind] && nullable(r->operands[ind]); ++ind) {
				if(ind && !(ind | (ind + 1)))
					derivs = realloc(derivs, sizeof(regex *) * 2 * (ind + 1));

				derivs[ind] = deriv(r->operands[ind], a, &working);
				next = csintersection(*s, working);
			
				free(s->parts);
				s->parts = next.parts;
			}

			derivs[ind] = deriv(r->operands[ind], a, &working);
			next = csintersection(*s, working);

			free(s->parts);
			s->parts = next.parts;

			if(ind) { /* there are probably memory leaks here, but my roommates won't shut up and I can't focus */
				res->type = OR;			
				res->operands = malloc(sizeof(regex *) * (ind + 2));
				res->operands[ind + 1] = NULL;
				
				len = reglen((void **) r->operands);
				for(ind = 0; res->operands[ind]; ++ind) {
					res->operands[ind]->type = CONCAT;
					res->operands[ind]->operands = malloc(sizeof(regex *) * (len - ind));
					res->operands[ind]->operands[0] = derivs[ind];
					for(fuck = ind + 1; (res->operands[ind]->operands[fuck] = r->operands[fuck]); ++fuck);
				}
			} else {
				res->type = CONCAT;
				res->operands = malloc(sizeof(regex *) * reglen((void **) r->operands));
				res->operands[0] = derivs[0];
				for(ind = 1; (res->operands[ind] = r->operands[ind]); ind++);
			}
			return res;

		case SET:
			if(contains(*r->set, a)) {
				s->parts = r->set->parts;
				return &empty_string_regex;
			}
			s->parts = cscompl(*r->set).parts;
			return &empty_set_regex;
	}
	return &empty_set_regex;
}








