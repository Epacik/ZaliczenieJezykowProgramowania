import os

import time

scrDir = os.getcwd()
p = os.path.join(scrDir, r"../Debug/")
os.chdir(p)

test = ""
for i in range(3, 1000):
    print("krok " + str(i - 2) + " z 997")
    for j in range(30):
        
        os.system("zaliczeniejezykowprogramowania.exe " + str(i))
        f = open("dane.txt", "r")
        expr = f.read()
        f.close()
        res = ""
        t = ""
        try:
            res = str(eval(expr))
            t = "passed " + expr + "\n"
        except:
            res = ""
            t = "FAILED " + expr + "\n"
            print(t)
        test += t;
        

os.chdir(scrDir)

ts = test.split('\n')

passed = 0
failed = 0

for l in ts:
    if(l.startswith("passed")): 
        passed += 1
    else: 
        failed += 1;

ratePassed = (100 * passed)/(passed + failed)
rateFailed = (100 * failed)/(passed + failed)
print(f"passed: {str(passed)} ({str(ratePassed)}),  failed: {str(failed)} ({str(rateFailed)})")

test += f"\n\npassed: {str(passed)} ({str(ratePassed)}),  failed: {str(failed)} ({str(rateFailed)})"

f = open("exprTestResult.txt", "w")
f.write(test)
f.close()


