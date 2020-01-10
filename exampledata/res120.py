import pandas as pd

#Se cambia el formato de los archivos con datos .rcpsp usados para el modelo con CPLEX 

'''
SOBRE ARCHIVO DE ENTRADA:

Se usa como base los datos en dormato .rcpsp pero separados solo por un espacio cada cadto (para obtener eso se puede 
usar un editor de texto y reemplazar los espaciados mayores por un espaciado simple " ".)
'''

# Input
data_file = "res120.txt"

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

n = len(df[:][0])

#Numero de recursos

m = 4

#Cantidad limite de recursos

L = [15, 18, 18 ,16]

print("m=",m)
#generamos cuanto usa de cada recurso
rec = np.zeros((m,n))

for i in range(0,m):
	for j in range(0,n):
		rec[i,j] = int(df[3+i][j])
		#print(a[i,j])

dur = np.zeros(n)
for i in range(0,n):
	dur[i] = int(df[2][i])


cpre = np.zeros(n)
for i in range(0,n):
	cpre[i] = int(df[2][i])

#Calculamos la cantidad total de precedencias

l = cpre.sum()
l = int(l)	

print("l = ",l)

#Obtenemos el tiempo maximo

t = 85

#Calculamos tasa de descuento

delta = float(df[2][0])



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

#duracion

DURout = "["
for i in range(0,len(dur)):
	np = dur[i]
	DURout += str(int(np))
	if i==(len(dur)-1):
		DURout+="]"
	else:
		DURout+=","
	
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


tf = open("res.dzn","w")
tf.write("n = " + str(n)+";"+"\n")
tf.write("m = " + str(m)+";"+"\n")
tf.write("L =" + Lout+";"+"\n")
tf.write("d =" +DURout+";"+"\n")
tf.write("res =" +RECout+";"+"\n")





#Calculamos la cantidad de precedencias
#print(df[9][3:].to_numeric().sum())
#prec = df[9][:3]

