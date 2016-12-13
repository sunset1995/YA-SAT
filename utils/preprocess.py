import os
import sys

for dirname, dirnames, filenames in os.walk(sys.argv[1]):
    for filename in filenames:
        if filename.find('.cnf') == -1:
            continue
        os.system("sed '$d' %s | sed '$d' | sed '$d' > .preprocess_tmp" % os.path.join(dirname, filename))
        os.system("mv .preprocess_tmp %s" % os.path.join(dirname, filename))
