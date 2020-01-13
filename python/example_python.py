from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from ortools.sat.python import cp_model
import pandas as pd
import numpy as np
import sys
from ortools.sat.python import visualization

# This a RCPSPDc Model 
# First we must read data:
def read_input(data_file):
	# Most of this code was stolen from @benjajf
	# Delimiter
	data_file_delimiter = ' '
	
	# The max column count a line in the file could have
	largest_column_count = 0

	# Loop the data lines
	with open(data_file, 'r') as temp_f:
		# Read the lines
    	
		lines = temp_f.readlines()

    	for l in lines:
			# Count the column count for the current line
        	column_count = len(l.split(data_file_delimiter)) + 1

			# Set the new most column count
        	largest_column_count = column_count if largest_column_count < column_count else largest_column_count

	# Close file
	temp_f.close()

	# Generate column names (will be 0, 1, 2, ..., largest_column_count - 1)
	column_names = [i for i in range(0, largest_column_count)]
	# Read csv
	df = pd.read_csv(data_file, header=None, delimiter=data_file_delimiter, names=column_names)
	print(df)

	df.fillna('-',inplace=True)

	#cantidad de trabajos

	n = int(df[0][0])

	#Numero de recursos

	m = int(df[1][0])

	#Cantidad limite de recursos

	L = []

	i = 0
	while i >=0:
		if df[i][1]=="-":
			break
		else:
			L.append(int(df[i][1]))
			i+=1
		print("m=",m)

	#Obtenemos el tiempo maximo

	tmax = int(df[3][0])

	#Calculamos tasa de descuento

	delta = float(df[2][0])

	#Vector de tasas de descuento

	DT = []
	for i in range(0,tmax+1):
		DT.append(np.exp(-i*delta))

	

	#La duracion de cada job

	dur = np.zeros(n)
	for i in range(3,n+3):
		dur[i-3] = int(df[1][i])

	#Cuando se libera el trabajo
	jrelease = []
	for i in range(3, n+3):
		jrelease.append(int(df[2][i]))

	#Deadline del trabajo
	jdeadline = []
	for i in range(3, n+3):
		jdeadline.append(int(df[3][i]))

	#si una actividad es obligatoria o no
	mandatory = []
	for i in range(3, n+3):
		mandatory.append(int(df[4][i]))

	#hotpoint
	hotpoint = []
	for i in range(3, n+3):
		hotpoint.append(int(df[5][i]))

	#Sacamos el profit

	pro = np.zeros(n)
	for i in range(3,n+3):
		pro[i-3] = float(df[6][i])

	#generamos cuanto usa de cada recurso
	rec = np.zeros((m,n))

	for i in range(0,m):
		for j in range(3,n+3):
			rec[i,j-3] = int(df[7+i][j])
			#print(a[i,j])

	#Sacamos cantidad de precedencias de cada trabajo

	cpre = np.zeros(n)
	for i in range(3,n+3):
		cpre[i-3] = int(df[7+m][i])

	#Calculamos la cantidad total de precedencias

	l = cpre.sum()
	l = int(l)	

	#Obtenemos los precedentes

	pre = []
	for i in range(3,n+3):
		for k in range(0,int(cpre[i-3])):
			predecesor = int(df[8+m+2*k][i])
			lag = int(df[8+m+2*k+1][i])
			if(predecesor<n):				#editao---------------------------
				pre.append([i-2, predecesor+1,lag])


	print(pro)
	print("cantidad de profifts: ",len(pro))





	return n, m, L, l, tmax, delta, DT, dur, jrelease, jdeadline, mandatory, hotpoint, pro, rec, cpre, pre; 

n, m, L, l, tmax, delta, DT, dur, jrelease, jdeadline, mandatory, hotpoint, pro, rec, cpre, pre = read_input("AB4c1.txt")

data = [n, m, L, l, tmax, delta, DT, dur, jrelease, jdeadline, mandatory, hotpoint, pro, rec, cpre, pre]
print(len(pre), l)

# Now let's define the model and solve it
def rcpsp_solver():
	# Create the model
	model=cp_model.CpModel()
	# Define some parameters


	# Create the variables 
	intervals = []
	starts=[]
	ends=[]
	present=[]
	for j in range(n):
		duration = int(dur[j])
		is_pre = model.NewBoolVar('is_pre_%i' %j)
		start=model.NewIntVar(0, tmax+1, 's_%i' % j)
		end=model.NewIntVar(0, tmax+1, 'e_%i' % j)
		interval = model.NewOptionalIntervalVar(
                    start, duration, end, is_pre,
                    'interval_%i' % j) 
		intervals.append(interval)
		starts.append(start)
		ends.append(end)
		present.append(is_pre)

	# Constraints
	 #for each arc (i,j) i.e. a precedence relation, activity j
	for before, after, lag in pre:
		print('job %i is after job %i with lag %i' % (after, before, lag))
		model.Add(ends[before-1] <= starts[after-1])

	#turn rec into int values:
	
	# cumulative constraint
	rec_int=rec.astype(np.int64)
	for k in range(m):
		model.AddCumulative(intervals, rec_int[k,:],int(L[k]))

	# Define objective

	model.Maximize(sum(ends[j]*int(round(pro[j])) for j in range(n)))
	#model.Maximize(int(round(sum(np.exp(-delta*s[j])*pro[j] for j in range(n))*1000)))
	#model.Maximize(sum(-9*s[j]+np.log(pro[j])*100 for j in range(n)))

	# Creates a solver and solves the model.
	solver = cp_model.CpSolver()
	status = solver.Solve(model)

	
	if status == cp_model.OPTIMAL:
		for j in range(n):
			print(j)
			print(solver.Value(starts[j]))
			print(solver.Value(ends[j]))
			print(solver.Value(present[j]))
			print(pro[j])
		print('Maximum of objective function: %i' % solver.ObjectiveValue())
		
	return intervals



rcpsp_solver()
