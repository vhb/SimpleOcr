#!/usr/bin/env python

import argparse as ap
import subprocess
import shlex, subprocess
import json
import xml.etree.ElementTree as ET

parser = ap.ArgumentParser(description='Ocr trainner')
parser.add_argument(
    '-dataset',
    #dest='dataset',
    type=str,
    help='Dataset to use'
)
parser.add_argument(
    '-jsons',
    dest='jsons',
    type=str,
    nargs='+',
    help='List of json to use'
)
parser.add_argument(
    '-outfile',
    dest='outfile',
    type=str,
    help='logfile path (!) Overide existing file'
)

args = parser.parse_args()

with open(args.outfile, 'wa+') as logfile:
    def get_layers(training_path):
        tree = ET.parse(training_path)
        root = tree.getroot()
        ocr = root.find('SimpleOcr')
        layers = ocr.find('layer_sizes')
        data = layers.find("data")
        return ET.tostring(data, method='text')

    def check_value(workflow, dataset_path, training, datas):
        with open(dataset_path, 'r') as r:
            dataset = sorted(json.load(r).items())

        total = float(0)
        errors = float(0)

        layers = get_layers(training).split()
        # fichier -> tant de layers, tel taux de success
        for key, values in dataset:
            for value in values:
                print value
                cmd = "./ocr {:s} {:s} {:s} {:s} | grep \'value:\' | sed \'s/value: //g\'"\
                    .format(value, workflow, dataset_path, training)
            #cmd = shlex.split(cmd)

                result = subprocess.check_output(['sh', '-c', cmd],
                                                 stderr=subprocess.PIPE)[:-1]
                print key + ", " + result
                total += 1
                if key != result:
                    errors += 1

        success = (total - errors) / (total) * 100
        logfile.write('Workflow: {:s} dataset: {:s} training: {:s} training_iteration: {:s} success {:f} layers: {:s}\n'.\
                      format(workflow, dataset_path, training, datas['iterations'], success, str(layers)))
        print success


    def parse_output(output):
        l = output.split('\n')
        value = {}
        for i in l:
            if i.startswith('iterations'):
                value['iterations'] = i.split()[1]
            elif i.startswith('training_file'):
                value['training_path'] = i.split()[1]
            #elif i.startswith('value'):
                #value['value'] = i.split()[1]
        return value


    for i in args.jsons:
        try:
            cmd = './ocr {:s} {:s} {:s}'.format('./data/step1/base/a.bmp', i, args.dataset)
            print cmd
            cmd = shlex.split(cmd)
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            out, err = p.communicate()
            print out, err
            output = parse_output(out)
            check_value(i, args.dataset, output['training_path'], output)
        except Exception as e:
            print e


    #def main():
    #tests = get_test_datas()

