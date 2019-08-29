# Absolem -- My 2D Endgame


![Absolem Splash](https://zealot.hu/absolem/pics/splash.jpg)


## tl;dr

I've designed and built my own mechanical keyboard. It's fucking awesome! I'm going to ramble about it now, *in detail*. Read on if you're interested, or [jump to the pictures](#the-finished-product), or post a comment to [the reddit thread](), or... you know... do whatever you fancy. I'm not your mom. :)





## Table of Contents

- [Intro](#intro)
- [Research](#research)
- [Design](#design)
- [Build](#build)
- [Firmware](#firmware)
- [Keymap](#keymap)
- [Writeup](#writeup)
- [Future work](#future-work)
- ["Club"](#club)





## Intro

Until about one and a half years ago, I'd been happily typing on a [shitty Genius](https://www.cnet.com/products/genius-slimstar-i220-keyboard-series/) with a QWERTZ (Hungarian QWERTY) layout. Ah... simpler times!

!["Ignorance is bliss"](https://zealot.hu/absolem/pics/ignorance_is_bliss_matrix.gif)

I was hovering at about 50-60 wpm, which -- while decidedly not blazing fast -- didn't bother me much. I also didn't really care that my typing "technique" involved around 4-6 fingers and a lot of looking at the keyboard. What _did_ bother me was nights when I couldn't actually see the keys and it slowed me down quite a bit.

If you're thinking that my solution was learning to touch type, you're wrong! (for now...) I, of course, decided that I needed a backlit keyboard. Around this time I was vaguely aware of mechanical keyboards and the "supposed" superior typing experience they provide. So to celebrate my dissertation defense, -- and after a cursory glance at full size vs. TKL arguments -- I treated myself to a [MasterKeys Pro S](https://www.coolermaster.com/catalog/peripheral/keyboards/masterkeys-pro-s-white/). And that's where the problems started...

To be fair, the Pro S is a fine keyboard. But it's not even the thing that convinced me about mechs. By the time it arrived, the geekhack-deskthority-r/mk Bermuda triangle sucked me in, and I was already too deep in the rabbit hole. Looking back now, I think the main cause was that I started looking at the topic as genuine "research", and in my mildly fanatic<a href="#footnote-1"><sup>1</sup></a> worldview that could only end with another "dissertation", which is what this post is, I guess.





## Research

Being a researcher by trade, I can very much appreciate the need for seeing what someone has already done in order to not reinvent the wheel.
Also, following the old saying "stealing from one source is plagiarism; stealing from many is research", I have basically patched together the (imho) best parts of what the current state of the art has to offer. It was an interesting observation to make that almost all the best<a href="#footnote-2"><sup>2</sup></a> ergo aspects came from different places, while their combination didn't exist yet. That's probably what lead to me deciding to design my own; had I found a board that checks all the boxes, I'd have just ordered that.
(I'm also secretly happy that it didn't turn out like that, because this way I got to make my own, and it was a good chance to grow... but psst, don't tell that to anyone!)

So with that in mind, let me just quickly walk you through the steps that lead me to "keeb enlightenment". Disclaimer, though: I'm only going to mention most concepts briefly, which can serve as good starting points to begin your own deeper research if you're interested.
 

### General stuff

Okay, basics first, if you're interested in the topic, you should browse [GeekHack](https://geekhack.org/), [Deskthority](https://deskthority.net/), and (of course) [r/mk](https://www.reddit.com/r/MechanicalKeyboards/). (As an example, [here](https://geekhack.org/index.php?topic=95771.0) are my tentative first steps in a brave new world over on Geekhack).

These places are not only chock full of information, ideas, and inspiration; they also house a very helpful and supportive community.
This is also where I'd like to thank a few people for their general help in this project, namely:

- [algernon](https://geekhack.org/index.php?action=profile;u=55020), fellow Hungarian keeb expert, for all the early advice,
- [Azel4231](https://feierabendprojekte.wordpress.com/2018/03/21/building-a-keyboard-by-hand/), for his help in switch layout related measurements (I'm the "redditor" from the addendum...),
- and, naturally, [DotDash32](), for the metric shitton of discussion we've done in both posts and reddit messages that really helped me shape what I should aim for.


### Staggers

What became really clear early on is that row-staggers are evil. The reason they exist is pure path dependence, and they should be eradicated. For me, this is an issue that's been a non-issue for a long long time, but now it's impossible to unsee... I'm not really a comment-y kind of guy, but you can sometimes find me in the [middle of an argument](https://www.reddit.com/r/MechanicalKeyboards/comments/c8njw4/ergonomic/esr6nab/?context=8&depth=9) under reddit posts that claim some connection to ergonomics, yet still retain the row-stagger.

I'm much more "lenient" towards ortho (a.k.a. grid, or matrix) layouts, but the clear winner of this aspect (for me) is column-staggered boards. Let's give future aliens a chance to figure out how we looked like!

![Nerd joke](https://zealot.hu/absolem/pics/nerd_joke.jpg)

As an example, compare a ["traditional" TKL (or 60%)](http://www.vortexgear.tw/vortex2_2.asp?kind=47&kind2=220&kind3=&kind4=997) vs. a [Planck](https://olkb.com/planck) vs. an [Atreus](https://atreus.technomancy.us/).


### The number of keys

Today's full size (and beyond) keyboards come from the assumption that there should be the same number of keys as there are desired functionalities and we should make our hands conform to the resulting layout. I, on the other hand, think that the inverse of this is true, namely that we should make the number of the keys match what's comfortably reachable from the home position and make the desired functionalities conform to that.

This leads to both touch typing (which has many more benefits I'm not going to discuss here) and to a need to significant decrease the number of keys. On the other extreme of the spectrum is [stenography](http://plover.stenoknight.com/), but even if we remain firmly in the realm of letter-based typing, we can (and should) make do following the "at most 1 key distance from home" principle. That leaves at most 6 &times; 3 keywells + 3 thumb keys per hand. I'd argue that anything more than that is bad. (Not only "unnecessary" or "wasteful", mind you, but actually bad. As in, it could be better with less...)

The natural result of a small number of keys while still wanting a large number of functions is the use of layers. And layers -- especially if combined with custom programmability, more on that later -- are the "next best thing"! Nevertheless, I encounter many posts that criticize the overuse of layers, or posts that express confusion about how a 40% keyboard can still be practical. I'd refer the former group to their shift keys and the notable lack of dedicated capital keys on their boards, while the latter group should take a look at their phone while writing a text and tell me again how a really small keyboard is unusable.

Anyways, I digress... The point is that I came to the conclusion that there should be very few keys with heavy layering support. As an example, consider traditional keyboards vs. a [corne](https://github.com/foostan/crkbd).


### The pinky column

Like we saw above, the pinky often gets 2 columns (similarly to the index finger) even when conforming to the "1 distance from home" (1DFH) rule; and a much more when not. What I've found is that a) it's unnecessary with a sufficiently clever keymap and a lo(oooo)t of practice, and b) it _should_ be avoided to spare your weakest fingers however you can. So I've adopted a further restriction over the 1DFH to limit myself to 5 &times; 3 keys per hand (plus the thumbs, of course).

Regarding the physical layout of the pinky keys, my experiments (and my eyes, when looking at my hand) showed that the pinky can use a little bit of separation from the others. This _could_ theoretically apply to the ring and middle fingers, too, but I didn't feel the need in those cases. However, it really shouldn't apply for the index finger, which already has an extra column to take care of, like in the case of the [Sector](https://github.com/omkbd/Sector).

As an example of pinky overworking, consider any regular layout (or even the [Ergodox](https://ergodox-ez.com)) vs. the [Minidox](https://geekhack.org/index.php?topic=89951.0) (of which my design is basically a slightly refined, glued together, and wireless-ized version). As for an illustration of the pinky angle, take a look at [this crazy thing](https://zealot.hu/absolem/pics/ergowrap.jpg) (I believe called ergowrap?).


### The thumb region

- probably the only "really" original part of the design
- as the others are all seen _somewhere_ before
    - plagiarism vs. research
    - good artists steal, great artists 

### The angle between the halves

- none is objectively bad
- splits solve this by relegating it to the user
- one piece boards should definitely add some

### 3D aspects

- tenting
- keywell curvature

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





## Future work

- PCB
- top layer gap for thumb
- sleeve
- firmware improvements





## "Club"





## That's it, folks!

If you've seriously read all the way down here, then you're awesome! I hope I managed to keep it interesting. Now go and upvote the [corresponding reddit post]() and star my [GitHub repo](https://github.com/mrzealot/absolem) to earn an official, non-refundable, virtual high-five!


<br />
<br />
<hr />

__Acknowledgment__: Besides the people already mentioned it the post, I'd like to thank my wife, who put up with me during this year-long process, and whose first reactions weren't "You're doing WHAT?!" and "How much?!" :)

__Disclaimer__: This post contains lots of pictures and references that are obviosly not my work, and I'm not claiming that they are. GIFs and jokes come from a quick Google searches, and I'm not going to put a "source: something..." under each one. Use your best judgement.

<sup id="footnote-1">1</sup>: notice how my nick is "zealot", which is just a cooler name for fanatic. (I reject the religious connotation, though.)

<sup id="footnote-2">2</sup>: "best" is arguable here, I admit. But, despite my every effort to keep this as objective as possible, the whole topic is somewhat subjective, so bear with me.