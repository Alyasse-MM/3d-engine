import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import math
import tools

def render_frame(state, vertices, faces, fps, pause_time, fig):
    if not plt.fignum_exists(fig.number): return
    plt.clf()

    rx_model, ry_model = tools.get_rotation_matrices(math.radians(state.angle_x), math.radians(state.angle_y))
    _, ry_view = tools.get_rotation_matrices(0, math.radians(-state.camera_yaw)) 
    
    transformed_vertices = []
    for v in vertices:
        world_v = rx_model @ (ry_model @ v)
        translated_v = world_v - state.camera_pos
        view_v = ry_view @ translated_v
        transformed_vertices.append(view_v)
    transformed_vertices = np.array(transformed_vertices)

    faces_to_draw = []

    for face_indices, color in faces:
        face_verts_3d = transformed_vertices[face_indices]
        
        tri_1 = [face_verts_3d[0], face_verts_3d[1], face_verts_3d[2]]
        tri_2 = [face_verts_3d[0], face_verts_3d[2], face_verts_3d[3]]
        
        for tri in [tri_1, tri_2]:
            A, B, C = tri[0], tri[1], tri[2]
            normal = np.cross(B - A, C - A)
            
            if normal[2] < 0: 
                clipped_tris = tools.clip_triangle_against_plane(tri[0], tri[1], tri[2])
                
                for tri_points in clipped_tris:
                    face_2d = []
                    z_values = [] 
                    for x, y, z in tri_points:
                        focal_length = 200
                        if z < 0.01: z = 0.01 
                        face_2d.append([(x / z) * focal_length, (y / z) * focal_length])
                        z_values.append(z)
                    
                    z_sort_key = np.mean(z_values)
                    faces_to_draw.append((face_2d, color, z_sort_key))

    faces_to_draw.sort(key=lambda x: x[2], reverse=True)

    ax = plt.gca()
    
    for points, color, _ in faces_to_draw:
        poly = Polygon(points, facecolor=color, edgecolor=color, linewidth=0.5, alpha=1.0) 
        ax.add_patch(poly)

    plt.xlim(-100, 100)
    plt.ylim(-100, 100)
    plt.axis('off')
    
    info = (f"Pos: {state.camera_pos}\nCam angle: {state.camera_yaw}°")
    plt.text(-95, 90, info, fontsize=10, family='monospace')
    plt.pause(pause_time)