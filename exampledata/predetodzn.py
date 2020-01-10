import pandas as pd

#Se cambia el formato de los archivos con datos .rcpsp usados para el modelo con CPLEX 

'''
SOBRE ARCHIVO DE ENTRADA:

Se usa como base los datos en dormato .rcpsp pero separados solo por un espacio cada cadto (para obtener eso se puede 
usar un editor de texto y reemplazar los espaciados mayores por un espaciado simple " ".)
'''

# Input
data_file = "prede120.txt"

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
print(n)




#Sacamos el profit

pronr= np.random.uniform(0,10000,[n])
pro = np.zeros(n)
for i in range(0,n):
	pro[i] = round(pronr[i],1)

print(pro)

print("cantidad de profifts: ",len(pro))

#Sacamos cantidad de precedencias de cada trabajo

cpre = np.zeros(n)
for i in range(0,n):
	cpre[i] = int(df[2][i])

#Calculamos la cantidad total de precedencias

l = cpre.sum()
l = int(l)	

print("l = ",l)

#Calculamos tasa de descuento

delta = 0.15

#Vector de tasas de descuento
t = 680
DT = []
for i in range(0,t+1):
	DT.append(np.exp(-i*delta))

#Obtenemos los precedentes

pre = []
for i in range(1,n+1):
	for k in range(0,int(cpre[i-1])):
		predecesor = int(df[3+k][i-1])
		pre.append([i,predecesor])

print(len(pre))
#Adaptamos los vectores al formato de salida

#Cantidad de recursos

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


#Cantidad de recursos que usa cada trabajo


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


#Ya que tenemos los datos los adaptamos al modelo de dato de MinZinc

tf = open("Inputprede.dzn","w")
tf.write("dr = 0.12;\n")
tf.write("maxt = 680\n")
tf.write("n = " + str(n)+";"+"\n")
tf.write("prof =" +PROout+";"+"\n")
tf.write("pre =" + PREout+";"+"\n")
tf.write("W ="+W+";"+"\n")





#Calculamos la cantidad de precedencias
#print(df[9][3:].to_numeric().sum())
#prec = df[9][:3]

