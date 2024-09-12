/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    simpleFoam

Group
    grpIncompressibleSolvers

Description
    Steady-state solver for incompressible, turbulent flows.

    \heading Solver details
    The solver uses the SIMPLE algorithm to solve the continuity equation:

        \f[
            \div \vec{U} = 0
        \f]

    and momentum equation:

        \f[
            \div \left( \vec{U} \vec{U} \right) - \div \gvec{R}
          = - \grad p + \vec{S}_U
        \f]

    Where:
    \vartable
        \vec{U} | Velocity
        p       | Pressure
        \vec{R} | Stress tensor
        \vec{S}_U | Momentum source
    \endvartable

    \heading Required fields
    \plaintable
        U       | Velocity [m/s]
        p       | Kinematic pressure, p/rho [m2/s2]
        \<turbulence fields\> | As required by user selection
    \endplaintable

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "fvCFD.H"
#include "fvOptions.H"
#include "simpleControl.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include <Python.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// int main(int argc, char *argv[])
void solve(const char *caseDir) {
  std::cout << "caseDir: " << caseDir << std::endl;
  int argc = 3;
  // char* argv[] = {"mySimpleFoam", "-parallel"};
  char *argv[] = {"PythonSimpleFoam"};
  argv[1] = "-case";
  argv[2] = const_cast<char*>(caseDir);
  char **argvPointer = argv; // Create a pointer to the array
  
  argList::addNote("Steady-state solver for incompressible, turbulent flows.");

  // #include "postProcess.H"

  // #include "addCheckCaseOptions.H"
  // #include "setRootCaseLists.H"
  // #include "setRootCaseListOptions.H"
  // #include "setRootCase.H"
  // Foam::argList args(argc, argv);
  Foam::argList args(argc, argvPointer);

#include "setRootCaseListOutput.H"

  // #include "createTime.H"

  Foam::Time runTime(Foam::Time::controlDictName, args);
#include "createMesh.H"
#include "createControl.H"
#include "initContinuityErrs.H"
#include "createFields.H"

  turbulence->validate();

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

  Info << "\nStarting time loop\n" << endl;

  while (simple.loop()) {
    Info << "Time = " << runTime.timeName() << nl << endl;

    // --- Pressure-velocity SIMPLE corrector
    {
#include "UEqn.H"
#include "pEqn.H"
    }

    laminarTransport.correct();
    turbulence->correct();

    runTime.write();

    runTime.printExecutionTime(Info);
  }

  Info << "End\n" << endl;

  // return 0;
}

void printingHello() { std::cout << "printing hello\n"; }

extern "C" {
static PyObject *mySimpleFoamWrapper(PyObject *self, PyObject *args_python) {
  const char *arg1;
  if (!PyArg_ParseTuple(args_python, "s", &arg1)) {
    return NULL;
  }
  solve(arg1);
  Py_RETURN_NONE;
}

static PyObject *printingHelloWrapper(PyObject *self, PyObject *args_python) {
  if (!PyArg_ParseTuple(args_python, "")) {
    return NULL;
  }
  printingHello();
  Py_RETURN_NONE;
}

static PyMethodDef MyMethods[] = {
    {"simpleFoamRun", mySimpleFoamWrapper, METH_VARARGS,
     "Executing simpleFoam from python"},
    {"print_hello", printingHelloWrapper, METH_VARARGS, "Print Hello"},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef pythonFoammodule = {
    PyModuleDef_HEAD_INIT,
    "pythonFoam",                  // Name of the module
    "an extension for simpleFoam", // Module documentation (docstring)
    -1, // Size of per-interpreter state of the module, or -1 if the module
        // keeps state in global variables
    MyMethods // The methods defined in this module
};

PyMODINIT_FUNC PyInit_pythonFoam(void) {
  return PyModule_Create(&pythonFoammodule);
}
}

// ************************************************************************* //
