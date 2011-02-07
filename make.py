#!/usr/bin/env python

import os, commands, re
from fabricate import *

targets  = {
    'skel': {
        'libs': [],
        'sources': ['skel']
    }
}

compiler = commands.getoutput('which llvm-g++ || echo -n "g++"')

cflags  = '''-g -O2 -ansi -pedantic-errors -Werror
             -Wall -Wextra -Wconversion -Wshadow -Weffc++
             -Wpointer-arith -Wcast-qual -Wwrite-strings
             -D__USE_FIXED_PROTOTYPES__
             -Ivendor'''.split()

def pkgconfig(what, libs):
    cmd = "pkg-config --%s %s 2>/dev/null" % (what, ' '.join(libs))
    return commands.getoutput(cmd).split()

def flags_for_target(target, what='cflags'):
    if 'libs' in targets[target]:
        return pkgconfig(what, targets[target]['libs'])
    else:
        return []

def build():
    for target in targets:
        if 'sources' in targets[target]:
            compile(target)
            link(target)

def oname(filename):
    return os.path.join('build', os.path.basename(filename))

def compile(target):
    for source in targets[target]['sources']:
        run(compiler, '-c', 'src/' + source + '.cpp', '-o', oname(source+'.o'), cflags, flags_for_target(target, 'cflags'))

def link(target):
    objects = [oname(s+'.o') for s in targets[target]['sources']]
    run(compiler, objects, '-o', oname(target), flags_for_target(target, 'libs'))

def clean():
    autoclean()

def rebuild():
    clean()
    build()

class QuieterBuilder(Builder):
    def echo_command(self, command):
        compiling = re.compile(r'-c (\S+\.cpp)\b', re.IGNORECASE).search(command)
        linking   = re.compile(r'((?:\S+\.o\s+)+)-o (\S+)\b', re.IGNORECASE).search(command)
        if compiling:
            self.echo('CC ' + compiling.group(1))
        elif linking:
            # self.echo('LD ' + linking.group(1) + '-> ' + linking.group(2))
            self.echo('LD ' + linking.group(2))
        else:
            self.echo(command)

setup(builder=QuieterBuilder)
main()
