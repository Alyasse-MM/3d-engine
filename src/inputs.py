import math

def input_manager(event, game_state):
    rot_step = 1
    move_step = 0.5
    cam_rot_step = 1
    
    rad = math.radians(game_state.camera_yaw)
    sin_a = math.sin(rad)
    cos_a = math.cos(rad)
    
    if event.key == 'left':
        game_state.angle_y = (game_state.angle_y + rot_step) % 360
    elif event.key == 'right':
        game_state.angle_y = (game_state.angle_y - rot_step) % 360
    elif event.key == 'up':
        game_state.angle_x = (game_state.angle_x + rot_step) % 360
    elif event.key == 'down':
        game_state.angle_x = (game_state.angle_x - rot_step) % 360
        
    elif event.key == 'a':
        game_state.camera_yaw = (game_state.camera_yaw - cam_rot_step) % 360
    elif event.key == 'e':
        game_state.camera_yaw = (game_state.camera_yaw + cam_rot_step) % 360

    elif event.key == 'z' or event.key == 'w': 
        game_state.camera_pos[0] += sin_a * move_step
        game_state.camera_pos[2] += cos_a * move_step
    elif event.key == 's':
        game_state.camera_pos[0] -= sin_a * move_step
        game_state.camera_pos[2] -= cos_a * move_step
    elif event.key == 'q': 
        game_state.camera_pos[0] -= cos_a * move_step
        game_state.camera_pos[2] += sin_a * move_step
    elif event.key == 'd':
        game_state.camera_pos[0] += cos_a * move_step
        game_state.camera_pos[2] -= sin_a * move_step