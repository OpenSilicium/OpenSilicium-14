#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <e_lib.h>

int main(void) {
	unsigned *a, *b, *res, *done;
//	unsigned int tmp;
//	unsigned int i;

	a = (unsigned *) 0x2000;
	b = (unsigned *) 0x4000;
	res = (unsigned *) 0x6000;
	done = (unsigned *) 0x7000;


	(*(res)) = (*(a)) * (*(b));

	/*
	tmp = 1;
	for (i = 1; i <= (*(a)); i++)
		tmp = tmp * i;
	(*(res)) += tmp;

	tmp = 1;
	for (i = 1; i <= (*(b)); i++)
		tmp = tmp * i;
	(*(res)) += tmp;
	*/

	(*(done)) = 1;

	__asm__ __volatile__("idle");

	return 0;
}
