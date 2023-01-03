import sys
import json
import requests

if __name__ == '__main__':
    import argparse
    argparser = argparse.ArgumentParser(allow_abbrev=False)
    argparser.add_argument('-i', '--indent', action="store_true", required=False,
                           help='väljundisse taanetega json')
    args = argparser.parse_args()
