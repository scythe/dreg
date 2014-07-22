#include <stdint.h>
/*
 * A representation of character sets.
 * The array set.partitions defines set as follows:
 * If a character's unicode representation is
 * greater than an odd number of partitions, it is
 * in the set. If it is greater than an even number
 * of partitions, it is not in the set.
 * The array partitions is always terminated by UINT32_MAX = 4,294,967,295.
 */
typedef struct character_set {
	uint32_t *parts;
} charset;

/* The union of a and b. This is calculated by "scrolling" through the parts
 * of a and b, keeping track of the parts in the result. For each iteration of the loop,
 * we first check whether the result set has an odd or even number of partitions, that is,
 * whether it is "open" (odd number) or "closed" (even number). If it is open, we attempt to find
 * the "end" of the current partition by finding the first character which is greater than an
 * even number of partitions from a and an even number of partitions from b; this is the next
 * partition in the result set, and it transitions us to a "closed" state. If it is closed, we find
 * the next partition in either a or b; this is the next partition in the result set, and it transitions
 * us to an "open" state. This might perhaps be implemented more elegantly with mutual recursion...
 */
charset csunion(charset a, charset b);

/*The intersection of a and b. This is just compl(union(compl(a),compl(b))).*/
charset csintersection(charset a, charset b);

/* the complement of a. This is calculated by prepending a 0 to a.parts if
 * a.parts does not contain 0, or dropping the 0 from the beginning if it does.
 */
charset cscompl(charset a);

/*1 if a contains c; 0 if a does not contain c*/
int contains(charset a, uint32_t c);

charset *newcharset(uint32_t *parts);
