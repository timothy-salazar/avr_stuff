# AVR Stuff
This repo contains rough/in progress AVR projects.
I'm still learning a lot about proframming microcontrollers, so the code will be rough in a lot of places. That's how you learn though, and my github would be a ghost town if I just pushed up the code that was absolutely perfect. 

## Fade LED
I wanted to make a cool prop for a Dungeons and Dragons game I was running (yeah, I'm that kind of nerd). I thought it would be cool if I had a bottle filled with a liquid that pulsed with light. I had a glass bottle with a hollow, transluscent stopper. I also happened to have an ATtiny13a microcontroller, which was just small enough to fit inside the stopper. By filling the bottle with tonic water, and using the ATtiny13a to fade a UV LED on and off, I could make the bottle's contents glow with a blue-ish pulsing irridescence. 

This would have taken a lot longer to put together, but I was able to find a tutorial that covered pulsing a pin on an ATtiny using PWM. I modified the code a little bit, but for the most part the code was provided by:
[Tinysaur tutorial - Tutorial 002: Fading LED](https://tinusaur.org/tutorials/tutorial-002-fading-led-x1/)

## Keyer
This is an in-progress project. I'm trying to make a one-handed chorded keyboard that will use bluetooth to communicate with a computer. 
