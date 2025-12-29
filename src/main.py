import matplotlib.pyplot as plt
import argparse

import state
import objects
import renderer
import inputs

def frequency_manager(fps):
    if fps <= 0: return 0.001
    return 1.0 / fps

def parameters_manager():
    parser = argparse.ArgumentParser(description="Custom 3D Engine")
    parser.add_argument('--fps', type=int, default=30, help="Target Frames Per Second")
    args = parser.parse_args()
    return args, frequency_manager(args.fps)

if __name__ == "__main__":
    config, pause_duration = parameters_manager()
    
    game_state = state.EngineState()
    
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

    fig.canvas.mpl_connect('close_event', on_close)
    fig.canvas.mpl_connect('key_press_event', lambda event: inputs.input_manager(event, game_state))
    
    print("Controls: Arrows, ZQSD/WASD, A/E (Turn)")

    while running:
        renderer.render_frame(game_state, objects.vertices, objects.faces, config.fps, pause_duration, fig)