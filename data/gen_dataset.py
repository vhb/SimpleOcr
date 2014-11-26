import sys
import subprocess
import shlex

font = sys.argv[0]
pointsize = 72

char = {
    'a': 'a_small'
}



for letter, file in char:
    cmd = '-font {:s} -pointsize {:d} label:{:s} {:s}.jpg'.format(
        font,
        pointsize,
        letter,
        file
    )
    cmd = shlex.split(cmd)
    subprocess.call(cmd)
