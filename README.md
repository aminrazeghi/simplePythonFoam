

## SimplePythonFOAM

A simple example of using Python to control OpenFOAM by wrapping a few of OpenFOAM commands in Python functions.


### Installation
First activate the OpenFOAM environment by running the following command in the terminal:
```bash
source /opt/openfoam7/etc/bashrc
# or
source $HOME/OpenFOAM/OpenFOAM-v<version>/etc/bashrc
```
where `<version>` is the version of OpenFOAM you have installed.

Then install the required Python packages by running the following command in the terminal:
```bash
pip intall setuptools
```
build and install the package by running the following command in the terminal:
```bash
python setup.py build_ext --inplace
python setup.py install
```

