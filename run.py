import os
from pythonFoam import simpleFoamRun, print_hello

print_hello()

current_path = os.path.dirname(__file__)

case_path = os.path.join(current_path, "test")


simpleFoamRun(case_path)

