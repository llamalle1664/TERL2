import os
import sys


files=os.listdir("speech")
i=0
print(files)
for file in files:
    print(file)
    print(os.path.isdir("speech/"+file))
    if os.path.isdir("speech/"+file): 
        files2=os.listdir("speech/"+file)
        j=0
        for file2 in files2:
            if file2.endswith(".wav"):
                print(file)
                print(file2)
                os.rename("speech/"+file+"/"+file2,"speech/"+str(i)+"_"+file+"_"+str(j)+".wav")
                j=j+1
        i=i+1

for file in os.listdir("speech"):
    if os.path.isdir("speech/"+file):
        os.system('rm -r speech/'+file)
        
