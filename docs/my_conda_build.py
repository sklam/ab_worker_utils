from __future__ import print_function
import os
import sys
import subprocess

INCLUDED = frozenset(['python', 'numpy'])


def get_env_setting():
    env = os.environ['BINSTAR_ENGINE']
    result = []
    terms = env.split()
    for term in terms:
        package, version = term.split('=', 2)
        if package in INCLUDED:
            result.append('--{0}={1}'.format(package, version))

    return ' '.join(result)


def main():
    env = get_env_setting()
    cmd = "conda build {env} {args}".format(env=env,
                                            args=' '.join(sys.argv[1:]))

    print(cmd)
    sys.stdout.flush()
    sys.stderr.flush()
    try:
        subprocess.check_call(cmd,
                              shell=True,
                              stdout=sys.stdout,
                              stderr=sys.stderr)
    finally:
        sys.stdout.flush()
        sys.stderr.flush()

if __name__ == "__main__":
    main()
