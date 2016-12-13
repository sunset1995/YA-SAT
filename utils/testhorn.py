import re
import os
import sys
import subprocess

if len(sys.argv) != 2:
    print('python3 test.py testbenchDir')
    exit(0)

horn = 0
error = 0
total = 0

for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if filename.find('.cnf') == -1:
            continue
        proc = subprocess.Popen(['./hornSatChecker', os.path.join(dirname, filename)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,)
        proc.wait()
        result = str(proc.stdout.read())
        total = total + 1
        if proc.returncode != 0:
            error = error + 1
            continue
        elif result.find('Horn') != -1:
            horn = horn + 1
            print(os.path.join(dirname, filename), result)

if error != 0:
    print("error %d" % error)
print("%d/%d" % (horn, total))
