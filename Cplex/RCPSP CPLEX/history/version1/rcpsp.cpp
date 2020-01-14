// -------------------------------------------------------------- -*- C++ -*-
// File: ./examples/src/cpp/sched_rcpsp.cpp
// --------------------------------------------------------------------------
// Licensed Materials - Property of IBM
//
// 5724-Y48 5724-Y49 5724-Y54 5724-Y55 5725-A06 5725-A29
// Copyright IBM Corporation 1990, 2014. All Rights Reserved.
//
// Note to U.S. Government Users Restricted Rights:
// Use, duplication or disclosure restricted by GSA ADP Schedule
// Contract with IBM Corp.
// --------------------------------------------------------------------------

/* ------------------------------------------------------------

   Problem Description
   -------------------

   The RCPSP (Resource-Constrained Project Scheduling Problem) is a
   generalization of the production-specific Job-Shop (see
   sched_jobshop.cpp), Flow-Shop (see sched_flowshop.cpp) and Open-Shop
   (see sched_openshop.cpp) scheduling problems. Given:

   - a set of q resources with given capacities,
   - a network of precedence constraints between the activities, and
   - for each activity and each resource the amount of the resource
   required by the activity over its execution,

   the goal of the RCPSP is to find a schedule meeting all the
   constraints whose makespan (i.e., the time at which all activities are
   finished) is minimal.

   ------------------------------------------------------------ */


#if 0
#include <ilcp/cp.h>

class FileError: public IloException {
public:
    FileError() : IloException("Cannot open data file") {}
};
#endif

#include "rcpsp.hpp"

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
	int numWorkers)
{
    IloEnv env;

    try {

        IloModel model(env);
        IloInt nbTasks, nbResources, i, j, r;

        nbTasks = njobs;

        // DEFINE VARIABLES

        IloIntervalVarArray task_variables(env, nbTasks);

        for (j=0; j<nbTasks; j++) 
        {
            task_variables[j] = IloIntervalVar(env);
            task_variables[j].setSizeMin(duration[j]);
            task_variables[j].setSizeMax(duration[j]);
			if(obj_type != 1) 
				task_variables[j].setOptional();
        }

        // DEFINE PRECEDENCE CONSTRAINTS

        for (j=0; j<nbTasks; j++)
        {
            IloInt nbSucc = nsucc[j];
            for (IloInt s=0; s<nbSucc; ++s) 
			{
                model.add(IloStartBeforeStart(env, task_variables[j], task_variables[succ[j][s]], lag[j][s])); // StartBeforeStart
				model.add(IloPresenceOf(env,task_variables[succ[j][s]]) <= IloPresenceOf(env,task_variables[j]));
            }
        }

        // DEFINE RELEASE DATES AND DUE DATES
		if(release_date)
		{
            for (j=0; j<nbTasks; j++)
				if(release_date[j] > 0)
					model.add(IloStartOf(task_variables[j]) >= release_date[j]);
		}
		if(latest_end_date)
		{
            for (j=0; j<nbTasks; j++)
				if(latest_end_date[j] > (-1))
					model.add(IloEndOf(task_variables[j]) <= latest_end_date[j]);
		}

		// SET MANDATORY JOBS
		if(mandatory)
		{
            for (j=0; j<nbTasks; j++)
				if(mandatory[j])
					task_variables[j].setPresent();
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
			IloIntExprArray ends(env, nbTasks); // an expression whose result is an int. ex: 3*x[4] - 10*x[5]^2, where x are variables

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

		// PROVIDE SOLVER WITH INPUT SOLUTIOn
		if(in_sched)
		{
			IloSolution hotStart(env);
			for (j=0; j<nbTasks; j++)
			{
				if(in_sched[j] > (-1))
				{
					hotStart.setStart(task_variables[j], in_sched[j]);
					hotStart.setEnd(task_variables[j], in_sched[j] + duration[j]);
				}
			}
			cp.setStartingPoint(hotStart);
		}

        // SOLVE MODEL

		// four is the highest display level
		cp.setParameter(IloCP::WarningLevel, 2);
		// if I want this thing to shut up (options: Quiet, Normal)
		//cp.setParameter(IloCP::LockVerbosity, IloCP::ParameterValues::Quiet);
		cp.setParameter(IloCP::LogVerbosity, IloCP::ParameterValues::Normal);
		//cp.setParameter(IloCP::LockPeriod, 50000); //how often it prints to screen

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
		while(cp.next() && (*out_gap >= GapLimit))
		{
			*out_ubound = cp.getObjBound();
			*out_solval = cp.getObjValue();
			*out_gap = cp.getObjGap();
		}
		cp.endSearch();

		*out_ubound = cp.getObjBound();
		*out_solval = cp.getObjValue();
		*out_gap = cp.getObjGap();

		if( cp.getInfo(IloCP::IntInfo::NumberOfSolutions) )
		{
        	cp.out() << "Objval: " << *out_solval << std::endl;
        	cp.out() << "Upper Bound: " << *out_ubound << std::endl;
        	cp.out() << "Gap: " << *out_gap << std::endl;
        	for (j=0; j<nbTasks; j++)
            	out_sched[j] = cp.getStart(task_variables[j]);	
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

    return 0;
}
