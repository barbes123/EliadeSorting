import os
print("runnbr=")
runnbr=int(input())
print("version1=")
n=int(input())
print("version2=")
m=int(input())
d=[]
d.append("sorted_run_"+str(runnbr)+".root") #sorted_run_14.root
for i in range (n, m+1, 1):
    d.append("sorted_run_"+str(runnbr)+"_"+str(i)+".root")
e=str(d[0])
for i in range (1, m-n+2, 1):
	e=str(e)+" "+str(d[i])
os.system("hadd " +e+" ")
print("hadd " +e+" ")
