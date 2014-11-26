import subprocess
import shlex
import json


car_list = open('./data/car_list').read().split('\n')
value = {}
for i in car_list:
    value[i] = []
    cmd = shlex.split("find . -name {:s}.bmp".format(i))
    files = subprocess.check_output(cmd).split('\n')
    value[i] += [item for item in files if item]
print(json.dumps(value))
