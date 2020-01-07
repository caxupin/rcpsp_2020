import pandas as pd

#Se cambia el formato de los archivos con datos .rcpsp usados para el modelo con CPLEX 

'''
SOBRE ARCHIVO DE ENTRADA:

Se usa como base los datos en dormato .rcpsp pero separados solo por un espacio cada cadto (para obtener eso se puede 
usar un editor de texto y reemplazar los espaciados mayores por un espaciado simple " ".)
'''

# Input
data_file = "AB4c1.txt"

# Delimiter
data_file_delimiter = ' '
import numpy as np
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
#generamos cuanto usa de cada recurso
rec = np.zeros((m,n))

for i in range(0,m):
	for j in range(3,n+3):
		rec[i,j-3] = int(df[7+i][j])
		#print(a[i,j])

#La duracion de cada job

dur = np.zeros(n)
for i in range(3,n+3):
	dur[i-3] = int(df[1][i])

#Sacamos el profit

pro = np.zeros(n)
for i in range(3,n+3):
	pro[i-3] = float(df[6][i])


print(pro)
print("cantidad de profifts: ",len(pro))

#Sacamos cantidad de precedencias de cada trabajo

cpre = np.zeros(n)
for i in range(3,n+3):
	cpre[i-3] = int(df[7+m][i])

#Calculamos la cantidad total de precedencias

l = cpre.sum()
l = int(l)	

#print(l)

#Obtenemos el tiempo maximo

t = int(df[3][0])

#Calculamos tasa de descuento

delta = float(df[2][0])

#Vector de tasas de descuento

DT = []
for i in range(0,t+1):
	DT.append(np.exp(-i*delta))

#Obtenemos los precedentes

pre = []
for i in range(3,n+3):
	for k in range(0,int(cpre[i-3])):
		predecesor = int(df[7+m+2*k][i])
		pre.append([predecesor+1,i+1])


#Adaptamos los vectores al formato de salida

#Cantidad de recursos
Lout = "["
for i in range(0,len(L)):
	Lout += str(L[i])
	if i!=(len(L)-1):
		Lout+=","
	else:
		Lout+="]"
#print(Lout)

#Profit
PROout = "["
for i in range(0,len(pro)):
	PROout += str(pro[i])
	if i!=(len(pro)-1):
		PROout+=","
	else:
		PROout+="]"
#print(PROout)

#Delta t 

DTout = "["
for i in range(0,len(DT)):
	DTout += str(DT[i])
	if i!=(len(DT)-1):
		DTout+=","
	else:
		DTout+="]"
#print(DTout)

#predecesores 

PREout = "[|"
for i in range(0,len(pre)):
	np = pre[i]
	PREout += str(np[0])+","+str(int(np[1]))
	if i!=(len(pre)-1):
		PREout+="|"
	else:
		PREout+="|]"
#print(PREout)

#duracion

DURout = "["
for i in range(0,len(dur)):
	np = dur[i]
	DURout += str(int(np))
	if i==(len(dur)-1):
		DURout+="]"
	else:
		DURout+= ","
	
print(DURout)

#Cantidad de recursos que usa cada trabajo

RECout = "[|"
for i in range(0,len(rec[:,0])):
	for j in range(0,len(rec[0,:])):
		np = int(rec[i,j])
		RECout += str(np)
		if j != (len(rec[0,:])-1):
			RECout+=","
	if i!=(len(rec[:,0])-1):
		RECout+="|"
	else:
		RECout+="|]"
#print(RECout)

W = "["
Wp=[]
for j in range(0,n):
	W+="|"
	for q in range(0,len(DT)):
		aux = float(DT[q])*int(pro[j])
		W += str(aux)
		Wp.append(aux)
		if q <(len(DT)-1):
			W+=","

W += "|]"

print(len(Wp))


print("pre: ",len(pre))
print("duraciones ",len(dur))
print("cprec ", len(cpre))
print("profits ", len(pro))
#Ya que tenemos los datos los adaptamos al modelo de dato de MinZinc

tf = open("Input.dzn","w")
tf.write("n = " + str(n)+";"+"\n")
tf.write("m = " + str(m)+";"+"\n")
tf.write("L =" + Lout+";"+"\n")
tf.write("l =" + str(l)+";"+"\n")
tf.write("prof =" +PROout+";"+"\n")
tf.write("d =" +DURout+";"+"\n")
tf.write("res =" +RECout+";"+"\n")
tf.write("maxt =" + str(t)+";"+"\n")
tf.write("dr = " + str(delta)+";"+"\n")
tf.write("pre =" + PREout+";"+"\n")
tf.write("W ="+W+";"+"\n")





#Calculamos la cantidad de precedencias
#print(df[9][3:].to_numeric().sum())
#prec = df[9][:3]

