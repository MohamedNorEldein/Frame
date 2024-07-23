import vtk

class VTKFrame:
    def __init__(self):
        # Initialize the renderer, render window, and interactor
        self.renderer = vtk.vtkRenderer()
        self.renderWindow = vtk.vtkRenderWindow()
        self.renderWindow.AddRenderer(self.renderer)
        self.renderWindowInteractor = vtk.vtkRenderWindowInteractor()
        self.renderWindowInteractor.SetRenderWindow(self.renderWindow)

    def create_line(self, start_point, end_point):
        # Create a line
        lineSource = vtk.vtkLineSource()
        lineSource.SetPoint1(start_point)
        lineSource.SetPoint2(end_point)

        # Create a mapper
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(lineSource.GetOutputPort())

        # Create an actor
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)

        return actor

    def add_line(self, start_point, end_point):
        # Create a line actor
        line_actor = self.create_line(start_point, end_point)
        
        # Add the actor to the renderer
        self.renderer.AddActor(line_actor)

    def set_background_color(self, color):
        self.renderer.SetBackground(color)

    def set_window_size(self, width, height):
        self.renderWindow.SetSize(width, height)

    def render(self):
        # Render and start interaction
        self.renderWindow.Render()
        self.renderWindowInteractor.Start()
