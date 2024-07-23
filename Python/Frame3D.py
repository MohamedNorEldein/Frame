import ctypes
from ctypes import c_void_p, c_double, c_uint8, c_size_t

# Load the shared library
lib = ctypes.CDLL('build/Release/MyProject.dll')

# Define the function prototypes for NodeF3D
lib.NodeF3DGetPos.argtypes = [c_void_p, c_uint8]
lib.NodeF3DGetPos.restype = c_double

lib.NodeF3DSetPos.argtypes = [c_void_p, c_uint8, c_double]
lib.NodeF3DSetPos.restype = None

lib.NodeF3DGetDis.argtypes = [c_void_p, c_uint8]
lib.NodeF3DGetDis.restype = c_double

lib.NodeF3DSetDis.argtypes = [c_void_p, c_uint8, c_double]
lib.NodeF3DSetDis.restype = None

lib.NodeF3DGetNodalForce.argtypes = [c_void_p, c_uint8]
lib.NodeF3DGetNodalForce.restype = c_double

lib.NodeF3DSetNodalForce.argtypes = [c_void_p, c_uint8, c_double]
lib.NodeF3DSetNodalForce.restype = None

lib.NodeF3DGetSupport.argtypes = [c_void_p, c_uint8]
lib.NodeF3DGetSupport.restype = c_double

lib.NodeF3DSetSupport.argtypes = [c_void_p, c_uint8, c_double]
lib.NodeF3DSetSupport.restype = None

# Python API to interact with the NodeF3D structure
class NodeF3D:
    def __init__(self, pointer):
        self.node = pointer

    def get_pos(self, index):
        return lib.NodeF3DGetPos(self.node, index)

    def set_pos(self, index, value):
        lib.NodeF3DSetPos(self.node, index, value)

    def get_dis(self, index):
        return lib.NodeF3DGetDis(self.node, index)

    def set_dis(self, index, value):
        lib.NodeF3DSetDis(self.node, index, value)

    def get_nodal_force(self, index):
        return lib.NodeF3DGetNodalForce(self.node, index)

    def set_nodal_force(self, index, value):
        lib.NodeF3DSetNodalForce(self.node, index, value)

    def get_support(self, index):
        return lib.NodeF3DGetSupport(self.node, index)

    def set_support(self, index, value):
        lib.NodeF3DSetSupport(self.node, index, value)

# Define the function prototypes for MemberF3D
lib.MemberF3DGetStartIndex.argtypes = [c_void_p]
lib.MemberF3DGetStartIndex.restype = c_size_t

lib.MemberF3DGetEndIndex.argtypes = [c_void_p]
lib.MemberF3DGetEndIndex.restype = c_size_t

lib.MemberF3DGetIendForces.argtypes = [c_void_p, c_uint8]
lib.MemberF3DGetIendForces.restype = c_double

lib.MemberF3DGetJendForces.argtypes = [c_void_p, c_uint8]
lib.MemberF3DGetJendForces.restype = c_double

lib.MemberF3DSetIendForces.argtypes = [c_void_p, c_uint8, c_double]
lib.MemberF3DSetIendForces.restype = None

lib.MemberF3DSetJendForces.argtypes = [c_void_p, c_uint8, c_double]
lib.MemberF3DSetJendForces.restype = None

# Python API to interact with the MemberF3D structure
class MemberF3D:
    def __init__(self, pointer):
        self.member = pointer

    def get_start_index(self):
        return lib.MemberF3DGetStartIndex(self.member)

    def get_end_index(self):
        return lib.MemberF3DGetEndIndex(self.member)

    def get_iend_forces(self, index):
        return lib.MemberF3DGetIendForces(self.member, index)

    def get_jend_forces(self, index):
        return lib.MemberF3DGetJendForces(self.member, index)

    def set_iend_forces(self, index, value):
        lib.MemberF3DSetIendForces(self.member, index, value)

    def set_jend_forces(self, index, value):
        lib.MemberF3DSetJendForces(self.member, index, value)

# Define the function prototypes for Frame3D
lib.CreateFrame3D.restype = c_void_p

lib.Frame3DAddNode.argtypes = [c_void_p, c_double, c_double, c_double]
lib.Frame3DAddNode.restype = c_size_t

lib.Frame3DAddMember.argtypes = [c_void_p, c_size_t, c_size_t, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double, c_double]
lib.Frame3DAddMember.restype = c_size_t

lib.Frame3DAddSupport.argtypes = [c_void_p, c_size_t, c_size_t, c_double]
lib.Frame3DAddSupport.restype = None

lib.Frame3DAddNodalForce.argtypes = [c_void_p, c_size_t, c_size_t, c_double]
lib.Frame3DAddNodalForce.restype = None

lib.Frame3DSolve.argtypes = [c_void_p]
lib.Frame3DSolve.restype = None

lib.Frame3DPrint.argtypes = [c_void_p]
lib.Frame3DPrint.restype = None

lib.DeleteFrame3D.argtypes = [c_void_p]
lib.DeleteFrame3D.restype = None

lib.Frame3DgetNodeDis.argtypes = [c_void_p, c_size_t, c_uint8]
lib.Frame3DgetNodeDis.restype = c_double

lib.Frame3DgetNodeCount.argtypes = [c_void_p]
lib.Frame3DgetNodeCount.restype = c_size_t

lib.Frame3DgetMemberCount.argtypes = [c_void_p]
lib.Frame3DgetMemberCount.restype = c_size_t

lib.Frame3DgetNode.argtypes = [c_void_p, c_size_t]
lib.Frame3DgetNode.restype = c_void_p

lib.Frame3DgetMember.argtypes = [c_void_p, c_size_t]
lib.Frame3DgetMember.restype = c_void_p

# Python API to interact with the Frame3D structure
class Frame3D:
    def __init__(self):
        self.frame = lib.CreateFrame3D()

    def get_node_count(self):
        return lib.Frame3DgetNodeCount(self.frame)

    def get_member_count(self):
        return lib.Frame3DgetMemberCount(self.frame)

    def get_node(self, index):
        node_pointer = lib.Frame3DgetNode(self.frame, index)
        return NodeF3D(node_pointer)

    def get_member(self, index):
        member_pointer = lib.Frame3DgetMember(self.frame, index)
        return MemberF3D(member_pointer)

    def add_node(self, x, y, z):
        return lib.Frame3DAddNode(self.frame, x, y, z)

    def add_member(self, i, j, E, G, A, Ix, Iy, J, LocalXx, LocalXy, LocalXz):
        return lib.Frame3DAddMember(self.frame, i, j, E, G, A, Ix, Iy, J, LocalXx, LocalXy, LocalXz)

    def add_support(self, i, j, dis):
        lib.Frame3DAddSupport(self.frame, i, j, dis)

    def add_nodal_force(self, i, j, force):
        lib.Frame3DAddNodalForce(self.frame, i, j, force)

    def solve(self):
        lib.Frame3DSolve(self.frame)

    def print(self):
        for i in range(0, self.get_node_count()):
            node =  self.get_node(i)
            print(f"""

    Node {i}
        Pos             < {node.get_pos(0)}   ,   {node.get_pos(1)},    {node.get_pos(2)}>        
        Nodal Force     < {node.get_nodal_force(0)}   ,   {node.get_nodal_force(1)},    {node.get_nodal_force(2)}>
        Nodal Moment    < {node.get_nodal_force(3)}   ,   {node.get_nodal_force(4)},    {node.get_nodal_force(5)}>
        Dis             < {node.get_dis(0)}   ,   {node.get_dis(1)},    {node.get_dis(2)}>
        Rotation        < {node.get_dis(3)}   ,   {node.get_dis(4)},    {node.get_dis(5)}>

            """)

        for i in range(0, self.get_member_count()):
            member =  self.get_member(i)
            print(f"""

    Memeber {i}
        Nodes {member.get_start_index()}->{member.get_end_index()}
        N       {round( member.get_iend_forces(0),2)}  \t->\t{round(member.get_jend_forces(0),2)}
        Qx      {round( member.get_iend_forces(1),2)}  \t->\t{round(member.get_jend_forces(1),2)}    
        Qy      {round( member.get_iend_forces(2),2)}  \t->\t{round(member.get_jend_forces(2),2)}
    \t\t
        Mx      {round( member.get_iend_forces(3),2)}  \t->\t{round(member.get_jend_forces(3),2)}
        My      {round( member.get_iend_forces(4),2)}  \t->\t{round(member.get_jend_forces(4),2)}
        T       {round( member.get_iend_forces(5),2)}  \t->\t{round(member.get_jend_forces(5),2)}
            """)


    def __del__(self):
        # Ensure cleanup is done
        lib.DeleteFrame3D(self.frame)
