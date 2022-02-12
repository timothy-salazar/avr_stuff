""" This demonstrates how to set up matplotlib to take user input.
This is a very simple toy demo, displaying a blank canvas and then printing
any keys the user presses to stdout. 
"""
import sys
import matplotlib.pyplot as plt

def on_press(event):
    """ Input: 
            KeyEvent: 
                see https://matplotlib.org/stable/api/backend_bases_api.html#matplotlib.backend_bases.KeyEvent
        Output:
            prints any keys pressed by the user.
    """
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'x':
        print('Boop!')

if __name__ == "__main__":

    fig, ax = plt.subplots()
    cid = ax.figure.canvas.mpl_connect('key_press_event', on_press)
    plt.show()
