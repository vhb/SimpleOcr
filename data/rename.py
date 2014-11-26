import sys, subprocess

for i in sys.argv[1:][::-1]:
    tmp = i
    i = i[0]
    if i.islower():
        dest = i[0] + "_small" + ".jpg"
    elif i.isdigit():
        dest = "num_" + i + ".jpg"
    else:
        dest= i.lower() + '.jpg'
    subprocess.call(['mv', tmp, dest])
