import numpy as np
import math

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