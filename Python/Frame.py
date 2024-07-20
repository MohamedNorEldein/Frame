
import ctypes
import os

# Load the shared library
libexample = ctypes.CDLL('./build\Release\MyProject.dll')


# Define the argument and return types for the C functions
libexample.CreateFrame2D.restype = ctypes.c_void_p
libexample.Frame2DAddNode.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
libexample.Frame2DAddNode.restype = ctypes.c_size_t
libexample.Frame2DAddMember.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double, ctypes.c_double, ctypes.c_double]
libexample.Frame2DAddMember.restype = ctypes.c_size_t
libexample.Frame2DAddSupport.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double]
libexample.Frame2DAddNodalForce.argtypes = [ctypes.c_void_p, ctypes.c_size_t, ctypes.c_size_t, ctypes.c_double]
libexample.Frame2DSolve.argtypes = [ctypes.c_void_p]
libexample.Frame2DPrint.argtypes = [ctypes.c_void_p]
libexample.DeleteFrame2D.argtypes = [ctypes.c_void_p]

class Frame2D:
    def __init__(self):
        self.obj = libexample.CreateFrame2D()

    def add_node(self, x, y):
        return libexample.Frame2DAddNode(self.obj, x, y)

    def add_member(self, i, j, E, A, I):
        return libexample.Frame2DAddMember(self.obj, i, j, E, A, I)

    def add_support(self, i, j, dis):
        libexample.Frame2DAddSupport(self.obj, i, j, dis)

    def add_nodal_force(self, i, j, force):
        libexample.Frame2DAddNodalForce(self.obj, i, j, force)

    def solve(self):
        libexample.Frame2DSolve(self.obj)

    def print(self):
        libexample.Frame2DPrint(self.obj)

    def __del__(self):
        libexample.DeleteFrame2D(self.obj)


# Example usage
if __name__ == "__main__":

    frame = Frame2D()
    node_index1 = frame.add_node(0.0, 0.0)
    node_index2 = frame.add_node(0.0, 5.0)
    node_index3 = frame.add_node(4.0, 5.0)
    node_index4 = frame.add_node(4.0, 0.0)

    member_index = frame.add_member(node_index1, node_index2, 2*10**6, 10.0, 0.004)
    member_index = frame.add_member(node_index2, node_index3,  2*10**6, 10.0, 0.004)
    member_index = frame.add_member(node_index3, node_index4,  2*10**6, 10.0, 0.004)


    frame.add_support(node_index1, 0, 0)
    frame.add_support(node_index1, 1, 0)
    frame.add_support(node_index4, 1, 0)



    frame.add_nodal_force(node_index2, 0, 100.0)

    frame.solve()
    frame.print()
