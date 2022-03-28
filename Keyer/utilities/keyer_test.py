""" Program for producing keyer states, displaying them to the user, and then
measuring the amount of time until the user presses and releases the keys shown.
These will then be saved somewhere.
"""
import sys
import random
import itertools
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from datetime import datetime
import time


def state_generator():
    """ Input:
            None
        Output:
            The state of the keyer can be represented by the state of each
            of its physical keys.
            There are two keys per finger, which means that each finger can
            cause one of three states to occur:
                - key 1 depressed
                - key 2 depressed
                - neither key depressed
            (if both key 1 and key 2 are depressed, we assume that key 1 was
            accidentally pressed at the same time as key 2, and it's treated the same
            as if just key 2 is pressed)

            This means that the state of the keyer can be represented by 5 numbers,
            each with a value of 0,1, or 2.

            We want to get the state of all the keys, but we want to do it in a
            way that covers the different valid key combinations in a more or less
            uniform way, so this will act as a generator which produces each of the 242
            key combinations (0,0,0,0,0 isn't very useful, so we'll skip it) in a random
            order before beginning again.
            Note that the generator will continue generating states indefinitely.
    """
    combos = [i for i in itertools.product([0, 1, 2], repeat=5)][1:]
    random.shuffle(combos)
    while True:
        for keyer_state in combos:
            yield keyer_state
        random.shuffle(combos)

def get_key_dict():
    """ Input:
            None
        Output:
            A dictionary whose keys are fingers.
            The values are a dictionary of rectangles which represent
            the two physical keys of the keyer, as well as "none" which
            we use to represent the state in which neither key is pressed.
    """
    key_dict = {
        0:{
            0: None,
            1: Rectangle((.1, .1), .1, .18),   # pinky key 1
            2: Rectangle((.1, .32), .1, .15),  # pinky key 2
            },
        1:{
            0: None,
            1: Rectangle((.22, .2), .1, .18),  # ring finger, key 1
            2: Rectangle((.22, .42), .1, .15), # ring finger, key 2
            },
        2:{
            0: None,
            1: Rectangle((.34, .25), .1, .18), # middle finger, key 1
            2: Rectangle((.34, .47), .1, .15), # middle finger, key 2
        },
        3:{
            0: None,
            1: Rectangle((.46, .22), .1, .18), # pointer finger, key 1
            2: Rectangle((.46, .44), .1, .15), # pointer finger, key 2
        },
        4:{
            0: None,
            1: Rectangle((.6, .13), .1, .12),  # thumb, key 1
            2: Rectangle((.67, .3), .1, .12),  # thumb, key 2
        }
    }
    return key_dict


def display_keyer_state(ax, key_dict, keyer_state):
    """ Input:
            key_dict: dict.
            keyer_state: tuple of ints. Each position represents a finger (see notes in
                the "get_key_dict" function), and the number in that position represents 
                which keys (if any) that finger should press.

    """
    for finger in range(5):
        for key in range(1, 3):
            key_dict[finger][key].set_facecolor('gray')
        active_key = keyer_state[finger]
        if active_key:
            key_dict[finger][active_key].set_facecolor('lime')
    ax.figure.canvas.draw()

def draw_keyer(ax):
    """ Input:
            ax: MatPlotLib axes
        Output:
            key_dict: a dict of dicts. keys are ints representing the finger and then the
                physical switch. values are MatPlotLib Rectangle patches.
                
    """
    key_dict = get_key_dict()
    for finger in range(5):
        for key in range(1, 3):
            ax.add_patch(key_dict[finger][key])
    ax.figure.canvas.draw()
    return key_dict

def save_results(results, filepath='results.csv'):
    """ Input:
            results: dict - a dictionary representation of the results.
    """
    with open(filepath, 'a') as f:
        f.write(f"{results['state']},{results['misses']},{results['time']}\n")

def check_keyer_state(keyer_state, key_values):

    # Convert key_values to the same format as 
    # keyer_state 
    user_state = []
    for i in range(0,9,2):
        x = [int(j) for j in key_values[i:i+2]]
        if sum(x) == 2:
            user_state.append(2)
        elif x[0] == 1:
            user_state.append(1)
        elif x[1] == 1:
            user_state.append(2)
        else:
            user_state.append(0)
    print('-'*20)
    print(keyer_state)
    print(user_state)
    state_comp = [i==j for i,j in zip(keyer_state, user_state)]
    return all(state_comp)

class KeyTracker():
    def __init__(self):
        self.sg = state_generator()
        self.keyer_state = self.sg.__next__()
        self.misses = 0
        self.key_values = []

    def on_press(self, event):
        print('press', event.key)
        sys.stdout.flush()
        if event.key == 'x':
            print('Boop!')

        if event.key == 'enter':
            print(self.key_values)

            # Check to see if we have the correct keyer state
            if check_keyer_state(self.keyer_state, self.key_values):
                stats = {
                    "state":self.keyer_state,
                    "misses":self.misses,
                    "time": datetime.now() - start_time,
                }
                self.misses = 0
                save_results(stats)
                self.keyer_state = self.sg.__next__()
                display_keyer_state(ax, key_dict, self.keyer_state)
            else:
                self.misses += 1
                self.key_values.clear()
        else:
            self.key_values.append(event.key)

if __name__ == '__main__':

    # List representing the key presses that have been received
    # key_values = []
    # Integer tracking the number of attempts
    # misses = 0
    # The generator that produces keyer states for us. Will go through all
    # possible keyer states before repeating
    kv = KeyTracker()
    

    # def on_press(event, key_values=key_values, 
    #     misses=misses, keyer_state=keyer_state):
    #     """ Input:
    #             event: matplotlib key press event

    #     This function is called when the matplotlib backend detects a key press.
    #     When this happens it will add the key that was pressed to the key_values
    #     list. When we receive an "enter", we will compare the expected value to
    #     the input we received.

    #     If they match it means that the user successfully reproduced the displayed
    #     keyer state. We will save data about the attempt to a file so we can play
    #     around with it later

    #     If they do not match, the program will make note of he fact and will wait for
    #     another attempt.


    #     """
    #     print('press', event.key)
    #     sys.stdout.flush()
    #     if event.key == 'x':
    #         print('Boop!')

    #     if event.key == 'enter':
    #         print(key_values)

    #         # Check to see if we have the correct keyer state
    #         if check_keyer_state(keyer_state, key_values):
    #             stats = {
    #                 "state":keyer_state,
    #                 "misses":misses,
    #                 "time": datetime.now() - start_time,
    #             }
    #             misses = 0
    #             save_results(stats)
    #             keyer_state = sg.__next__()
    #             display_keyer_state(ax, key_dict, keyer_state)
    #         else:
    #             misses += 1
    #             key_values.clear()
    #     else:
    #         key_values.append(event.key)
    #     return key_values, misses, keyer_state


    fig, ax = plt.subplots()
    cid = ax.figure.canvas.mpl_connect('key_press_event', kv.on_press)
    key_dict = draw_keyer(ax)
    display_keyer_state(ax, key_dict, kv.keyer_state)
    start_time = datetime.now()
    plt.show()

