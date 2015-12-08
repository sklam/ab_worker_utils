from __future__ import print_function, absolute_import
import os
import sys
import subprocess
import yaml
from collections import namedtuple

def write_config(argv):
    try:
        [filename] = argv
    except ValueError:
        print("config: expected args: <filename>")
        sys.exit(1)

    data = {}
    data['auth'] = input("auth token:")
    data['worker'] = input("worker name:")
    data['queue'] = input("queue name:")

    with open(filename, 'w') as fout:
        yaml.dump(data, fout)


config_struct = namedtuple("config_struct", ['auth', 'worker', 'queue'])


def read_config(filename):

    with open(filename, 'r') as fin:
        data = yaml.load(fin)

    return config_struct(**data)


def call_script(filename, **envvars):
    # Set ENV
    for k, v in envvars.items():
        os.environ[k] = v
    # Launch script
    subprocess.check_call(get_script(filename), shell=True)


def get_script(fileprefix):
    if sys.platform.startswith('win'):
        suffix = '.bat'
    else:
        suffix = '.sh'
    return os.path.abspath(os.path.join('scripts', fileprefix + suffix))


def config_to_env_dict(config):
    return dict(BINSTAR_AUTH=config.auth,
                BINSTAR_WORKER=config.worker,
                BINSTAR_QUEUE=config.queue)


def start_worker(argv):
    try:
        [filename] = argv
    except ValueError:
        print("start: expected args: <filename>")
        sys.exit(1)

    config = read_config(filename)
    call_script('start_worker', **config_to_env_dict(config))


def setup_worker(argv):
    try:
        [filename] = argv
    except ValueError:
        print("setup: expected args: <filename>")
        sys.exit(1)

    config = read_config(filename)
    call_script('setup_worker', **config_to_env_dict(config))


def main():
    commands = {
        'config': write_config,
        'start': start_worker,
    }
    cmd = sys.argv[1]
    try:
        fn = commands[cmd]
    except NameError:
        print("command {0!r} is invalid".format(cmd))
        sys.exit(1)
    else:
        fn(sys.argv[2:])


if __name__ == '__main__':
    main()
