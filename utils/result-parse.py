import sys
from natsort import natsort_keygen, ns

categories = {}

def parse(results):
    global categories
    nowScale = ''
    for line in results:
        if line == '':
            nowScale = ''
            continue
        if nowScale=='':
            nowScale = line.strip('-')
            continue
        now = line.split(':')
        if len(now)!=2:
            continue
        nowCategory = now[0].strip()
        nowData = now[1].replace(' ', '')
        if nowCategory not in categories:
            categories[nowCategory] = []
        categories[nowCategory].append((nowScale, nowData))

# Parse all result
for resultFile in sys.argv[1:]:
    with open(resultFile) as f:
        resultSAT = f.read().split('\n')
    parse(resultSAT[2:-2])

# Print result
width1 = 0
width2 = 0

for catego, datas in categories.items():
    for data in datas:
        width1 = max(width1, len(data[0]))
        width2 = max(width2, len(data[1]))

for catego, datas in categories.items():
    print('### %s' % catego)
    datas.sort(key=lambda x: natsort_keygen(alg=ns.IGNORECASE)(x))
    for data in datas:
        print('| %s | %s |' % (data[0].ljust(width1, ' '), data[1].rjust(width2, ' ')))
    print('')
