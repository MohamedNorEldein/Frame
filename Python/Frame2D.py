
import ctypes
import os

class Frame2D:
    def __init__(self):
                
        # Load the shared library
        self.lib = ctypes.CDLL('./build\Release\MyProject.dll')

        # Define the argument and return types for the C functions
        self.lib.CreateFrame2D.restype = ctypes.c_void_p
        self.lib.Frame2DAddNode.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
        self.lib.Frame2DAddNode.restype = ctypes.c_size_t
        self.lib.Frame2DAddMember.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double, ctypes.c_double, ctypes.c_double]
        self.lib.Frame2DAddMember.restype = ctypes.c_size_t
        self.lib.Frame2DAddSupport.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double]
        self.lib.Frame2DAddNodalForce.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double]
        self.lib.Frame2DSolve.argtypes = [ctypes.c_void_p]
        self.lib.Frame2DPrint.argtypes = [ctypes.c_void_p]
        self.lib.DeleteFrame2D.argtypes = [ctypes.c_void_p]

        self.obj = libexample.CreateFrame2D()

    def add_node(self, x, y):
        return self.lib.Frame2DAddNode(self.obj, x, y)

    def add_member(self, i, j, E, A, I):
        return self.lib.Frame2DAddMember(self.obj, i, j, E, A, I)

    def add_support(self, i, j, dis):
        self.lib.Frame2DAddSupport(self.obj, i, j, dis)

    def add_nodal_force(self, i, j, force):
        self.lib.Frame2DAddNodalForce(self.obj, i, j, force)

    def solve(self):
        self.lib.Frame2DSolve(self.obj)

    def print(self):
        self.lib.Frame2DPrint(self.obj)

    def __del__(self):
        self.lib.DeleteFrame2D(self.obj)

