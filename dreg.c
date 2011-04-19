#include "dreg.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

static uint32_t full[2] = {0, -1};
static uint32_t empty[1] = {-1};
static charset empty_charset = {empty};
static regex empty_set_regex = {SET, NULL, &empty_charset, 0, 0};
static regex *empty_string_operands[1] = {&empty_set_regex};
static regex empty_string_regex = {KLEENE, empty_string_operands, NULL, 0, 1};

int regcompare(regex *r, regex *s) {
	int ind, ret;
	
	if((ret = r->type - s->type))
		return ret;

	if(r->type == SET)
		return strcmp((char *) r->set->parts, (char *) s->set->parts);

	for(ind = 0; ind < r->oplen && ind < s->oplen; ++ind)
		if((ret = regcompare(r->operands[ind], s->operands[ind])))
			return ret;

	return (r->oplen > s->oplen ? 2 : -2) + r->capture - s->capture;
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
			for(ind = 0; ind < r->oplen; ++ind)
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

regex *newreg(char type, regex **operands, charset *set, char capture, int oplen) {
	regex *res = malloc(sizeof(regex));
	res->type = type;
	res->operands = operands;
	res->set = set;
	res->capture = capture;
	res->oplen = oplen;
	return res;
}

regex *deriv(regex *r, char a, charset *s) {
	int ind, len;
	charset working, next;
	regex *res, **derivs;

	if(s->parts)
		free(s->parts);

	switch(r->type) {

		case KLEENE:
		case COMPL:
			res = newreg(r->type, malloc(sizeof(regex *) * 2), NULL, r->capture, 2);
			res->operands[0] = deriv(r->operands[0], a, s);
			if(r->type == KLEENE && res->operands[0] != &empty_string_regex) {
					res->type = CONCAT;
					res->operands[1] = r;
					return res;
			}
			return res;

		case OR:
		case AND:
			res = newreg(r->type, malloc(sizeof(regex *) * r->oplen), NULL, r->capture, r->oplen);
			s->parts = full;

			for(ind = 0; ind < r->oplen; ++ind) {
				res->operands[ind] = deriv(r->operands[ind], a, &working);
				next = csintersection(*s, working);

				free(s->parts);
				s->parts = next.parts;
			}
			free(working.parts);
			return res;

		case CONCAT:
			s->parts = full;
			derivs = malloc(sizeof(regex *));

			for(ind = 0; ind < r->oplen && nullable(r->operands[ind]); ++ind) {
				if(ind && !(ind | (ind + 1)))
					derivs = realloc(derivs, sizeof(regex *) * 2 * (ind + 1));

				derivs[ind] = deriv(r->operands[ind], a, &working);
				next = csintersection(*s, working);
			
				free(s->parts);
				s->parts = next.parts;
			}
			if(ind < r->oplen) {
				derivs[ind] = deriv(r->operands[ind], a, &working);
				next = csintersection(*s, working);
				ind++;
				free(s->parts);
				s->parts = next.parts;
			}
			if(ind) {		
				res = newreg(OR, malloc(sizeof(regex *) * ind), NULL, r->capture, ind);

				for(ind = 0; ind < res->oplen; ++ind) {
					res->operands[ind] = newreg(CONCAT, malloc(sizeof(regex *) * (r->oplen - ind)), NULL, 0, r->oplen - ind);
					res->operands[ind]->operands[0] = derivs[ind];
					for(len = ind + 1; len < r->oplen; ++len)
						res->operands[ind]->operands[len - ind] = r->operands[len];
				}
			} else {
				res = newreg(CONCAT, malloc(sizeof(regex *) * (r->oplen + 1)), NULL, r->capture, r->oplen);
				res->operands[0] = derivs[0];
				for(ind = 1; ind < r->oplen; ind++)
					res->operands[ind] = r->operands[ind];
			}
			free(derivs);
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

regex *reduce(regex *r) {
	/*int ind;*/
	regex *res;

	switch(r->type) {
		case COMPL:
			if(r->operands[0]->type == COMPL) 
				return reduce(r->operands[0]->operands[0]);
			res = newreg(COMPL, malloc(sizeof(regex *)), NULL, r->capture, 1);
			res->operands[0] = reduce(r->operands[0]);
			return res;
		case KLEENE:
			if(r->operands[0]->type == KLEENE)
				return reduce(r->operands[0]);
			res = newreg(KLEENE, malloc(sizeof(regex *)), NULL, r->capture, 1);
			res->operands[0] = reduce(r->operands[0]);
			return res;
		default:
			return NULL; /*fix me!*/
	}
}





