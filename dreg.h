/*dreg functions and accessories */
#include "charset.h"

enum TYPES {
	COMPL,
	KLEENE,
	OR,
	AND,
	CONCAT,
	SET
};

/* Represents a regular expression. The type specifies an operator from enum TYPES
 * which is carried out on the array of regexen operands. Type STRING has null operands
 * and represents the char *string; type MAGIC is the empty set if capture is 0 and an
 * always-match if capture is 1. The types COMPL and KLEENE constrain operands 
 * to be of length 1; other types allow operands to be of any length.
 */
typedef struct regular_expression {
	char type;
	struct regular_expression **operands; /* this array must be null-terminated! */
	charset *set;
	char capture;
} regex;

typedef struct automaton_state {
	regex *value;
	charset **dclasses;
	struct automaton_state **results;
} dfastate;

typedef struct dfastate_list {
	dfastate *state;
	struct dfastate_list *next;
} dfalist;

typedef struct capture_list {
	char *capture;
	int len;
	struct capture_list *next;
} caplist;

/* Adds the DFA state state to the circularly-linked list of states list. */
dfalist *dfalist_add(dfalist *list, dfastate *state);

/* Converts each regex to a "canonical form" under the similarity conventions.
 * Commutative list operators (or, and) have operands ordered according to compare(),
 * The following rules are respected: r&r = r, r|r = r, (r*)* = r*, e* = e, 
 * er = re = r, 0r = r0 = 0, 0* = e, !!r = r, r(st) = (rs)t = rst, 0&r = 0, 0|r = r
 * r&!0 = r, r|!0 = !0, r&(s&t) = (r&s)&t = r&s&t, r|(s|t) = (r|s)|t = r|s|t,
 * returns a reduced version of reg.
 */
regex *reduce(regex *reg);

/* Converts a string representing a regex to a regex struct.
 * Uses POSIX Extended Regular Expressions syntax and meta-characters:
 * "." matches any character, INCLUDING the line feed \n, [...] matches any
 * character within the brackets, [^...] matches anything not within the brackets,
 * (...) defines a capture subexpression, r* is the Kleene star, r|s is r or s, 
 * r&s is r and s, r? is r|e, r+ is rr*.
 * Probably going to be implemented in a separate library. On hold for now.
 * Probably going to use some sort of stack.
 */
regex *storeg(char *sreg, int len);

/* Attempts to match the regex in the DFA d against the string a.
 * Any captures are recorded in the capture list. Works recursively:
 * if a[0] is \x00, returns a if d->value is nullable and null if it isn't,
 * otherwise progresses to the next state depending on the derivative of
 * d->value and a[0] and matches this derivative against a+1 (the rest of the string)
 */
char *match(char *a, dfastate *d, caplist *l);

/* Finds the derivative of the regex reg with respect to the character a,
 * and populates the set s with those characters for which reg has the same
 * derivative as a. Brzozowski's rules for the derivative are as follows:
 * de/da = d0/da = 0, da/da = e, d(rs)/da = (dr/da)s|nu(r)(ds/da), d(r|s)/da = dr/da|ds/da,
 * d(r&s)/da = dr/da&ds/da, d(!r)/da = !(dr/da), and the equivalence classes are
 * determined simultaneously, so that s(r|s,a) = s(r&s,a) = s(r,a)^s(s,a), 
 * s(r*,a) = s(!r,a) = s(r,a), s(rs,a) = (r is nullable) ? s(r,a) : s(r,a)^s(s,a).
 */
regex *deriv(regex *reg, char a, charset *s);

/* Compares the regexen r and s. Returns 0 -- equal -- only if r and s are exactly equivalent;
 * that is, they have the same structure in memory -- otherwise uses a total ordering on
 * regexen: first, they are ordered by type, and then by operands, and such that a shorter operand
 * list is taken as "less than", all other things being equal. Regexen representing exact strings are
 * compared lexicographically with strcmp().
 */
int regcompare(regex *r, regex *s); /* -1 if r < s, 1 if r > s, 0 if r == s */

/* Frees the regex r and all of its components. Causes UNDEFINED BEHAVIOR if 
 * any of r's components are still in-use.
 */
int regfree(regex *r); 

/* Determines if the regex r is nullable, that is, if it matches the empty string.
 * This is done recursively: nu(r*) = 1, nu(!r) = !nu(r), nu(r|s) = nu(r) || nu(s),
 * nu(r&s) = nu(rs) = nu(r) && nu(s), nu(string) = 0.
 */
int nullable(regex *r);
