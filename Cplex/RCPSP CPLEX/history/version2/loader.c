#include "loader.h"

int loader(const char *filename, int failLimit)
{
	int njobs = -1;
	int *duration = NULL;
	double discount_rate = 0.0;
	int *release_date = NULL;
	int *due_date = NULL;
	int *mandatory = NULL;
	double *profit = NULL;
	double **tprofit = NULL;
	int *nsucc = NULL;
	int **succ = NULL;
	int **lag = NULL;
	int tmax = -1;
	int nresources;
	int **lhs = NULL;
	int *rhs = NULL;
	int *out_sched = NULL;

	load( (char*) filename,
		&njobs,
		&duration,
		&discount_rate,
		&profit,
		&nsucc,
		&succ,
		&lag,
		&tmax,
		&nresources,
		&lhs,
		&rhs);

	out_sched = (int*)malloc(sizeof(int)*njobs);

	fprintf(stdout, "Loaded instance with %d jobs and %d resources.\n", njobs, nresources);

	int FailLimit = -1;
	double ExtractionTimeLimit = 100000.0;
	double CpTimeLimit = 600.0;
	double GapLimit = 0.0;
	int NumWorkers = 8;
	double out_val, out_ubound, out_gap;

	rcpsp_cp(njobs,
		duration,
		release_date,
		due_date,
		mandatory,
		1,
		discount_rate,
		profit,
		tprofit,
		nsucc,
		succ,
		lag,
		tmax,
		nresources,
		lhs,
		rhs,
		NULL,
		out_sched,
		&out_val,
		&out_ubound,
		&out_gap,
		FailLimit,
		ExtractionTimeLimit,
		CpTimeLimit,
		GapLimit,
		NumWorkers);

	return 0;
}
