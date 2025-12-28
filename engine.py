import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import math
import argparse

try:
    plt.rcParams['keymap.save'].remove('s')
    plt.rcParams['keymap.quit'].remove('q')
    if 'a' in plt.rcParams['keymap.all']:
        plt.rcParams['keymap.all'].remove('a')
except:
    pass

def frequency_manager(fps):
    if fps <= 0: return 0.001
    return 1.0 / fps

def parameters_manager():
    parser = argparse.ArgumentParser(description="Custom 3D Engine")
    parser.add_argument('--fps', type=int, default=30, help="Target Frames Per Second")
    args = parser.parse_args()
    return args, frequency_manager(args.fps)

def input_manager(event):
    global angle_x, angle_y, camera_pos, camera_yaw
    
    rot_step = 1
    move_step = 0.5
    cam_rot_step = 5
    
    rad = math.radians(camera_yaw)
    sin_a = math.sin(rad)
    cos_a = math.cos(rad)
    
    if event.key == 'left':
        angle_y = (angle_y + rot_step) % 360
    elif event.key == 'right':
        angle_y = (angle_y - rot_step) % 360
    elif event.key == 'up':
        angle_x = (angle_x + rot_step) % 360
    elif event.key == 'down':
        angle_x = (angle_x - rot_step) % 360
        
    elif event.key == 'a':
        camera_yaw = (camera_yaw - cam_rot_step) % 360
    elif event.key == 'e':
        camera_yaw = (camera_yaw + cam_rot_step) % 360

    elif event.key == 'z' or event.key == 'w': 
        camera_pos[0] += sin_a * move_step
        camera_pos[2] += cos_a * move_step
    elif event.key == 's':
        camera_pos[0] -= sin_a * move_step
        camera_pos[2] -= cos_a * move_step
    elif event.key == 'q': 
        camera_pos[0] -= cos_a * move_step
        camera_pos[2] += sin_a * move_step
    elif event.key == 'd':
        camera_pos[0] += cos_a * move_step
        camera_pos[2] -= sin_a * move_step

vertices = np.array([
    [-1, -1, -1], [1, -1, -1], [1, 1, -1], [-1, 1, -1],
    [-1, -1, 1], [1, -1, 1], [1, 1, 1], [-1, 1, 1],

    [4, -1, 4], [6, -1, 4], [6, 1, 4], [4, 1, 4],
    [4, -1, 6], [6, -1, 6], [6, 1, 6], [4, 1, 6]
])

faces = [
    ([0, 3, 2, 1], 'red'),
    ([4, 5, 6, 7], 'cyan'),
    ([0, 4, 7, 3], 'green'),
    ([1, 2, 6, 5], 'blue'),
    ([3, 7, 6, 2], 'yellow'),
    ([0, 1, 5, 4], 'magenta'),
    ([8, 11, 10, 9], 'red'),
    ([12, 13, 14, 15], 'cyan'),
    ([8, 12, 15, 11], 'green'),
    ([9, 10, 14, 13], 'blue'),
    ([11, 15, 14, 10], 'yellow'),
    ([8, 9, 13, 12], 'magenta')
]

def get_rotation_matrices(theta_x, theta_y):
    rot_x = np.array([
        [1, 0, 0],
        [0, math.cos(theta_x), -math.sin(theta_x)],
        [0, math.sin(theta_x), math.cos(theta_x)]
    ])
    rot_y = np.array([
        [math.cos(theta_y), 0, math.sin(theta_y)],
        [0, 1, 0],
        [-math.sin(theta_y), 0, math.cos(theta_y)]
    ])
    return rot_x, rot_y

def clip_triangle_against_plane(p1, p2, p3):
    """
    Slices a triangle against the Near Plane (z = z_near).
    Returns a list of triangles (0, 1, or 2) that are fully visible.
    """
    z_near = 0.5
    
    def intersect_plane(start, end):
        t = (z_near - start[2]) / (end[2] - start[2])
        return start + t * (end - start)
    
    output_verts = []
    input_poly = [p1, p2, p3]
    
    for i in range(3):
        curr_p = input_poly[i]
        prev_p = input_poly[(i - 1) % 3]
        
        curr_in = curr_p[2] >= z_near
        prev_in = prev_p[2] >= z_near
        
        if curr_in:
            if not prev_in:
                output_verts.append(intersect_plane(prev_p, curr_p))
            output_verts.append(curr_p)
        elif prev_in:
            output_verts.append(intersect_plane(prev_p, curr_p))
    
    result_triangles = []
    if len(output_verts) == 3:
        result_triangles.append(output_verts)
    elif len(output_verts) == 4:
        result_triangles.append([output_verts[0], output_verts[1], output_verts[2]])
        result_triangles.append([output_verts[0], output_verts[2], output_verts[3]])
        
    return result_triangles

def render_frame(theta_x, theta_y, cam_pos, cam_yaw, fps, pause_time, fig):
    if not plt.fignum_exists(fig.number): return
    plt.clf()

    rx_model, ry_model = get_rotation_matrices(math.radians(theta_x), math.radians(theta_y))
    _, ry_view = get_rotation_matrices(0, math.radians(-cam_yaw)) 
    
    for v in vertices:
        world_v = rx_model @ (ry_model @ v)
        translated_v = world_v - cam_pos
        view_v = ry_view @ translated_v
        transformed_vertices.append(view_v)
    transformed_vertices = np.array(transformed_vertices)

    faces_to_draw = []

    for face_indices, color in faces:
        face_verts_3d = transformed_vertices[face_indices]
        
        tri_1 = [face_verts_3d[0], face_verts_3d[1], face_verts_3d[2]]
        tri_2 = [face_verts_3d[0], face_verts_3d[2], face_verts_3d[3]]
        
        initial_tris = [tri_1, tri_2]
        
        for tri in initial_tris:
            A, B, C = tri[0], tri[1], tri[2]
            normal = np.cross(B - A, C - A)
            
            if normal[2] < 0: 
                
                clipped_tris = clip_triangle_against_plane(tri[0], tri[1], tri[2])
                
                for tri_points in clipped_tris:
                    face_2d = []
                    z_values = [] 
                    
                    for x, y, z in tri_points:
                        focal_length = 200
                        if z < 0.01: z = 0.01 
                        
                        x_proj = (x / z) * focal_length
                        y_proj = (y / z) * focal_length
                        face_2d.append([x_proj, y_proj])
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
    
    info = (f"Pos: {cam_pos}\nCam angle: {cam_yaw}°")
    plt.text(-95, 90, info, fontsize=10, family='monospace')
    plt.pause(pause_time)

if __name__ == "__main__":
    config, pause_duration = parameters_manager()
    running = True
    angle_x, angle_y = 0, 0
    camera_pos = np.array([0.0, 0.0, -4.0]) 
    camera_yaw = 0 
    
    fig = plt.figure(figsize=(5,5))
    
    def on_close(event):
        global running
        running = False
        print("Window closed.")

    fig.canvas.mpl_connect('close_event', on_close)
    fig.canvas.mpl_connect('key_press_event', input_manager)
    
    print("Controls: Arrows, ZQSD/WASD, A/E (Turn)")

    while running:
        render_frame(angle_x, angle_y, camera_pos, camera_yaw, config.fps, pause_duration, fig)