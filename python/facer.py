# coding: utf-8

import cognitive_face as CF
import sys
import argparse
import json
import os

def command_detect(args, CF):
    result = CF.face.detect(args.image)
    if args.debug:
        print(json.dumps(result))
    else:
        print(result)

if __name__ == '__main__':
    #set up FaceAPI SDK
    KEY = os.getenv("FACEAPIKEY")
    BASE_URL = os.getenv("FACEAPIURL")
    if KEY == None or BASE_URL == None:
        print("error: The environment variable is not set.\n please set FACEAPIKEY or FACEAPIURL")
        sys.exit(1)
    CF.Key.set(KEY)
    CF.BaseUrl.set(BASE_URL)
    
    # create a parser
    parser = argparse.ArgumentParser(description="this is face certification cli.")
    subparsers = parser.add_subparsers()

    # create a parser of face_group command 
    face_detect = subparsers.add_parser('detect', help='see `detect -h`')
    face_detect.add_argument('image', help = 'Please set a picture with a face.')
    face_detect.add_argument('-d', '--debug', \
        action='store_true', \
        default=False, \
        help='debug mode if this flag is set print json. (default: False)')
    face_detect.set_defaults(handler=command_detect)

    args = parser.parse_args()
    if hasattr(args, 'handler'):
        args.handler(args, CF)
    else:
        # when unknown sub command
        parser.print_help()
    
