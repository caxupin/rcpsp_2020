#include <cstdlib>
#include <iostream>

#include "rcpsp.hpp"

int main(int argc, const char* argv[])
{
    try {
        const char* filename = "data/rcpsp_default.data";
        int failLimit = 10000;
        if (argc > 1)
            filename = argv[1];
        if (argc > 2)
            failLimit = atoi(argv[2]);
        std::ifstream file(filename);
        if (!file){
			std::cout << "usage: " << argv[0] << " <file> <failLimit>" << std::endl;
            throw 1;
        }

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

        std::cout << "Filename = '"<< filename << "' and failLimit = " << failLimit << std::endl; 

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

        out_sched = new int[njobs];

        std::cout << "Loaded instance with njobs = "<< njobs << ", and nresources = " << nresources << "." << std::endl; 

		int FailLimit = -1;
		double ExtractionTimeLimit = 100000.0;
		double CpTimeLimit = 600.0;
		double GapLimit = 0.0;
		int NumWorkers = 8;
		double out_val, out_ubound, out_gap;

        rcpsp(njobs,
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

    } catch(int e){
		std::cout << " ERROR: " << e << std::endl;
    }
    return 0;
}

#if 0
int original_main(int argc, const char* argv[]){
    IloEnv env;
    try {
        const char* filename = "data/rcpsp_default.data";
        IloInt failLimit = 10000;
        if (argc > 1)
            filename = argv[1];
        if (argc > 2)
            failLimit = atoi(argv[2]);
        std::ifstream file(filename);
        if (!file){
            env.out() << "usage: " << argv[0] << " <file> <failLimit>" << std::endl;
            throw FileError();
        }

        IloModel model(env);
        IloInt nbTasks, nbResources, i, j;
        file >> nbTasks;
        file >> nbResources;
        IloCumulFunctionExprArray resources(env, nbResources);
        IloIntArray capacities(env, nbResources);
        for (j=0; j<nbResources; j++) {
            IloInt c;
            file >> c;
            capacities[j] = c;
            resources[j] = IloCumulFunctionExpr(env);
        }
        IloIntervalVarArray tasks(env, nbTasks);
        for (i=0; i<nbTasks; i++) {
            tasks[i] = IloIntervalVar(env);
        }
        IloIntExprArray ends(env);
        for (i=0; i<nbTasks; i++) {
            IloIntervalVar task = tasks[i];
            IloInt d, nbSucc;
            file >> d;
            task.setSizeMin(d);
            task.setSizeMax(d);
            ends.add(IloEndOf(task));
            for (j = 0; j < nbResources; j++) {
                IloInt q;
                file >> q;
                if (q > 0) {
                    resources[j] += IloPulse(task, q);
                }
            }
            file >> nbSucc;
            for (IloInt s=0; s<nbSucc; ++s) {
                IloInt succ;
                file >> succ;
                model.add(IloEndBeforeStart(env, task, tasks[succ-1]));
            }
        }

        for (j = 0; j < nbResources; j++) {
            model.add(resources[j] <= capacities[j]);
        }

        IloObjective objective = IloMinimize(env,IloMax(ends));
        model.add(objective);

        IloCP cp(model);
        cp.setParameter(IloCP::FailLimit, failLimit);
        cp.out() << "Instance \t: " << filename << std::endl;
        if (cp.solve()) {
            cp.out() << "Makespan \t: " << cp.getObjValue() << std::endl;
        } else {
            cp.out() << "No solution found."  << std::endl;
        }
    } catch(IloException& e){
        env.out() << " ERROR: " << e << std::endl;
    }
    env.end();
    return 0;
}
#endif
