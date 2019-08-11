# Absolem -- My 2D Endgame





## tl;dr

I've designed and built my own mechanical keyboard. It's fucking awesome! I'm going to ramble about it now, *in detail*. Read on if you're interested, or jump to the pictures, or post a comment to the reddit thread, or... you know... do whatever you fancy. I'm not your mom. :)





## Intro

- shitty Genius
- need backlighting to see keys at night
- MasterKeys Pro S
- rabbithole
- structure
- mention scrollspy and how it works





## Research

Being a researcher by trade, I can very much appreciate the need for seeing what someone has already done in order to not reinvent the wheel.
Also, following the old saying "stealing from one source is plagiarism; stealing from many is research", I have basically patched together the best parts of what the current state of the art has to offer. It was an interesting observation to make that almost all the best* ergo aspects came from different places, while their combination didn't exist yet. That's probably what lead to me deciding to design my own; had I found a board that checks all the boxes, I'd have just ordered that.
- (I'm also secretly happy that it didn't turn out like that, because this way I got to make my own, and it was a good chance to grow... but psst, don't tell that to anyone!)
- 

### General stuff

- Geekhack + Deskthority
- original post: https://geekhack.org/index.php?topic=95771.0
- then r/mk, of course
- algernon, DotDash32, Azel4231 shoutout in this phase

### Staggers

### The number of keys

### The pinky column

### The thumb region

- probably the only "really" original part of the design
- as the others are all seen _somewhere_ before
    - plagiarism vs. research
    - good artists steal, great artists 

### 3D aspects

### Switches

- feedback is good
- and if sound is important (which it is for me), then there is a really weak claim to endgame status for any "in-between" switches. You either want it to make sound (in which case you're probably one of those clicky fanatics with MX blues in a workplace board :P) or you don't (in which case you go for silent options).
- naturally I went for the latter, and 

### Keycaps

- the aesthetics are discussed below, the focus here is the profile of the caps, which greatly add to the overall feel and ergonomy.
- during my "awakening" I went from staring at high res photos and not seeing any difference other than color and symbols to being able to identify SA, DSA, XDA, OEM and Cherry by a cursory glance. This is probably not a very notable feat amongst the hardcore keeb folk, I just put it here to remind myself (and the esteemed reader) that it is easy to forget what is "obvious" and what isn't to an outsider...
- Anyways, I knew early on that I value sculpted sets, sort of as a compensation for not having curvature within the keywell.
- I mention the "feedback is good" principle here again, which also points to sculpted
- And from among the sculpted sets, I felt that lower is better than higher -- so SA was out pretty fast.
- Then came a research session from all over the net to see what people like/dislike, and once I factored in my priorities (mostly typing comfort, thereby largely eliminating gamer concerns) I found a pretty unanimous preference for the OG Cherry profile. And having tried it, I don't really see myself going for anything else. Cherry for the win, case closed.

### Actually doing something

- learning about electronics in general, wiring, soldering, etc
    - mention the ghosting fuckup
http://blog.komar.be/how-to-make-a-keyboard-the-matrix/
soldering tutorial




## Design

### The Polygon phase

- little cardboard example
- a working prototype: https://www.reddit.com/r/MechanicalKeyboards/comments/9aam0u/polygon_a_prototype_demo_a_roadmap_and_lots_of/
- a 3D model --> maybe still useful for someone
- a huge gap...

### Transitioning to 2D

- reconsidered aspects based on the prototyping experience
- no split
    - always ended up in the same position anyway
    - was a hassle to align every time
- no thumbplane + tenting
    - was not "bad" but didn't add much
    - hindered mobility, killed aesthetics
    - the most important angle was the _inward_, not the upward
- redesign with maker.js
- rough first draft --> 

### A visit from the "Low Profile Police"

- this is where I _think_ I'm done with 

### Etimology

### Logo ~~stealing~~ design





## Build

- tanárúr említés
- Brave-es lecke --> meg tudom csinálni!

### General aesthetics

- natural --> more like an instrument
- high profile
    - low profile only "viable" when milled
    - so a) contradicts wood, and
    - b) "slim" is rarely "robust"
- NO screws visible
- minimalist, clean
- quiet
- wireless

### Materials

And then a looot of waiting for everything to arrive.

### Prep

### Assembly

Most of the assembly consisted of waiting. Yes, really.
- only 3 legs for the controller --> improvisation is necessary :)

### Wiring

- battery fuckup
    - https://forums.adafruit.com/viewtopic.php?f=57&t=155069
    - reverse polarity...

### The finished product





## Firmware

- QMK on the protos
- looking into alternatives, not finding an "as is" match
    - just like with the physical keyboard itself
- writing own
- quick structure summary + point to separate readme
- fight with DIODE_DIRECTION
- huge fight with Arduino
    - include paths inside src
    - no code in the global scope
        - what do you mean that a variable name is not a type?!
    - https://forums.adafruit.com/viewtopic.php?f=57&t=155100
        - g++ linking is important!





## Keymap

### Base

### Symbols

### Navigation

### Misc

### Adjustment progress

- typing.com: 15 -> 30
- nitrotype.com: 30 -> 80 (so far)
- also using 10fastfingers.com, keybr.com (a lot), and www.speedcoder.net
- graph about the progress





## Writeup

I then decided that instead of writing a simple, short, and sweet reddit post about this, I'd write a more long-winded blog thingy to serve as the canonical home for the Absolem project.
In it, I divide the whole process into individual sections and talk about each topic in detail.
There's even a section in it which is about this writeup, in which I discuss why I wrote it, and how it consists of sections, one of which is about this exact writeup, in which... Whoops, infinite loop! `break;`





## Sleeve





## Club





## That's it, folks!

If you've seriously read all the way down here, then you're awesome! I hope I managed to keep it interesting. Now go and upvote the corresponding reddit post and star my github repo to earn an official, non-refundable, virtual high-five!

.
.
.

.
.
.

This is here to allow the top progress bar to reach completion :)