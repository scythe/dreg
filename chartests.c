#include "charset.h"
#include <stdio.h>

int main(int argc, char **argv) {
	charset one, two, three;
	uint32_t arr1[5] = {40, 50, 70, 71, -1}, arr2[4] = {50, 73, 85, -1};
	one.parts = arr1;
	two.parts = arr2;
	printf("%d, %d, %d, %d\n", contains(one, (uint32_t) 40), contains(one, (uint32_t) 45), contains(one, (uint32_t) 50), contains(one, (uint32_t) 60));
	printf("%d, %d, %d, %d\n", contains(two, 30), contains(two, 50), contains(two, 70), contains(cscompl(two), 60));
	three = csunion(one, two);
	printf("%d, %d, %d, %d\n", contains(three, 45), contains(three, 48), contains(three, 73), contains(three, 81));
	printf("%d, %d, %d, %d, %d\n", three.parts[0], three.parts[1], three.parts[2], three.parts[3], three.parts[4]);
	return 0;
}
