import sys, subprocess

for i in sys.argv[1:][::-1]:
    tmp = i
    i = i[0]
    if i.islower():
        dest = i[0] + "_small" + ".bmp"
    elif i.isdigit():
        dest = "num_" + i + ".bmp"
    else:
        dest= i.lower() + '.bmp'
    subprocess.call(['mv', tmp, dest])
