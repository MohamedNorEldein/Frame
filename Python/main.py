import Frame3D

# Example usage
if __name__ == '__main__':
    
    frame = Frame3D.Frame3D()
    
    frame.add_node(0.0, 0.0, 0.0)
    frame.add_node(0.0, 0.0, 10.0)
    frame.add_node(2.0, 0.0, 10.0)
    
    frame.add_member(0, 1, 210e9, 81e9, 0.01, 1e-4, 1e-4, 1e-4,1,0,0)
    frame.add_member(1, 2, 210e9, 81e9, 0.01, 1e-4, 1e-4, 1e-4,0,0,1)
    
    frame.add_support(0, 0, 0.0)    #fx
    frame.add_support(0, 1, 0.0)    #fy  
    frame.add_support(0, 2, 0.0)    #fz  
    frame.add_support(0, 3, 0.0)    #Mx 
    frame.add_support(0, 4, 0.0)    #My
    frame.add_support(0, 5, 0.0)    #Mz

    frame.add_nodal_force(2, 1, 10000.0)

    frame.solve()
    frame.print()
    