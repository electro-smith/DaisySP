import os
import re
import glob

# global comment tracker
in_comment_section = False
prev_comment_section = False

# returns either an empty string, the unmodified line, or a replaced line
def process_line(line):
    global in_comment_section
    global prev_comment_section
    section_match = re.compile(r'^\s{0,}// #')
    line_match = re.compile(r'^\s{0,}// ')
    space_match = re.compile(r'^\s{0,}//$')
    code_section_match = re.compile(r'^\s{0,}// ~~~~')
    prev_comment_section = in_comment_section
    if section_match.match(line):
        return ''
    elif code_section_match.match(line):
        return ''
    if space_match.match(line):
        outline = space_match.sub('', line)
    elif line_match.match(line):
        if not in_comment_section:
            outline = line_match.sub('/** ', line)
        else:
            outline = line_match.sub('', line)
            #print(outline)
        in_comment_section = True
    else:
        if (len(line) > 0):
            in_comment_section = False
        if not in_comment_section and prev_comment_section:
            outline = '*/\n{}'.format(line)
        else:
            outline = line
    # delete empty lines inside of comments
    if len(outline.strip(' ').strip('\t').strip('\n')) == 0 and in_comment_section:
        outline = ''
    return outline

def main():
    parse_ext = '.h'
    src_dir = os.path.abspath('modules')
    infiles = glob.glob(src_dir+'/*.h')
    for infile in infiles:
        print("Parsing {}".format(infile))
        newlines = ''
        with open(infile, 'r') as f:
            newlines = list(map(process_line, f.readlines()))
        with open(infile, 'w') as f:
            for line in newlines:
                f.write(line)

if __name__ == '__main__':
    main()
