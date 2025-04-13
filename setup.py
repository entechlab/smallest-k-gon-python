import sys
from setuptools import setup, Extension
import pybind11

# Check if NumPy is installed, needed for headers during build
try:
    import numpy
except ImportError:
    print("Error: NumPy is required to build this extension.")
    print("Please install NumPy: pip install numpy")
    sys.exit(1)

cpp_args = ["-std=c++11", "-O3"]  # Use C++17 standard and optimization

ext_modules = [
    Extension(
        "smallest_kgon",  # Module name in Python
        ["smallest-k-gon.cpp"],  # Source file
        include_dirs=[
            pybind11.get_include(),
            numpy.get_include(),  # Add NumPy header path
            # Add other include paths if needed
        ],
        language="c++",
        extra_compile_args=cpp_args,
    ),
]

setup(
    name="smallest_kgon",
    version="0.1.0",
    description="Python wrapper for C++ Smallest Kgon enclosing Ngon",
    author="Amir Kimiyaie",  # Replace with your name
    author_email="amirrad3010@gmail.com",  # Replace with your email
    ext_modules=ext_modules,
    setup_requires=["pybind11>=2.6", "numpy"],  # Dependencies for setup
    zip_safe=False,  # Important for C extensions
)
