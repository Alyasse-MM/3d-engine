import matplotlib.pyplot as plt
import argparse
import math
import numpy as np

# Import our custom modules
import state
import data
import renderer

# --- Configuration Helpers ---
def frequency_manager(fps):
    if fps <= 0: return 0.001
    return 1.0 / fps

def parameters_manager():
    parser = argparse.ArgumentParser(description="Custom 3D Engine")
    parser.add_argument('--fps', type=int, default=30, help="Target Frames Per Second")
    args = parser.parse_args()
    return args, frequency_manager(args.fps)

# --- Input Manager ---
# Now accepts the 'game_state' as an argument
def input_manager(event, game_state):
    rot_step = 5
    move_step = 0.5
    cam_rot_step = 5
    
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

# --- Main Execution ---
if __name__ == "__main__":
    # 1. Setup
    config, pause_duration = parameters_manager()
    
    # 2. Initialize State
    game_state = state.EngineState()
    
    # 3. Setup Plot
    try:
        plt.rcParams['keymap.save'].remove('s')
        plt.rcParams['keymap.quit'].remove('q')
        if 'a' in plt.rcParams['keymap.all']:
            plt.rcParams['keymap.all'].remove('a')
    except:
        pass

    fig = plt.figure(figsize=(5,5))
    running = True

    def on_close(event):
        global running
        running = False
        print("Window closed.")

    # We use a lambda function here to pass 'game_state' to the input manager
    fig.canvas.mpl_connect('close_event', on_close)
    fig.canvas.mpl_connect('key_press_event', lambda event: input_manager(event, game_state))
    
    print("Controls: Arrows, ZQSD/WASD, A/E (Turn)")

    # 4. Loop
    while running:
        renderer.render_frame(game_state, data.vertices, data.faces, config.fps, pause_duration, fig)