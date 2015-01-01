

f=open("lin2012xyz2e_5_7sf.csv")
data=f.readlines()
f.close()

cie={}

for line in data:
	tmp=line.split(",")
	
	nm=int(tmp[0])
	x=float(tmp[1])
	y=float(tmp[2])
	z=float(tmp[3])
	
	print("{0}: {1} {2} {3}".format(nm,x,y,z))
	
	cie[nm]=(x,y,z)
	
	
aux_x=""
aux_y=""
aux_z=""
yint=0.0
for n in range(390,710,10):
	x,y,z=cie[n]
	
	aux_x=aux_x+str(x)+"f,"
	aux_y=aux_y+str(y)+"f,"
	aux_z=aux_z+str(z)+"f,"
	
	yint+=y

print("X:")
print(aux_x)
print("Y:")
print(aux_y)
print("Z:")
print(aux_z)
print("yint:")
print(yint)


