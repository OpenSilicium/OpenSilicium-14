#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <e-hal.h>

#define _BufSize   (128)
#define _BufOffset (0x01000000)
#define _SeqLen    (30)

int main(int argc, char *argv[])
{
	unsigned int i,j;
	e_platform_t platform;
	e_epiphany_t dev;

	unsigned int a;
	unsigned int b;
	unsigned int res;
	unsigned int done;
	unsigned int alldone = 0;

	srand(time(NULL));

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	if(e_init(NULL) == E_OK) {
		printf("(i) e_init done.\n");
	} else {
		fprintf(stderr, "Error initialising\n");
		return(EXIT_FAILURE);
	}

	if(e_reset_system() == E_OK) {
		printf("(i) e_reset_system done.\n");
	} else {
		fprintf(stderr, "Error system reset");
		return(EXIT_FAILURE);
	}

	if(e_get_platform_info(&platform) == E_OK) {
		printf("(i) e_get_platform_info done.\n");
	} else {
		fprintf(stderr, "Error getting plateform info");
		return(EXIT_FAILURE);
	}

	printf("=========================\n");
	printf("== Using platform version : %s\n", platform.version);
	printf("== with %ux%u cores\n", platform.rows, platform.cols);
	printf("== %u chip(s) and %u external memory segment(s)\n", platform.num_chips, platform.num_emems);
	printf("=========================\n");

	// Create the workgroup with all 16 eCore 
	if(e_open(&dev, 0, 0, platform.rows, platform.cols) == E_OK) {
		printf("(i) e_open done.\n");
	}

	for (i=0; i<platform.rows; i++) {
		for (j=0; j<platform.cols; j++) {
			a = (rand() % 1000)+1;
			b = (rand() % 1000)+1;
			res = 0;
			done = 0;
			e_write(&dev, i, j, 0x2000, &a, sizeof(a));
			e_write(&dev, i, j, 0x4000, &b, sizeof(b));
			e_write(&dev, i, j, 0x6000, &res, sizeof(res));
			e_write(&dev, i, j, 0x7000, &done, sizeof(done));
		}
	}

	// load and run the cores code
	e_load_group("e_hello_world.srec", &dev, 0, 0, platform.rows, platform.cols, E_TRUE);

	// loop while all cores done the job
	while(alldone != 0xffff) {
		for (i=0; i<platform.rows; i++){
			for (j=0; j<platform.cols;j++){
				e_read(&dev, i, j, 0x7000, &done, sizeof(done));
				if(done) {
					printf("(i) core %u,%u is done\n", i, j);
					alldone = alldone | (1 << (i*platform.cols+j));
				}
			}
		}
	}

	// get the results
	for (i=0; i<platform.rows; i++){
		for (j=0; j<platform.cols;j++){
			e_read(&dev, i, j, 0x2000, &a, sizeof(a));
			e_read(&dev, i, j, 0x4000, &b, sizeof(b));
			e_read(&dev, i, j, 0x6000, &res, sizeof(res));
			printf("result from core %u,%u with args a=%u b=%u : %u\n", i, j, a, b, res);
		}
	}

	// Release the allocated buffer and finalize the
	// e-platform connection.
	// e_free(&emem);
	e_close(&dev);
	e_finalize();

	return 0;
}

