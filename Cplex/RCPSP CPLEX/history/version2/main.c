#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cp_rcpsp.hpp"
#include "loader.h"

int main(int argc, const char* argv[])
{
	const char* filename = "data/rcpsp_default.data";
	int failLimit = 10000;
	if (argc > 1)
		filename = argv[1];
	if (argc > 2)
		failLimit = atoi(argv[2]);

	fprintf(stdout, "filename = %s, faillimit=%d\n", filename, failLimit);
	int rval = loader(filename, failLimit);

	return rval;
}
