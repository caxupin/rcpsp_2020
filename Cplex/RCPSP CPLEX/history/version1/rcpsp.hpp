#include <ilcp/cp.h>

class FileError: public IloException {
public:
    FileError() : IloException("Cannot open data file") {}
};

int load( char *fname,
    int *pnjobs,
    int **pduration,
    double *pdiscount_rate,
    double **pprofit,
    int **pnsucc,
    int ***psucc,
    int ***plag,
    int *ptmax,
    int *pnresources,
    int ***plhs,
    int **prhs);

int rcpsp(
    int njobs,
    int *duration,        // duration[j] is duration of job j
    int *release_date,    // release_date[j] is release date of job j, -1 if none
    int *latest_end_date, // latest_end_date[j] is the latest time period one can end job j, -1 if none
	int *mandatory,       // 1 if this job has to be scheduled, 0 if not, -1 if none
	int obj_type,         // 1 if makespan, 2 if npv-disc-end, 3 if npv-disc-start, 4 if time-dependent
    double discount_rate,
    double *profit,       // profit[j] profit of job j
	double **tprofit,     // tprofit[j][t] profit of job j starting in time t
    int *nsucc,           // nsucc[j] number of successors of job j
    int **succ,           // succ[j][i] is the i-th successor of j
    int **lag,            // lag[j][i] is the lag of job j's i-th successor arc (we assume it is of type end-start)
    int tmax,             // number of time periods
    int nresources,       // number of resources
    int **lhs,            // lhs[j][r] indicates the amount of resource r consumed by job j in every period
    int *rhs,             // amount of resource r available every period
	int *in_sched,        // in_sched[j] input solution for hot start (format described below) - NULL if no hot start
    int *out_sched,       // out_sched[j] is the time in which job j is scheduled to start, or, it is -1
	double *out_solval,
	double *out_ubound,
	double *out_gap,
	int failLimit,
	double ExtractionTimeLimit,
	double CpTimeLimit,
	double GapLimit,
	int numWorkers);



