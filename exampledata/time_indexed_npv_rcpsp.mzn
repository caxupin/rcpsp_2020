int: n; % number of tasks
int: maxt; % maximum time
set of int: TASK = 1..n;
set of int: TASK2 = 0..n-1;
array[TASK] of int: d; % duration

int: m; % number of resources
set of int: RESOURCE = 1..m;
array[RESOURCE] of int: L; % resource limit
array[RESOURCE,TASK] of int: res; % usage
set of int: TIME = 0..maxt;
set of int: TIME1 = 1..maxt+1; % q wea xd
array[TASK,TIME1] of float: W;

int: l; % number of precedences
set of int: PREC = 1..l;
array[PREC,1..2] of TASK: pre; % predecessor/successor pairs



%---------Variation to NPV maximization-----------------------

array[TASK] of float: prof; %PROFIT
float: dr; %DISCOUNT RATE
array[TASK, TIME] of var bool: x; %binary

%--------------------------------------------------------------

% Each activity has exactly one start time
constraint forall(j in TASK) (sum (t in TIME) (x[j, t])==1);  


% Activity j cannot start before t+p_j if activity i stars at or after t for each (i,j) in PREC    
constraint forall(p in PREC) (
                    forall(t in TIME)
                            (sum(k in t..maxt) (x[pre[p,1],k]) + sum(k in 0..t+d[pre[p,2]]-1) (x[pre[p,2], k])<=1)); 
% All the simultaneous resource usage must be less or equal than the resource capacity
constraint forall(k in RESOURCE) (
                 forall(t in TIME) 
                       (sum(j in TASK)(res[j, k]*(sum(s in t-d[j]+1..t)(x[j,s])))<=RESOURCE[k])
                       ); 
                 
                 
solve maximize sum (j in TASK) (sum(t in TIME) (W[j,t]*x[j,t]));

output [show(x)];
