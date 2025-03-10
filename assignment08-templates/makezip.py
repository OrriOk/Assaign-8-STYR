#!/usr/bin/env python3

ASSIGNMENT=8

import os
import re
import sys
import getpass
import subprocess

def get_group_info():
    # Call external program in skel to automatically determine group info
    #try:
    #    result = subprocess.check_output(["/home/sty24/bin/getgroupinfo"], universal_newlines=True)
    #except Exception as e:
    #    print("Error getting group info (must be run on skel):\n", e)

    # Check if the file exists
    try:
        with open("groupinfo.txt", 'r') as file:
            lines = file.readlines()
            print("\nUsing groupinfo.txt:")
            print("".join(lines))
            if len(lines) >= 2:
                # Extract last word from the second line
                match_second_line = re.search(r'\(([^)]+)\)', lines[1])
                last_word_second_line = match_second_line.group(1) if match_second_line else getpass.getuser()

                # Extract last word from the third line
                if len(lines) >= 3:
                    match_third_line = re.search(r'\(([^)]+)\)', lines[2])
                    last_word_third_line = "-"+match_third_line.group(1) if match_third_line else ""
                else:
                    last_word_third_line = ""

                return last_word_second_line+last_word_third_line
            else:
                return getpass.getuser()
    except FileNotFoundError:
        print("\ngroupinfo.txt not found")
        #print("Run on skel (for automatic creation) or create manually.")
        print("Create that file with three lines:\nGroup: [your group number]\nFull Name 1 (userid1)\nFull Name 2 (userid2)\n\nWithout group (individual submission):\nFirst line 'No group', second line your name (userid)\n")
        sys.exit(1)



def getsubfile(dir):
    try: 
       fn = subprocess.check_output(['make', '-f', dir+"/Makefile", "showsrc"]).decode('utf-8').strip()
       return dir+"/"+fn;
    except:
       return None

files = []
s1 = getsubfile("p1")
if s1: files.append(s1)
s2 = getsubfile("p2")
if s2: files.append(s2)

usernames = get_group_info()

print("Selected files for inclusion: ", files)

filename = "STY25-P" + str(ASSIGNMENT) + "-" + usernames + ".zip"

print("\nCreating file", os.path.abspath(filename))
cmd = ['zip', '-v', '-j', filename]
cmd += files
cmd += [ 'groupinfo.txt' ]
subprocess.call(cmd)
