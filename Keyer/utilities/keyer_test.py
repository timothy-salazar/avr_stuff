import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import sys 
import random
import itertools

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
    combos = [i for i in itertools.product([0,1,2],repeat=5)][1:]
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
        'pinky':{
            0: None,
            1: Rectangle((.1, .1), .1, .18),   # pinky key 1
            2: Rectangle((.1, .32), .1, .15),  # pinky 1 key 2
            },
        'ring_finger':{
            0: None,
            1: Rectangle((.22, .2), .1, .18),  # ring finger, key 1
            2: Rectangle((.22, .42), .1, .15), # ring finger, key 2
            },
        'middle_finger':{
            0: None,
            1: Rectangle((.34, .25), .1, .18), # middle finger, key 1
            2: Rectangle((.34, .47), .1, .15), # middle finger, key 2
        },
        'pointer_finger':{
            0: None,
            1: Rectangle((.46, .22), .1, .18), # pointer finger, key 1
            2: Rectangle((.46, .44), .1, .15), # pointer finger, key 2
        },
        'thumb':{
            0: None,
            1: Rectangle((.6, .13), .1, .12),  # thumb, key 1
            2: Rectangle((.67, .3), .1, .12),  # thumb, key 2
        }
    }
    return key_dict

def on_press(event):
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'x':
        print('Boop!')

fig, ax = plt.subplots()
cid = ax.figure.canvas.mpl_connect('key_press_event', on_press)

keys = [
    Rectangle((.1, .1), .1, .18),   # pinky key 1
    Rectangle((.1, .32), .1, .15),  # pinky 1 key 2
    Rectangle((.22, .2), .1, .18),  # ring finger, key 1
    Rectangle((.22, .42), .1, .15), # ring finger, key 2
    Rectangle((.34, .25), .1, .18), # middle finger, key 1
    Rectangle((.34, .47), .1, .15), # middle finger, key 2
    Rectangle((.46, .22), .1, .18), # pointer finger, key 1
    Rectangle((.46, .44), .1, .15), # pointer finger, key 2
    Rectangle((.6, .13), .1, .12),  # thumb, key 1
    Rectangle((.67, .3), .1, .12),  # thumb, key 2
]
for key in keys:
    ax.add_patch(key)
plt.show()