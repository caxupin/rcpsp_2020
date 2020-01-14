#include <ilcp/cp.h>
#include <ilcp/cpext.h>
#include "cp_rcpsp.hpp"
#include <cfloat>

class FileError: public IloException {
public:
    FileError() : IloException("Cannot open data file") {}
};

int load_npv( char *fname,
    int *pnjobs,
    int **pduration,
    int **prelease_date,
    int **platest_end_date,
    int **pmandatory,
    double *pdiscount_rate,
    double **pprofit,
    int **pnsucc,
    int ***psucc,
    int ***plag,
    int *ptmax,
    int *pnresources,
    int ***plhs,
    int **prhs,
    int **pin_sched,
	double *pin_ubound)
{

    int njobs = -1;
    int *duration = NULL;
    double discount_rate = 0.0;
    double *profit = NULL;
    int *nsucc = NULL;
    int **succ = NULL;
    int **lag = NULL;
    int tmax = -1;
    int nresources;
    int **lhs = NULL;
    int *rhs = NULL;
    int index = -1;
    int *in_sched = NULL;
    int *releasedate = NULL, *duedate = NULL;
    int *mandatory = NULL;
    double *objective = NULL;
	double in_ubound = DBL_MAX;

    try 
    {
        // OPEN FILE IN CPLEX FORMAT

        std::ifstream file(fname);

        if (!file)
        {
            std::cout << "unknown file: " << fname << std::endl;
            throw FileError();
        }

        // READ FIRST LINE

        file >> njobs;
        file >> nresources;
        file >> discount_rate;
		file >> tmax;
		file >> in_ubound;

        // ALLOCATE MEMORY FOR ARRAYS

        duration = new int[njobs];
        nsucc = new int[njobs];
        succ = new int*[njobs];
        lag = new int*[njobs];
        lhs = new int*[njobs];
        rhs = new int[nresources];
        in_sched = new int[njobs];
        releasedate = new int[njobs];
        duedate = new int[njobs];
        mandatory = new int[njobs];
        objective = new double[njobs];

        // READ RESOURCE AVAILABILITIES

        for(int r = 0; r < nresources; r++)
            file >> rhs[r];

        // READ JOB LINES

        for(int j = 0; j < njobs; j++)
        {
            file >> index;
            file >> duration[j];
            file >> releasedate[j];
            file >> duedate[j];
            file >> mandatory[j];
            file >> in_sched[j];
            file >> objective[j];
            lhs[j] = new int[nresources];
            for(int r = 0; r < nresources; r++)
                file >> lhs[j][r];
            file >> nsucc[j];
            succ[j] = new int[nsucc[j]];
            lag[j] = new int[nsucc[j]];
            for(int s = 0; s < nsucc[j]; s++)
            {
                file >> succ[j][s];
                file >> lag[j][s];
            }
        }
    }
    catch(IloException& e)
    {
        std::cout << " ERROR: " << e << std::endl;
    }

    *pnjobs = njobs;
    *pduration = duration;
    *prelease_date = releasedate;
    *platest_end_date = duedate;
    *pmandatory = mandatory;
    *pdiscount_rate = discount_rate;
    *pprofit = objective;
    *pnsucc = nsucc;
    *psucc = succ;
    *plag = lag;
    *ptmax = tmax;
    *pnresources = nresources;
    *plhs = lhs;
    *prhs = rhs;
    *pin_sched = in_sched;
	*pin_ubound = in_ubound;

    return 0;

}

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
    int **prhs)
{

    int njobs = -1;
    int *duration = NULL;
    double discount_rate = 0.0;
    double *profit = NULL;
    int *nsucc = NULL;
    int **succ = NULL;
    int **lag = NULL;
    int tmax = -1;
    int nresources;
    int **lhs = NULL;
    int *rhs = NULL;

    try 
    {
        // OPEN FILE IN CPLEX FORMAT

        std::ifstream file(fname);

        if (!file)
        {
            std::cout << "unknown file: " << fname << std::endl;
            throw FileError();
        }

        // READ FIRST LINE

        file >> njobs;
        file >> nresources;

        // ALLOCATE MEMORY FOR ARRAYS

        duration = new int[njobs];
        nsucc = new int[njobs];
        succ = new int*[njobs];
        lag = new int*[njobs];
        lhs = new int*[njobs];
        rhs = new int[nresources];

        // READ RESOURCE AVAILABILITIES

        for(int r = 0; r < nresources; r++)
            file >> rhs[r];

        // READ JOB LINES

        for(int j = 0; j < njobs; j++)
        {
            file >> duration[j];
            lhs[j] = new int[nresources];
            for(int r = 0; r < nresources; r++)
                file >> lhs[j][r];
            file >> nsucc[j];
            succ[j] = new int[nsucc[j]];
            lag[j] = new int[nsucc[j]];
            for(int s = 0; s < nsucc[j]; s++)
            {
                file >> succ[j][s];
                succ[j][s] -= 1; // we enumerate from zero internally
                lag[j][s] = duration[j];
            }
        }
    }
    catch(IloException& e)
    {
        std::cout << " ERROR: " << e << std::endl;
    }

    *pnjobs = njobs;
    *pnresources = nresources;
    *pduration = duration;
    *pdiscount_rate = 0.0;
    *pprofit = NULL;
    *pnsucc = nsucc;
    *psucc = succ;
    *plag = lag;
    *ptmax = tmax;
    *plhs = lhs;
    *prhs = rhs;

    return 0;

}

#ifdef __cplusplus
extern "C" 
{
#endif
int rcpsp_write(
    int njobs,
    int *duration,        
    int *release_date,    
    int *latest_end_date, 
	int *mandatory,       
	int obj_type,         
    double discount_rate,
    double *profit,       
	double **tprofit,     
    int *nsucc,           
    int **succ,           
    int **lag,            
    int tmax,             
    int nresources,       
    int **lhs,            
    int *rhs,             
	int *in_sched,        
	double in_ubound,
    int *out_sched,       
	double *out_solval,
	double *out_ubound,
	double *out_gap,
	int failLimit,
	double ExtractionTimeLimit,
	double CpTimeLimit,
	double GapLimit,
	int numWorkers,
	int with_headers)
{

	// LINE 1
	if(with_headers)
	    fprintf(stdout, "  njobs nresources discount_rate tmax ubound\n");
    fprintf(stdout, "%7d %10d %13lf %4d %lf\n", njobs, nresources, discount_rate, tmax, in_ubound);

	// LINE 2
	if(with_headers)
	{
		for(int r=0; r < nresources; r++)
			fprintf(stdout, "%8d_rhs ", r);
		fprintf(stdout, "\n");
	}

    for(int r=0; r < nresources; r++)
        fprintf(stdout, "%12d ", rhs[r]); 
    fprintf(stdout, "\n");

	if(with_headers)
	{
		fprintf(stdout, "ind dur rel due man sch objective_function ");
		for(int r=0; r < nresources; r++)
			fprintf(stdout, "%3d_resource ", r); 
		fprintf(stdout, "out\n");
	}

    for(int j=0; j < njobs; j++)
    {
        int dur = (duration ? duration[j] : 0);
        int rel = (release_date ? release_date[j] : 0);
        int due = (latest_end_date ? latest_end_date[j] : -1);
        int man = (mandatory ? mandatory[j] : 0);
        int sch = (in_sched ? in_sched[j] : -1);
        double obj = (profit ? profit[j] : 0.0);

        fprintf(stdout, "%3d %3d %3d %3d %3d %3d %18lf ", j, dur, rel, due, man, sch, obj);

        for(int r=0; r < nresources; r++)
            fprintf(stdout, "%12d ", lhs[j][r]);

        fprintf(stdout, "%3d ", nsucc[j]);

        for(int s = 0; s < nsucc[j]; s++)
            fprintf(stdout, "%3d %3d ", succ[j][s], lag[j][s]);
        fprintf(stdout, "\n");
    }
    return 0;
}
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" 
{
#endif
int rcpsp_cp(
    int njobs,
    int *duration,        // duration[j] is duration of job j
    int *release_date,    // release_date[j] is release date of job j, -1 if none
    int *latest_start_date, // latest_end_date[j] is the latest time period one can end job j, -1 if none
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
	double in_ubound,
    int *out_sched,       // out_sched[j] is the time in which job j is scheduled to start, or, it is -1
	double *out_solval,
	double *out_ubound,
	double *out_gap,
	int failLimit,
	double ExtractionTimeLimit,
	double CpTimeLimit,
	double GapLimit,
	int numWorkers)
{
    IloEnv env;

	if(1)
    rcpsp_write(
        njobs,
        duration,        
        release_date,    
        latest_start_date, 
	    mandatory,       
	    obj_type,         
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
	    in_sched,        
		in_ubound,
        out_sched,       
	    out_solval,
	    out_ubound,
	    out_gap,
	    failLimit,
	    ExtractionTimeLimit,
	    CpTimeLimit,
	    GapLimit,
	    numWorkers,
		0);

	int rval = test_rcpsp_solution(
        njobs,
        duration,        
        release_date,    
        latest_start_date, 
	    mandatory,       
	    obj_type,         
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
	    in_sched);        
	if(rval)
		exit(1);

	fprintf(stderr, "TESTED INPUT SOLUTION AND FOUND NO ERRORS (%d).\n", rval);

    try {

        IloModel model(env);
        IloInt nbTasks, nbResources, i, j, r;

        nbTasks = njobs;

        // DEFINE VARIABLES

        IloIntervalVarArray task_variables(env, nbTasks);

        for (j=0; j<nbTasks; j++) 
        {
            task_variables[j] = IloIntervalVar(env);
            if(duration)
            {
                task_variables[j].setSizeMin(duration[j]);
                task_variables[j].setSizeMax(duration[j]);
            }
            else
            {
                task_variables[j].setSizeMin(1);
                task_variables[j].setSizeMax(1);
            }

            task_variables[j].setOptional();

			/*
			if(obj_type == 1) 
				task_variables[j].setPresent();
            else if(!mandatory)
                task_variables[j].setOptional();
            else if(!mandatory[j])
                task_variables[j].setOptional();
            else
                task_variables[j].setPresent();
			*/
        }

        // DEFINE PRECEDENCE CONSTRAINTS

		
        for (j=0; j<nbTasks; j++)
        {
            IloInt nbSucc = nsucc[j];
            for (IloInt s=0; s<nbSucc; ++s) 
			{
				model.add(IloPresenceOf(env,task_variables[succ[j][s]]) <= IloPresenceOf(env,task_variables[j]));
                model.add(IloStartBeforeStart(env, task_variables[j], task_variables[succ[j][s]], lag[j][s])); // StartBeforeStart
            }
        }

        // DEFINE RELEASE DATES AND DUE DATES
		/*
		if(release_date)
		{
            for (j=0; j<nbTasks; j++)
				if(release_date[j] > 0)
					model.add(IloStartOf(task_variables[j]) >= release_date[j]);
		}
		*/

		if(latest_start_date)
		{
            for (j=0; j<nbTasks; j++)
				if(latest_start_date[j] > (-1))
					model.add(IloStartOf(task_variables[j]) <= latest_start_date[j]);
		}

        // DEFINE RESOURCE CONSTRAINTS

        nbResources = nresources;
        IloCumulFunctionExprArray resource_consumptions(env, nbResources); // note this is per time period

        for (r=0; r<nbResources; r++)
        {
            resource_consumptions[r] = IloCumulFunctionExpr(env);

            for (j=0; j<nbTasks; j++)
                if (lhs[j][r] > 0) 
                    resource_consumptions[r] += IloPulse(task_variables[j], lhs[j][r]);

            model.add(resource_consumptions[r] <= rhs[r]);
        }

        // DEFINE OBJECTIVE FUNCTION

		if(obj_type == 1)
		{
			IloIntExprArray ends(env, nbTasks); // expression whose result is int. ex: 3*x[4] - 10*x[5]^2, where x are vars

			for (j=0; j<nbTasks; j++)
				ends[j] = IloEndOf(task_variables[j]); //represents end time of the task (dynamic expression)

			IloObjective objective = IloMinimize(env,IloMax(ends));
			model.add(objective);
		}
		else if(obj_type == 2 || obj_type == 3)
		{
			IloNumExprArray profitsExpr(env, nbTasks);
			for (j=0; j<nbTasks; j++)
			{
				IloExpr pexp;
				if(obj_type == 2)
					pexp = profit[j] / IloPower(1.0 + discount_rate, IloEndOf(task_variables[j]));
				else
					pexp = profit[j] / IloPower(1.0 + discount_rate, IloStartOf(task_variables[j]));
				profitsExpr[j] = pexp*IloPresenceOf(env,task_variables[j]);
			}
			IloObjective objective = IloMaximize(env,IloSum(profitsExpr));
			model.add(objective);
		}
		else if(obj_type == 4)
		{
			IloNumExprArray profitsExpr(env, nbTasks);
			for (j=0; j<nbTasks; j++)
			{
				IloNumExpr P(env);
				for( int t = 0; t < tmax; t++ )
					P += (IloStartOf(task_variables[j]) == t)*tprofit[j][t];
				profitsExpr[j] = P*IloPresenceOf(env, task_variables[j]);
			}
			IloObjective objective = IloMaximize(env,IloSum(profitsExpr));
			model.add(objective);
		}
		else
		{
			fprintf(stderr, "error: unknown obj_type = %d.\n", obj_type);
			exit(1);
		}

		// CREATE MODEL
        IloCP cp(model);

		// PROVIDE SOLVER WITH INPUT SOLUTION
		if(in_sched)
		{
			IloSolution hotStart(env);
			int cnt=0;
			for (j=0; j<nbTasks; j++)
			{
				if(in_sched[j] > (-1))
				{
					hotStart.setPresent(task_variables[j]);
					hotStart.setStart(task_variables[j], in_sched[j]);
                    if(duration)
					    hotStart.setEnd(task_variables[j], in_sched[j] + duration[j]);
                    else
					    hotStart.setEnd(task_variables[j], in_sched[j] + 1);
					cnt += 1;
				}
				else
					hotStart.setAbsent(task_variables[j]);
			}
			if(cnt)
				cp.setStartingPoint(hotStart);
		}

        // SOLVE MODEL

		// four is the highest display level
		cp.setParameter(IloCP::WarningLevel, 3);
		// if I want this thing to shut up (options: Quiet, Normal)
		//cp.setParameter(IloCP::LockVerbosity, IloCP::ParameterValues::Quiet);
		cp.setParameter(IloCP::LogVerbosity, IloCP::ParameterValues::Normal);
		//cp.setParameter(IloCP::LogPeriod, 50000); //how often it prints to screen

		if(failLimit > (-1))
        	cp.setParameter(IloCP::FailLimit, failLimit);
		else
			fprintf(stderr, "CP:No Fail Limits Imposed.\n");
		if( numWorkers > (-1) )
        	cp.setParameter(IloCP::Workers, numWorkers);
		else
			fprintf(stderr, "CP:No numWorkers Limit Imposed.\n");

        cp.out() << "Instance \t: RCPSP " << std::endl;

		if( ExtractionTimeLimit > (-1) )
        	cp.setParameter(IloCP::TimeLimit, ExtractionTimeLimit);
		else
			fprintf(stderr, "CP:No Extraction Time Limits Imposed.\n");

		cp.startNewSearch();

		if( CpTimeLimit > (-1) )
			cp.setParameter(IloCP::TimeLimit, cp.getInfo(IloCP::ExtractionTime) + CpTimeLimit);
		else
			fprintf(stderr, "CP:No CP Time Limits Imposed.\n");

		*out_gap = 1e10;

        try
        {

            while(cp.next() && (*out_gap >= GapLimit))
			{
				*out_ubound = cp.getObjBound();
				*out_solval = cp.getObjValue();
				*out_gap = cp.getObjGap();

				double ub = (*out_ubound < in_ubound) ? (*out_ubound) : in_ubound;
				double ogap = (1 - (*out_solval/(ub+0.0001)));

				//fprintf(stderr, "ISALLFIXED=%d\n", cp.isAllFixed());
				fprintf(stdout, "FOUND SOL WITH VAL %lf | %lf (GAP = %lf).\n", *out_solval, ub, ogap);

				if(ogap < GapLimit)
					break;
			}
            cp.endSearch();
        }
        catch(IloException& e)
        {
            env.out() << " ERROR: " << e << std::endl;
        }

		if( cp.getInfo(IloCP::IntInfo::NumberOfSolutions) )
		{
		    *out_ubound = cp.getObjBound();
		    *out_solval = cp.getObjValue();
		    *out_gap = cp.getObjGap();

			double ub = (*out_ubound < in_ubound) ? (*out_ubound) : in_ubound;
			double ogap = (1 - (*out_solval/(ub+0.0001)));

			*out_ubound = ub;
			*out_gap = ogap;

        	cp.out() << "Objval: " << *out_solval << std::endl;
        	cp.out() << "Upper Bound: " << *out_ubound << std::endl;
        	cp.out() << "Gap: " << *out_gap << std::endl;

        	for (j=0; j<nbTasks; j++)
			{
				if( cp.isPresent(task_variables[j]) )
            		out_sched[j] = cp.getStart(task_variables[j]);	
				else
					out_sched[j] = -1;
			}

			int cnterr = test_rcpsp_solution(
				njobs,
				duration,        
				release_date,    
				latest_start_date, 
				mandatory,       
				obj_type,         
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
				out_sched);

		}
        else 
        {
            cp.out() << "No solution found."  << std::endl;
        }



    } catch(IloException& e)
    {
        env.out() << " ERROR: " << e << std::endl;
    }

    env.end();

    // PROHIBIT OTHER PERIODS 

    // GLOBAL CONSTRAINTS

    return rval;
}

int test_rcpsp_solution(
    int njobs,
    int *duration,        
    int *release_date,    
    int *latest_start_date, 
	int *mandatory,       
	int obj_type,         
    double discount_rate,
    double *profit,       
	double **tprofit,     
    int *nsucc,           
    int **succ,           
    int **lag,            
    int tmax,             
    int nresources,       
    int **lhs,            
    int *rhs,             
	int *in_sched)
{

	int j, r, s, t;
	int **rconsumption=NULL;
	int *sch = in_sched;
	int endt;
	int cnt=0;

	rconsumption = new int*[nresources];
	for(r=0; r < nresources; r++)
	{
		rconsumption[r] = new int[tmax+1];
		for(t=0; t < (tmax+1); t++)
			rconsumption[r][t] = 0;
	}

	for(j=0; j < njobs; j++)
	{

		if(sch[j] == -1)
		{
			if(mandatory && mandatory[j])
			{
				fprintf(stderr, "job %d is mandatory, but not scheduled.\n", j);
				cnt+=1;
			}

			if(obj_type == 1)
			{
				fprintf(stderr, "makespan problem, but job %d is not scheduled.\n", j);
				cnt+=1;
			}

			continue;
		}

		if(release_date && (sch[j] < release_date[j]) )
		{
			fprintf(stderr, "job %d with release_date %d scheduled in time %d.\n",
				j, release_date[j], sch[j]);
			cnt += 1;
		}

		if(latest_start_date && (sch[j] > latest_start_date[j]) )
		{
			fprintf(stderr, "job %d with latest date %d scheduled in time %d.\n",
				j, latest_start_date[j], sch[j]);
			cnt += 1;
		}

		if( sch[j] > tmax )
		{
			fprintf(stderr, "job %d with scheduled in time %d, yet tmax = %d.\n",
				j, sch[j], tmax);
			cnt += 1;
		}

		endt = sch[j] + (duration ? duration[j] : 1);
		if(endt > tmax)
			endt = tmax;
		for(r=0; r < nresources; r++)
			for(t=sch[j]; t < endt; t++)
				rconsumption[r][t] += lhs[j][r];
	}

	endt = (tmax+1);
	for(t=0; t < endt; t++)
		for(r=0; r < nresources; r++)
			if( rconsumption[r][t] > rhs[r] )
			{
				fprintf(stderr, "rconsumption[r=%d][t=%d] = %d > %d = rhs[r=%d].\n", 
					r,t, rconsumption[r][t], rhs[r], r);
				cnt += 1;
			}

	for(j = 0; j < njobs; j++)
		for(s=0; s < nsucc[j]; s++)
		{
			if(sch[succ[j][s]] == -1)
				continue;
			if( sch[succ[j][s]] < (sch[j] + lag[j][s]) )
			{
				fprintf(stderr, "bad prec: %d << %d with lag %d, yet sch[%d] = %d and sch[%d] = %d.\n",
					j, succ[j][s], lag[j][s], j, sch[j], succ[j][s], sch[succ[j][s]]);
				cnt += 1;
			}
		}

	return cnt;
}


#ifdef __cplusplus
}
#endif
