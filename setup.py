from setuptools import setup, Extension
import os
import sysconfig

include_dirs = [
            os.getenv("FOAM_SRC") + "/TurbulenceModels/turbulenceModels/lnInclude",
            os.getenv("FOAM_SRC") + "/TurbulenceModels/incompressible/lnInclude",
            os.getenv("FOAM_SRC") + "/transportModels",
            os.getenv("FOAM_SRC") + "/transportModels/incompressible/singlePhaseTransportModel",
            os.getenv("FOAM_SRC") + "/finiteVolume/lnInclude",
            os.getenv("FOAM_SRC") + "/meshTools/lnInclude",
            os.getenv("FOAM_SRC") + "/sampling/lnInclude",
            os.getenv("FOAM_SRC") + "/fileFormats/lnInclude",
            os.getenv("FOAM_SRC") + "/surfMesh/lnInclude",
            os.getenv("FOAM_SRC") + "/dynamicFvMesh/lnInclude",
            os.getenv("FOAM_SRC") + "/dynamicMesh/lnInclude",
            # These are common for all OpenFOAM executives
            os.getenv("FOAM_SRC") + "/OpenFOAM/lnInclude",
            os.getenv("FOAM_SRC") + "/OSspecific/POSIX/lnInclude",
            os.getenv("FOAM_LIBBIN"),
            # # CoDiPack and MeDiPack
            # os.getenv("FOAM_SRC") + "/codipack/include",
            # os.getenv("FOAM_SRC") + "/medipack/include",
            # os.getenv("FOAM_SRC") + "/medipack/src",
            # os.getenv("PETSC_DIR") + "/include",
            # os.getenv("PETSC_DIR") + "/" + os.getenv("PETSC_ARCH") + "/include",
            "./",
    sysconfig.get_paths()["include"],
]
compile_args = [
            "-std=c++11",
            "-Wno-deprecated-copy",
            "-DIncompressibleFlow",
            "-m64",
            "-DOPENFOAM_PLUS=1812",
            "-Dlinux64",
            "-DWM_ARCH_OPTION=64",
            "-DWM_DP",
            "-DWM_LABEL_SIZE=32",
            # "-Wall",
            # "-Wextra",
            # "-Wnon-virtual-dtor",
            # "-Wno-unused-parameter",
            # "-Wno-invalid-offsetof",
            # "-O3",
            "-DNoRepository",
            "-ftemplate-depth-100",
            "-fPIC",
            "-c",
            # "-D" + codiADMode,
]
libraries = [
    "finiteVolume",
    "fvOptions",
    "meshTools",
    "sampling",
    "turbulenceModels",
    "incompressibleTurbulenceModels",
    "incompressibleTransportModels",
    "atmosphericModels",
    "OpenFOAM",
    "dl",
    "m",
]
module = Extension(
    "pythonFoam",
    sources=["mySimpleFoam.C"],
    language="c++",
    include_dirs=include_dirs,
    library_dirs=[
            os.getenv("FOAM_LIBBIN"),
    ],
    libraries=libraries,
    extra_compile_args=compile_args,
)

setup(
    name="pythonFoam", version="1.0", description=" a description", ext_modules=[module],
    package_data= {"myextension": ["myextension.pyi"]},
    include_package_data=True,
)
