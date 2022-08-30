#!/bin/env python3

import hfst
import re

def naide4():
    ifs = hfst.HfstInputStream('analyser-gt-desc.hfst')    # set up an input stream
    transducer = ifs.read()                         # read the first transducer
    tokens = ['allmaaraudteejaam', 'köisraudtee', 'vanamees', 'jalutan', 'karuga', 'lääbakas', 'kxxds']
    for t in tokens:
        mrfs = transducer.lookup(t)
        print(t)
        for mrf in mrfs:
            print('  ', re.sub(r'@[^@]*@', '', mrf[0]), mrf[1])


if __name__ == '__main__':
    naide4()
