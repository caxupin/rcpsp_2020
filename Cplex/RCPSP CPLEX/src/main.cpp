#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cfloat>
#include "cp_rcpsp.hpp"
#include <vector>

int main(int argc, const char* argv[])
{
    int rval=0;

    const char* filename = "../data/rcpsp_default.data";
    int failLimit = -1;
    if (argc > 1)
        filename = argv[1];
    if (argc > 2)
        failLimit = atoi(argv[2]);

    fprintf(stdout, "filename = %s, faillimit=%d\n", filename, failLimit);
    
    int njobs=-1;
    int *duration=NULL;
    int *release_date=NULL;
    int *latest_start=NULL;
    int *mandatory=NULL;
    double discount_rate=0.0;
    double *objfunction=NULL;
    double **tobjfunction=NULL;
    int *nsucc=NULL;
    int **succ=NULL;
    int **lag=NULL;
    int tmax=-1;
    int nresources=-1;
    int **lhs=NULL;
    int *rhs=NULL;
    int *in_sched=NULL;

    int obj_type = 2;
    double ExtractionTimeLimit = 1000000;
    double CpTimeLimit = 1000000;
    double GapLimit = 0.02;
    int numWorkers = 8;

    int *out_sched=NULL;
    double out_solval = -DBL_MAX;
    double out_ubound = -DBL_MAX;
    double out_gap = DBL_MAX;
	double in_ubound = DBL_MAX;
    std::vector<std::vector<int>> per;

    rval = load_npv(
        (char*) filename,
        &njobs,
        &duration,
        &release_date,
        &latest_start,
        &mandatory,
        &discount_rate,
        &objfunction,
        &nsucc,
        &succ,
        &lag,
        &tmax,
        &nresources,
        &lhs,
        &rhs,
        &in_sched,
		&in_ubound,
        per);
    if(rval)
    {
        fprintf(stderr, "ERROR LOADING %s\n", filename);
        goto CLEANUP;
    }

    out_sched = new int[njobs];

    rval = rcpsp_cp(
        njobs,
        duration,
        release_date,
        latest_start,
        mandatory,
        obj_type,
        discount_rate,
        objfunction,
        tobjfunction,
        nsucc,
        succ,
        lag,
        tmax,
        nresources,
        lhs,
        rhs,
        in_sched,
		in_ubound,
        out_sched,
        &out_solval,
        &out_ubound,
        &out_gap,
        failLimit,
        ExtractionTimeLimit,
        CpTimeLimit,
        GapLimit,
        numWorkers,
        per);
    if(rval)
    {
        fprintf(stderr, "ERROR SOLVING %s WITH CP\n", filename);
        goto CLEANUP;
    }

    fprintf(stdout, "val=%lf, ubound=%lf, gap=%lf\n",
        out_solval, out_ubound, out_gap);

	/*
	for(int j=0; j < njobs; j++)
		fprintf(stderr, "time[%d] = %d\n", j, out_sched[j]);
	*/

CLEANUP:

    return rval;
}
