int: n; % number of tasks
int: maxt; % maximum time
set of int: TASK = 1..n;
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
array[TASK,TIME] of var bool: y;

%--------------------------------------------------------------


% Each activity has exactly one start time

constraint forall(j in TASK) (sum (t in TIME) (x[j, t])<=1); 
 
% Activity j cannot start before t+p_i if activity i starts at or after t for each (i,j) in PREC   
 
constraint forall(p in PREC) (
          sum(t in TIME)(t*(x[pre[p,2],t]-x[pre[p,1],t]))>=d[pre[p,1]]
);

%restriccion sobre limite de recursos

%constraint forall(k in RESOURCE)
%            forall(t in TIME)(
%            sum(j in TASK)(res[k,j]*(y[j,t]-y[j,t+d[j]]))<=L[k])
%            );
                                                       
%All the simultaneous resource usage must be less or equal than the resource capacity
constraint forall(k in RESOURCE) (
                 forall(t in TIME) 
                       (sum(j in TASK)(res[k,j]*(sum(s in max(t-d[j]+1,0)..t)(x[j,s])))  <=L[k])
                       ); 
                 
% MAXIMIZATION    
solve maximize sum (j in TASK) (sum(t in TIME) (W[j,t+1]*x[j,t]));

output ["job 1", "\n", show(x[1,TIME]), "\n"];
output ["job 2", "\n", show(x[2,TIME]), "\n"];

output ["job 3", "\n", show(x[3,TIME]), "\n"];

output ["job 4", "\n", show(x[4,TIME]), "\n"];

output ["job 5", "\n", show(x[5,TIME]), "\n"];

