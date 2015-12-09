from __future__ import print_function
import os
import sys
import subprocess
import traceback


def run_cmd(cmd):
    sys.stdout.flush()
    sys.stderr.flush()
    subprocess.check_call(cmd, shell=True,
                          stdout=sys.stdout, stderr=sys.stdout)


def do_work():
    print("do_work")
    base = sys.prefix
    scripts = os.path.join(base, 'Scripts')

    add_to_path = [base, scripts]

    parentdir = os.path.dirname(base)
    if os.path.basename(parentdir) == 'envs':
        # we are in an environment
        # prepend scripts of parent
        maindir = os.path.dirname(parentdir)
        add_to_path.append(os.path.join(maindir, 'Scripts'))

    print("Add the folloing to path")
    for idx, path in enumerate(add_to_path):
        print("[{0}] {1}".format(idx, path))

    print("Prending to PATH")
    newpaths = ';'.join(add_to_path)
    os.environ['PATH'] = ';'.join([newpaths, os.environ['PATH']])

    run_cmd("conda info")
    run_cmd("anaconda -h")
    run_cmd("python -V")
    yml = sys.argv[1]
    print("Use YAML:", yml)
    run_cmd("python control.py start {0}".format(yml))


def main():
    cwd = os.path.dirname(__file__)
    os.chdir(os.path.join(cwd, '..'))
    try:
        sys.stdout = open("log_stdout.txt", 'w')
        do_work()
    except:
        traceback.print_exc()
    finally:
        sys.stdout.close()
        sys.stdout = sys.__stdout__


if __name__ == '__main__':
    main()
