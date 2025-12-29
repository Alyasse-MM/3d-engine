import numpy as np

class EngineState:
    def __init__(self):
        # Model Rotation
        self.angle_x = 0
        self.angle_y = 0
        
        # Camera
        self.camera_pos = np.array([0.0, 0.0, -4.0])
        self.camera_yaw = 0