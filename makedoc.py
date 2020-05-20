#!/usr/bin/env python

import re
import sys

from contextlib import contextmanager

def start_section(line):
    sys.stdout.write("\n{}".format(line.lstrip()[3:]))

def emit_line(line):
    sys.stdout.write("{}".format(line.lstrip()[3:]))

def emit_line_nosub(line):
    sys.stdout.write(line)

def emit_empty():
    sys.stdout.write("\n")

## Global state to track code section beg/end
in_code_section = False

def process_line(line):
    global in_code_section 

    section_match = re.compile(r'^\s{0,}// #')
    line_match = re.compile(r'^\s{0,}// ')
    space_match = re.compile(r'^\s{0,}//$')
    code_section_match = re.compile(r'^\s{0,}// ~~~~')
    m = section_match.match(line)
    if m:
        start_section(line)
    elif code_section_match.match(line):
        if not in_code_section:
            emit_line_nosub('```c')
            emit_empty()
        else:
            emit_line_nosub('```')
            emit_empty()
        in_code_section = not in_code_section
    elif line_match.match(line):
        emit_line(line)
    elif space_match.match(line):
        emit_empty()
    elif in_code_section:
        emit_line_nosub(line)

            
def process(filename):
    with open(filename, 'r') as f:
        list(map(process_line, f.readlines()))
    sys.stdout.flush()

        
if __name__ == '__main__':
    process(sys.argv[1])
