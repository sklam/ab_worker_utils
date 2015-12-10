# How to submit jobs to the numba build queue

## Requirements

conda packages:

* `anaconda-build`
* `anaconda-client`

## Submit a directory

Submit any directory with a `.binstar.yml` file.  Anaconda-build will
package up the directory recursively for you.  

**Note for git directory:** anaconda-build will use git to find out
what files to package up.  It seems that it will only use the files
that are tracked by git.

```bash
anaconda build submit --queue numba/ci_test <path-to-dir>
```

Use `--no-upload` to disable uploading result package:

```bash
anaconda build submit --queue numba/ci_test <path-to-dir> --no-upload
```

## Sample `.binstar.yml`

```yaml
package: numba
platform:
    - osx-64
    - linux-32
    - linux-64
    - win-32
    - win-64
engine:
    - python=2.7 numpy=1.10
    - python=3.4 numpy=1.10
script:
    - python my_conda_build.py -c numba <condarecipe> --build-only --no-anaconda-upload

---
# CUDA machines
platform:
    - linux-64-cuda
engine:
    - python=2.7 numpy=1.10
    - python=3.4 numpy=1.10
script:
    - export HAS_CUDA=1
    - python my_conda_build.py -c numba <condarecipe> --build-only --no-anaconda-upload

---
# HSA machines
platform:
    - linux-64-hsa-kaveri
    - linux-64-hsa-dgpu
engine:
    - python=2.7 numpy=1.10
    - python=3.4 numpy=1.10
script:
    - export HAS_HSA=1
    - python my_conda_build.py -c numba <condarecipe> --build-only --no-anaconda-upload
```

The separator are used because we need to special case CUDA and HSA machines.  

The `my_conda_build.py` is a temporary workaround for anaconda-build not propagating `--numpy x.y`.  Otherwise, we could
have used conda build directly.

The `meta.yml` file for conda build is expected to have the following
in the `run` section:

```yaml
run:
  - python
  # ... excerpt
  - {{ 'cudatoolkit' if environ.get('HAS_CUDA', False) else 'python' }}
  - {{ 'libhlc' if environ.get('HAS_HSA', False) else 'python' }}
```

So that it will pull in `cudatoolkit` and `libhlc` on certain platforms.

Also note that conda requires a version number to work:

```yaml
version: {{ environ.get('GIT_DESCRIBE_TAG','devel') }}
```

If you are submitting a local directory, it won't have the git tag.
We need to set the version to something other than a empty string; e.g. "devel".

## Running on a subset of the platforms

### One platform
It is possible to run on individual platform using the `--platform` option of `anaconda build submit`.

For example:

```bash
anaconda build submit --platform linux-64-cuda --queue numba/ci_test .
```

The above will only submit to the `linux-64-cuda` platform.

The dry-run `-n` flag is useful to verify you are doing the right thing.

### Multiple platforms

The only way to run on multiple selected platforms is to modify the `.binstar.yml` (comment out the platforms that you are not interested).

## Utilities

Worker status: https://anaconda.org/numba/queues/ci_test/workers

Use the "last seen" time to determine the status of the workers.
