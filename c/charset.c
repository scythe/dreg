#include "charset.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

charset csunion(charset a, charset b) {
	int ai = 0, bi = 0, ni = 0, psize = 1;
	charset uni;
	uni.parts = malloc(2 * sizeof(uint32_t));
	uni.parts[0] = MIN(a.parts[0], b.parts[0]);
	do {
		if(ni % 2) { /*we're finding the beginning of a set*/
			bi++;
			ai++;

			if(++ni >= pow(2, psize)) /* since parts is even-sized, it is only ever necessary to reallocate when ni is odd... */
				uni.parts = realloc(uni.parts, pow(2, ++psize) * sizeof(uint32_t));

			uni.parts[ni] = MIN(a.parts[ai], b.parts[bi]);
		} 
		else {		 /* we're finding the end of a set.*/
			/*first go to the end of the current partitions */
			ai++;
			bi++;

			/* if a new partition starts before the end of the current partition,
			 * go to the end of that partition instead.
			 */
			while(1) {
				if(a.parts[ai] > b.parts[bi + 1] && (b.parts[bi + 1] + 1)) {
					bi += 2;
					continue;
				}
				if(b.parts[bi] > a.parts[ai + 1] && (a.parts[ai + 1] + 1)) {
					ai += 2;
					continue;
				}
				break;
			}

			uni.parts[++ni] = MAX(a.parts[ai], b.parts[bi]); 
		}
		
	} while(uni.parts[ni] + 1);

	return uni;
}

charset cscompl(charset a) {
	int ai = 0, ni = 0, psize = 1;
	charset compl;
	compl.parts = malloc(3 * sizeof(uint32_t));

	/* if a starts with 0, we skip it; otherwise, we prepend 0... */
	if(a.parts[0] == 0)
		ai = 1;
	else
		compl.parts[ni++] = 0;

	/* ...and copy the rest of the indices of a. This guarantees that a and compl[a] are mutually exclusive and exhaustive. */
	while(a.parts[ai] + 1) {
		compl.parts[ni] = a.parts[ai++];

		if(++ni > (pow(2, psize) - 1)) {
			compl.parts = realloc(compl.parts, pow(2, ++psize) * sizeof(uint32_t));
		}
	}
	compl.parts[ni] = -1;

	return compl;
}

charset csintersection(charset a, charset b) {
	return cscompl(csunion(cscompl(a), cscompl(b))); /* don't repeat yourself! */
}

int contains(charset a, uint32_t c) {
	int i;
	for(i = 0; a.parts[i] + 1; ++i) 
		if(c <= a.parts[i])
			break;
	return i % 2;
}

