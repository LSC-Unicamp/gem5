from m5.objects.BaseO3CPU import BaseO3CPU
from m5.params import *

class MatrixO3CPU(BaseO3CPU):
    type = "MatrixO3CPU"
    cxx_class = "gem5::o3::MatrixCPU"
    cxx_header = "cpu/o3/dyn_inst.hh"

    mdcache_port = RequestPort("Matrix Data Port")
    
