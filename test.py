#!/usr/bin/env python

import os
import sys
import json
import subprocess

argc = len(sys.argv)
if argc < 4:
	print 'usage: ./tesy.py workflow dataset training_datas'
	exit()

workflow_path = str(sys.argv[1])
dataset_path = str(sys.argv[2])
training_path = str(sys.argv[3])
training_path = training_path.replace(' ', '\\ ')

workflow_file = open(dataset_path)
workflow = json.load(workflow_file)

total = float(len(workflow))
errors = float(0)

workflow = sorted(workflow.items())

for key, value in workflow:
    cmd = './ocr ' + value + ' ' + workflow_path + ' ' + dataset_path + ' ' + training_path + ' | grep \'value:\' | sed \'s/value: //g\''
    print cmd
    result = subprocess.check_output(['sh', '-c', cmd])[:-1]
    print key + ", " + result
    print '[', key, ']'
    print '{', result, '}'
    if key != result:
        errors += 1

workflow_file.close()

print (total - errors) / (total) * 100
