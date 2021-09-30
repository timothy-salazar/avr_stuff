import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import sys 

def on_press(event):
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'x':
        print('Boop!')

fig, ax = plt.subplots()
cid = ax.figure.canvas.mpl_connect('key_press_event', on_press)

keys = [
    Rectangle((.1, .1), .1, .18), #Finger 1 key 1
    Rectangle((.1, .32), .1, .15), #Finger 1 key 2
    Rectangle((.22, .2), .1, .18),
    Rectangle((.22, .42), .1, .15),
    Rectangle((.34, .25), .1, .18),
    Rectangle((.34, .47), .1, .15),
    Rectangle((.46, .22), .1, .18),
    Rectangle((.46, .44), .1, .15),
    # Rectangle((.1, .1), .1, .18),
    # Rectangle((.1, .1), .1, .15),
]
for key in keys:
    ax.add_patch(key)
plt.show()