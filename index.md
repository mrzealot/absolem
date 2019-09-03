# Absolem -- My 2D Endgame


![Absolem Splash](https://zealot.hu/absolem/pics/splash.jpg)


## tl;dr

I've designed and built my own mechanical keyboard.
It's fucking awesome!
I'm going to ramble about it now, *in detail*.
Read on if you're interested, or [jump to the pictures](#the-finished-product), or post a comment to [the reddit thread](), or... you know... do whatever you fancy.
I'm not your mom. :)





## Contents

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

Until about one and a half years ago, I'd been happily typing on a [shitty Genius](https://www.cnet.com/products/genius-slimstar-i220-keyboard-series/) with a QWERTZ (Hungarian QWERTY) layout.
Ah... simpler times!

!["Ignorance is bliss"](https://zealot.hu/absolem/pics/ignorance_is_bliss_matrix.gif)

I was hovering at about 50-60 wpm, which -- while decidedly not blazing fast -- didn't bother me much.
I also didn't really care that my typing "technique" involved around 4-6 fingers and a lot of looking at the keyboard.
What _did_ bother me was nights when I couldn't actually see the keys and it slowed me down quite a bit.

If you're thinking that my solution was learning to touch type, you're wrong! (for now...)
I, of course, decided that I needed a backlit keyboard.
Around this time I was vaguely aware of mechanical keyboards and the "supposed" superior typing experience they provide.
So to celebrate my dissertation defense, -- and after a cursory glance at full size vs. TKL arguments -- I treated myself to a [MasterKeys Pro S](https://www.coolermaster.com/catalog/peripheral/keyboards/masterkeys-pro-s-white/).
And that's where the problems started...

To be fair, the Pro S is a fine keyboard.
But it's not even the thing that convinced me about mechs.
By the time it arrived, the geekhack-deskthority-r/mk Bermuda triangle sucked me in, and I was already too deep.
Looking back now, I think the main cause was that I started looking at the topic as genuine "research", and in my mildly fanatic<a href="#footnote-1"><sup>1</sup></a> worldview that could only end with another "dissertation", which is what this post is, I guess.

So, strap in as I rant about the whole journey that led me here.
Also, fair warning that I'm writing this on the already finished Absolem, which is just a pleasure to type on, so I'm going to be verbose! :P





## Research

Being a researcher by trade, I can very much appreciate the need for seeing what someone has already done in order to not reinvent the wheel.
Also, following the old saying "stealing from one source is plagiarism; stealing from many is research", I have basically patched together the (imho) best parts of what the current state of the art has to offer.
It was an interesting observation to make that almost all the best<a href="#footnote-2"><sup>2</sup></a> ergo aspects came from different places, while their combination didn't exist yet.
That's probably what lead to me deciding to design my own; had I found a board that checks all the boxes, I'd have just ordered that.
(I'm also secretly happy that it didn't turn out like that, because this way I got to make my own, and it was a good chance to grow... but psst, don't tell that to anyone!)

So with that in mind, let me just quickly walk you through the steps that lead me to "keeb enlightenment".
Disclaimer, though: I'm only going to mention most concepts briefly to keep the post's length manageable, but it can hopefully serve as a good starting point to begin your own, deeper research if you want.
 

### General stuff

Okay, basics first, if you're interested in the topic, you should browse [GeekHack](https://geekhack.org/), [Deskthority](https://deskthority.net/), and (of course) [r/mk](https://www.reddit.com/r/MechanicalKeyboards/).
(As an example, [here](https://geekhack.org/index.php?topic=95771.0) are my tentative first steps in a brave new world over on Geekhack).

These places are not only chock full of information, ideas, and inspiration; they also house a very helpful and supportive community.
This is also where I'd like to thank a few people for their general help in this project, namely:

- [algernon](https://geekhack.org/index.php?action=profile;u=55020), fellow Hungarian keeb expert, for all the early advice,
- [Azel4231](https://feierabendprojekte.wordpress.com/2018/03/21/building-a-keyboard-by-hand/), for his help in switch layout related measurements (I'm the "redditor" from the addendum...),
- and, naturally, [DotDash32](https://www.reddit.com/user/Dotdash32/), for the metric shitton of discussion we've done in both posts and reddit messages that really helped me shape what I should aim for.


### Staggers

What became really clear early on is that row-staggers are evil.
The reason they exist is pure path dependence (we've always done it like this, let's keep doing it like this), and they should be eradicated.
For me, this is an issue that's been a non-issue for a long long time, but now (that I'm "enlightened") it's impossible to unsee...
I mean, I sympathize with all the muscle memory that will be lost in a transition (I'm in the middle of one right now, after all), but that can't be a good enough reason not to switch!
I'm not really a comment-y kind of guy, but you can find even me sometimes in the [middle of an argument](https://www.reddit.com/r/MechanicalKeyboards/comments/c8njw4/ergonomic/esr6nab/?context=8&depth=9) under reddit posts that claim some connection to ergonomics, yet still retain the row-stagger.

I'm much more "lenient" towards ortho (a.k.a. grid, or matrix) layouts, but the clear winner of this aspect (for me) is column-staggered boards.
Let's give future aliens a chance to figure out how we looked like!

![Nerd joke](https://zealot.hu/absolem/pics/nerd_joke.jpg)

What's more, I'm very much in favour of an "agressive" stagger.
Many boards started in the right direction, but few went far enough, so I've been planning to be a little heavier-handed in the stagger department from the start.

As an example, compare a ["traditional" TKL (or 60%)](http://www.vortexgear.tw/vortex2_2.asp?kind=47&kind2=220&kind3=&kind4=997) vs. a [Planck](https://olkb.com/planck) vs. an [Atreus](https://atreus.technomancy.us/).


### The number of keys

Today's full size (and beyond) keyboards come from the assumption that there should be the same number of keys as there are desired functionalities and we should make our hands conform to the resulting layout.
I, on the other hand, think that the inverse of this is true, namely that we should make the number of the keys match what's comfortably reachable from the home position and make the desired functionalities conform to that.

This leads to a) touch typing -- or at least a strictly enforced finger-key relationship (which has many more benefits I'm not going to discuss here) and b) to the need to significantly decrease the number of keys.
On the other extreme of the spectrum is [stenography](http://plover.stenoknight.com/), but even if we remain firmly within the realm of letter-based typing, we can (and should) make do following the "at most 1 key distance from home" principle.
That leaves at most 6 &times; 3 keywells + 3 thumb keys per hand.
I'd argue that anything more than that is bad.
(Not only "unnecessary" or "wasteful", mind you, but actually bad. As in, it could be better with less...)

The natural result of a small number of keys while still wanting a large number of functions is the use of layers.
And layers -- especially if combined with custom programmability, more on that later -- are the "bees knees"!
Nevertheless, I encounter many posts that criticize the overuse of layers, or posts that express confusion about how a 40% keyboard can still be practical.
I'd refer the former group to their shift keys and the notable lack of dedicated capital letters on their boards, while the latter group should take a look at their phone while writing a text and tell me again how a really small keyboard is unusable.

Anyways, I digress...
The point is that I came to the conclusion that there should be very few keys with heavy layering support.
As an example, consider traditional keyboards vs. a [corne](https://github.com/foostan/crkbd).


### The pinky column

Like we saw above, the pinky often gets 2 columns (similarly to the index finger) even when conforming to the "1 distance from home" (1DFH) rule; and much more when not.
What I've found is that a) it's unnecessary with a sufficiently clever keymap and a lo(oooo)t of practice, and b) it _should_ be avoided to spare your weakest fingers however you can.
So I've adopted a further restriction over the 1DFH to limit myself to 5 &times; 3 keys per hand (plus the thumbs, of course).

Regarding the physical layout of the pinky keys, my experiments (and my eyes, when looking at my hand) showed that the pinky can use a little bit of separation from the others.
This _could_ theoretically apply to the ring and middle fingers, too, but I didn't feel the need in those cases.
However, it really shouldn't apply for the index finger, which already has an extra column to take care of, like in the case of the [Sector](https://github.com/omkbd/Sector).

As an example of pinky overworking, consider any regular layout (or even the [Ergodox](https://ergodox-ez.com)) vs. the [Minidox](https://geekhack.org/index.php?topic=89951.0) (of which my design is basically a slightly refined, glued together, and wireless-ized version). As for an illustration of the pinky angle, take a look at [this crazy thing](https://zealot.hu/absolem/pics/ergowrap.jpg) (I believe called ergowrap?).


### The thumb region

Generally, there are three approaches for the thumbs:

1. SPACEBAR!!! -- one of the thumbs can keep hacking away on a button that takes up 6-7 spaces, while the other just exists. Very efficient... &lt;/sarcasm&gt; There are more sophisticated layouts, with split spacebars and the like, but from an ergonomical standpoint, these are all subpar compared to the next two.
2. Clusters -- consider the [Ergodox](https://ergodox-ez.com) again. While this way is definitely better than a single spacebar, in my opinion it overcompensates with the amount of work it tries to give to the thumbs. The side effect of this is that very few of those thumb keys are actually convenient (or usable, according to some). This leads us nicely to:
3. Fans -- consider the [KeyboardIO Model 01](https://shop.keyboard.io/). This approach appreciates that the thumb actually moves in an arc, and doesn't try to add extra functionality either above or below it.

This is probably the only "really" original part of the Absolem design, as all the other stuff I've mentioned so far could be seen _somewhere_ before.
And, depending on how we interpret "original", maybe not even this...
But: I actually placed the thumb keys on an arc, with a measured thumb radius.
Yes, I actually had to refresh my trigonometry for this!
And I, of course, followed the 1DFH principle, too, so there can only be 3 (unlike the KeyboardIO's 4).

I'd also like to mention, as someone with quite wide (read: fat) thumbs, I've aimed to have 1.25u thumb keys from the start, at least for the home position.
The sides can more easily be 1u because they don't have a neighbour on one side, so there's less chance for misclicking (mispressing?).
But the thumb home position (which is flanked by other thumb keys on both sides) definitely deserves to be bigger imho.


### The angle between the halves

Like we already established, our hands don't just sprout out from our chests.
The forearms (like the thumbs) move in an arc, if we consider the elbows fixed.
That means that the keywells for the hands should be in line with each other (parallel? co-linear?) only if they are shoulder width apart, which they rarely are.

So for a "normal" compact board, no angle is definitely bad for the wrists.
Splits solve this quite elegantly by completely relegating the issue to the user, but for non-splits there _should_ be an angle.
Additionally, my (very un-scientific and subjective) measurements indicate that that angle should be at least 20&deg; (so a little steeper than anything I've seen, apart from [crazy adjustable ones](https://www.youtube.com/watch?v=185AWX6_pHE)).

For illustration, consider traditional one-piece boards vs. splits like a [Let's Split](https://www.reddit.com/r/MechanicalKeyboards/comments/4v51co/lets_split_the_build_guide/) vs. angled one-piece ones like -- again -- an [Atreus](https://atreus.technomancy.us/).


### 3D aspects

What we haven't touched on yet, are 3D aspects like forward/backward tilting, inward/outward tenting, and concave curvature within the keywell.
The curvature was probably the first thing I abandoned because a) it prohibits a lot of manufacturing options that lead to an "heirloom-grade"<a href="#footnote-3"><sup>3</sup></a> keyboard, and b) it is really only useful for larger keywells anyway, which we've already ruled against.
As for tilting/tenting and separate thumb planes, I gave them a chance and then decided not to utilize them, as I'll talk about in the design section.

For examples of the concepts, see the [Dactyl-Manuform](https://geekhack.org/index.php?topic=88576.0) which conveniently has them all.


### Switches

To keep it simple (and to avoid straying into religious wars territory), all I'll say is that generally there are linear, tactile, and tactile+clicky switches, where the former two can be silent or regular.
Plus I wanted to stay within the bounds of the [MX standard](https://www.cherrymx.de/en/mx-original/mx-red.html) (mainly for keycap reasons), so if you're interested in more extreme switch choices, I'll have to disappoint...
A round of research on the interwebs suggested that linears are mostly for gaming, plus everyone seemed to agree that a tactile switch is better for typing purposes.
There's also this [speed typing guide](https://forum.colemak.com/topic/2455-vipers-speedtyping-guide/) that states that feedback is good!
So linears were out.

That leaves silent tactile, regular tactile, and clicky.
Now, if sound is important (which it is for me), then there is a really weak claim to endgame status for any regular "in-between" switches.
You either want it to make sound (in which case you're probably one of those clicky fanatics with MX blues in a workplace board :P) or you don't (in which case you go for silent options).

You might have deduced from the wording of that last sentence that I went for the latter, and if you constrain your search to silent tactile switches, you'll inevitably run into [Zilents](https://zealpc.net/products/zilents).
There are cheaper alternatives, but since this was an endgame build for me, I went with the Zilents, and they are absolutely worth it.
People might chalk all the rave Zealio/Zilent reviews up to choice-supportive bias, but I know I love them, so there's that.

Another thing to note here is that I've seen very few boards out there that utilize variable spring weights for their switches.
However, if we hold ourselves to the notion that each key has its dedicated finger, we could account for the different finger strengths by the different switch weights.
So that's what I did.
Luckily I didn't even have to take apart and modify any switches manually, as Zilents come in four flavours.
See my ergo finger-aware weight layout in the [Assembly section](#assembly).


### Keycaps

The aesthetics are discussed below; the focus here is the profile of the caps, which greatly add to the overall feel and ergonomy.

Side story: during my "awakening", I went from staring at high resolution photos and not seeing any difference other than color and symbols, to being able to identify SA, DSA, XDA, OEM and Cherry by a cursory glance.
This is probably not a very notable feat amongst the hardcore keeb folk, I just put it here to remind myself (and the esteemed reader) that it is easy to forget what is "obvious" and what isn't to an outsider...

Anyways, I knew early on that I value sculpted sets, sort of as a compensation for not having curvature within the keywell.
Plus I'd like to mention the "feedback is good" principle here again, which also points to sculpted.
And from among the sculpted sets, I felt that lower is better than higher -- so SA was out pretty fast.

Then came a research session from all over the net to see what people like/dislike, and once I factored in my priorities (mostly typing comfort, thereby largely eliminating gamer concerns) I found a pretty unanimous preference for the OG Cherry profile.
Now I have to note here that I therefore don't have personal experience with other profiles, just the stock OEMs of the Pro S and then the Cherries.
But having tried them, I don't really see myself going for anything else.
Cherry for the win, case closed.


### Actually doing something

The research section cannot be complete without mentioning how much reading and browsing I had to do in order to at least have a superficial grasp on how _anything_ works within the field of electronics in general.
I mean I haven't held a soldering iron in my hand my whole life.
And that doesn't even take it into account that I've always kinda sucked at anything DIY<a href="#footnote-4"><sup>4</sup></a>.

But that's in the past now, and a PhD (plus a short but very intense job at a cutting edge tech firm) quickly teaches you that you're capable of much more than you initially think.
So even if you feel as hopeless as I did at the beginning, give it a go!
Read/watch a few Sparkfun/Adafruit tutorials!
Learn about [keyboard matrices](http://blog.komar.be/how-to-make-a-keyboard-the-matrix/), then [fuck it up](https://www.reddit.com/r/MechanicalKeyboards/comments/95o35o/diodes_in_series_am_i_an_idiot/) regardless...
Then try again!
It's gonna be worth it in the end.





## Design

With all this very important (&lt;/yawn&gt;) knowledge collected, it was time to design "the endgame".

### The Polygon phase

I started out with a little cardboard example (fig. 1) to get a feel for a different thumb plane.
Note that the main keywell layout was already identical to the final product -- that was never in question.
It then grew into a working prototype which I've already posted about [here](https://www.reddit.com/r/MechanicalKeyboards/comments/9aam0u/polygon_a_prototype_demo_a_roadmap_and_lots_of/) (fig. 2-3).
I was generally satisfied with it, so I went ahead and coded a Python &rarr; OpenSCAD model, too (fig. 4-5)<a href="#footnote-5"><sup>5</sup></a>.

Here came a huge gap where I did not have the luxury to retrain myself with the new board/layout (or to think a whole lot about keyboards in general) as I constantly had to meet strict deadlines; with the good ol' QWERTZ.


### Transitioning to 2D

That "gap" ended this April, by which time I've reconsidered a few things due to a) the prototyping experience, b) the designs I kept seeing on occasional visits to reddit, and c) the notion and pursuit of minimalism that kept creeping into my life.

- reconsidered aspects based on the prototyping experience
- no split
    - always ended up in the same position anyway
    - was a hassle to align every time
- no thumbplane + tenting
    - was not "bad" but didn't add much
    - hindered mobility, killed aesthetics
    - the most important angle was the _inward_, not the upward
- redesign with maker.js
- rough first draft --> https://www.reddit.com/r/MechanicalKeyboards/comments/bpc54c/absolem_step_2_towards_world_domination/
    - the form here was almost completely dictated by the ergonomic layout of the keys themselves, with a small, automatic outline applied.
    - as I said in a comment (first half): "I'm totally unapologetic about the looks as the design is completely driven by ergonomics decisions..."

### Going fancy and wireless

- new controller footprint, plus battery accommodation
- incredible OCD phase with the corners
    - second half of the above quote: "The only minor things I have my "aesthetic influence" in is how the corners are rounded, how thick the margin is, how big the back opening is, etc."
    - well, these might sound minor indeed, but the amount of time I pissed away trying to make them look just right was nothing short of ridiculous.
- I also scheduled a complete rewrite to be as logical and "elegant" as I can make it (because why wouldn't I?)
- results, summarized in a quality shitpost: https://www.reddit.com/r/MechanicalKeyboards/comments/c5rrao/you_may_not_like_it_but_this_is_what_peak/

### A visit from the "Low Profile Police"

- this is where I _think_ I'm done with the design but then [DanL4](https://www.reddit.com/user/DanL4/) comes along

### Etimology

The name comes from the Tim Burton version of [Alice in Wonderland](https://en.wikipedia.org/wiki/Alice_in_Wonderland_(2010_film)), where they actually named the [Caterpillar](https://en.wikipedia.org/wiki/Caterpillar_(Alice%27s_Adventures_in_Wonderland)).
And connecting this keyboard project to the Caterpillar from Alice is pertinent for multiple reasons:

- the keyboard looks like a butterfly...

![D'uuuh](https://zealot.hu/absolem/pics/duh.gif)

- my all-time computer (and the in-progress environment I'm cultivating on it) is called Alice, and it's very fitting to have the "Absolem" blow letters to her over the air:

![Hookah smoke letters](https://zealot.hu/absolem/pics/smoke_letters.jpg)

- in the Tim Burton version (again), Absolem helped Alice & co. interpret the Oracle, which would make _me_ the Oracle in this analogy, and that's an ego-boost I can't pass up!

![Oracle](https://zealot.hu/absolem/pics/oracle.jpg)

- plus it also sounds really cool, even if I do say so myself.


### Logo ~~stealing~~ design





## Build

Ooookay, enough small talk, let's get our hands dirty!


### Aesthetics

With the ergonomic side of the design ready, it is time to discuss the specifics of building an actual, physical thing.
The general guidelines I followed can be summed up as:

- natural -- it should feel more like an instrument than a cheap peripheral
- high profile -- anything that is "slim" is rarely "robust"
- minimalistic -- so a clean, no frills look, no wires, and definitely NO visible screws
- quiet -- only the bare minimum noise that's required for ergonomic operation
- heirloom-grade<a href="#footnote-3"><sup>3</sup></a> -- a construction quality that would actually make it feasible to leave one of these to the next generation.

Translating the above to materials led to:

- 4 mm thick oak layers for the cover/undercover and the "belly",
- 1.5 and 1 mm thick stainless steel layers for the switch plate and the bottom cover, respectively, and
- 3 mm thick embossed neoprene for the anti-slip bottom and the internal sound insulation.


### Preparations

Getting all these required a lot of calls and messages to a lot of different people, and that doesn't even include having them cut according to the plans.
Oak boards of the desired thickness are only sold by one webshop in the whole country.
For the neoprene, I had to make a specific deal with a Chinese wholesale manufacturer to send me A4 sized "samples" (and where the shipping cost was a whole magnitude larger than the material).
As for lasercutting, a lot of companies don't take small, one-off orders so I had to shop around quite a bit to find two that accepted the metalwork and the wood/neoprene, respectively.

As for the other "ingredients", the Zilents came straight from the official store in Canada, and all the rest came from Aliexpress:

- [Blank black Cherry profile thick PBT keycaps](https://www.aliexpress.com/item/32750162619.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3)
- [Adafruit Feather nRF52 Bluefruit](https://www.aliexpress.com/item/32956053585.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3) 
- [Magnetic Micro USB connector](https://www.aliexpress.com/item/32856265666.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3)
- [1N4148 diodes](https://www.aliexpress.com/item/32729204179.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3)
- [24 AWG tinned solid core wire](https://www.aliexpress.com/item/32879288883.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3)
- [M2.5 button head Allen screws](https://www.aliexpress.com/item/33013007352.html?spm=a2g0s.9042311.0.0.27424c4dEGY0hq); 4 (controller), 6 (battery), and 14 mm (case)
- [M2.5 hex nuts](https://www.aliexpress.com/item/32959149109.html?spm=a2g0s.9042311.0.0.27424c4dhR6zv3)
- [M2.5 brass threaded inserts](https://www.aliexpress.com/item/32961915881.html?spm=a2g0s.9042311.0.0.27424c4dEGY0hq); 4 mm
- [1500 mAh LiPo battery](https://www.aliexpress.com/item/32910470051.html?spm=a2g0s.9042311.0.0.27424c4dEGY0hq); 40 &times; 50 &times; 5 mm
- [JST 2.0 PH 2-pin connector](https://www.aliexpress.com/item/32711927418.html?spm=a2g0s.9042311.0.0.27424c4dFcdI48)

For the assembly, I also needed:

- sandpaper in different grits
- alcohol and Q-tips to clean the plates and the neoprene sides
- universal glue and a professional, dedicated "spreading toothpick" -- to keep the threaded inserts in place, and also to glue the wooden layer-pairs together (I would use dedicated wood glue next time)
- a hot glue gun -- to affix the controller nuts and to glue the switches in place
- clamps (I had 4, but I would probably buy more next time)
- wood stain, rags, brushes
- a soldering kit, and a good wire stripper for the wiring
- a 1.5 mm allen key for the screws

Note that while the above shopping list looks quite nice and tidy, it's the result of probably half a dozen (or more) trips to different stores on as many occasions when I just remembered that "Oh, I need _that_ as well!".
And then a looot of waiting for everything to arrive.


### Assembly

Even most of the assembly consisted of waiting with all those drying and glue-setting times.
It also didn't help much that I'm a complete n00b, so I would rather not tell you the actual (ungodly amount of) time it took me to do this.
- only 3 legs for the controller --> improvisation is necessary :)

### Wiring

- battery fuckup
    - https://forums.adafruit.com/viewtopic.php?f=57&t=155069
    - reverse polarity...

### The finished product

Feast your eyes!





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

My keymap is available [here](https://docs.google.com/spreadsheets/d/1Af3q4IMkBpMjVFEbvwf8Ib_Vf0bDYXK0Xkphn-pAfto/edit?usp=sharing), which (similarly to the physical aspects of the builds) was very much born in the spirit of "go hard or go home".
The alpha layer is [Colemak Mod-DH](https://colemakmods.github.io/mod-dh/) without the punctuation, and the thumbfan is mostly the "destress the pinky" zone leaning heavily on [mod/tap](https://docs.qmk.fm/#/feature_advanced_keycodes?id=mod-tap) and layers.
The numbers are laid out like on a numpad, and the navigation layer (with full modifier support on the left) is designed with text editing in mind.
There are ergonomic consideration even on the symbol layer, where I placed the symbols to more/less comfortable positions according to their relative frequency in both natural language corpora (corpuses?) and programming language source code.


### Adjustment progress

As it's an enormous shift from full size QWERTZ to 36 key ergo Colemak, I've decided to put in the hours and learn proper touch typing this time.
It was pretty disheartening to go from around 60 wpm to below 10 in blink.
But I then completed the [typing.com](https://www.typing.com/) tutorial and reached ~30wpm, which provided enough familiarity to start the slow but steady climb over on [NitroType](https://www.nitrotype.com/team/D20R).
After daily practice for over 4 months (which I log [here](https://docs.google.com/spreadsheets/d/1pBDQd8YSD9eLS8331yy9vSNgv2Vac4_oS9yFa0fSX98/edit?usp=sharing)), I'm currently at an average of about 80 wpm with occasional visits in the 90s... Not part of the elite (yet) but progress nevertheless.

![WPM progress](https://zealot.hu/absolem/pics/wpm_progress.png)

I also frequently use [Keybr.com](https://www.keybr.com/) (like, a LOT), [10fastfingers](https://10fastfingers.com/typing-test/english), and [SpeedCoder](http://www.speedcoder.net/) for some varied practice.





## Writeup

I then decided that instead of writing a simple, short, and sweet reddit post about this, I'd write a more long-winded blog thingy to serve as the canonical home for the Absolem project.
In it, I divide the whole process into individual sections and talk about each topic in detail.
There's even a section in it which is about this writeup, in which I discuss why I wrote it, and how it consists of sections, one of which is about this exact writeup, in which... Whoops, infinite loop! `break;`





## Future work

Okay, if you want to nitpick, I did really say "endgame" in the title, which _should_ mean that there's no "future work" remaining...

![Technically Correct](https://zealot.hu/absolem/pics/technically_correct.jpg)

In my defense, though, I'd like to say that:

a) my TODO list is just about refinement, so we can justifiably say that it's v1.0 already;
b) I could have waited with this post until all the refinements are done as well... but just ~~couldn't~~ didn't want to; and
c) I've always planned to have 2 boards anyway (so that I have a backup if the main one ever needs service) in which case it would be foolish of me not to learn from the first round's experience and correct any minor imperfections.

So, even though I'm re(eeee)aly pleased with how things turned out, here are some further improvement ideas I have in mind:

- Physical:
    - I want a **PCB** -- despite the fact that I clearly stated in the past that I [don't want a PCB](https://www.reddit.com/r/MechanicalKeyboards/comments/bpc54c/absolem_step_2_towards_world_domination/esr8nbd/). I mean, I still stand by what I said there about the potential durability and fixability differences, but what I didn't consider with enough weight then is that it could hold both the Feather and the battery in place and I wouldn't need to glue extra nuts to the underside of the cover layer (I hate gluing...). It would also add more stability to the switches so I wouldn't need to glue those in either (Have I mentioned yet that I hate gluing?). Plus it would look much more "professional" and clean. And PCBs are not that expensive... And it would make it much easier for other people to potentially bulid their own. So yeah, I want a PCB.
    - I want a **thumb valley** -- as the topmost _cover_ layer (while beautiful as it is) still bothers my thumbs a little at certain angles, even after I deliberately filed away most of the edges there in anticipation of this exact thing... Again, it's not prohibitive or anything; it's barely noticable most of the time. But if I were to do this again (which I will), I would probably leave that region completely out as early as the lasercutting stage and then merge it together with the _undercover_ layer to make a more pronounced "valley". I'm probably not describing this very visually, but you'll see the difference when I get to it...
    - I want a **sleeve** -- to replace my trusty keyboard carrying [travel towel](). It's not the functionality, it's the prestige of a dedicated (and, of course, branded) carry case. Don't know much about how I want to solve this one yet, so it can end up a sleeve, or a hard case, or some sort of origami-ish [furoshiki](https://en.wikipedia.org/wiki/Furoshiki) cloth... Time will tell; stay tuned...
- Firmware-related:
    - I intend to solve various energy efficiency issues down the line, including a "deep" sleep, plus also a kind of middle ground between full activity and full sleep that would kick in earlier and where the matrix scanning is slower.
    - Yet another kind of sleep, this time for the Bluetooth connection polling, which could then be restarted of course, it just wouldn't waste the battery in an infinite loop.
    - Measuring (and then hopefully further optimizing) battery usage.
    - Finish the initial ideas sprinkled throughout the repo regarding general use and personalization, and add a build scrit that streamlines the process.
    - Separate the firmware part to its own repository (and add that as a submodule of the original one) to make it more accessible for potential other projects not associated with the physical aspects of the Absolem.

I'll probably write another post (and link it here) when I get around to addressing all these. 





## "Club"

I have a controlled, but ever present tendency to "fanboy" over something if it really deserves it.
For example, I'm a proud owner of a [Secrid Miniwallet](https://secrid.com/en-global/miniwallet-crisple-black) and I consider it a cool "club" to be a part of.
And I would very much like it if there was an official "Absolem club", even if it's just the two of us!

![Nerd Party](https://zealot.hu/absolem/pics/nerd_party.jpg)

So all I ask of you, dear reader, is that if you ever decide that the Absolem is a good match for you and you build one, please contact me so that I can add you to the club! Or you could contact me to build you one; then I'll know to add you automatically! It'll be great, I promise... We could even wear a badge! Too much? Okay, I'll stop now.

Anyways, there'll be a list of some sort here, if there's ever someone other than me... :)





## That's it, folks!

If you've seriously read all the way down here, then you're awesome! I hope I managed to keep it interesting. Now go and upvote the [corresponding reddit post]() and star my [GitHub repo](https://github.com/mrzealot/absolem) to earn an official, non-refundable, virtual high-five!


<br />
<br />
<hr />

__History__:

- Originally posted on 2019.??.??.

__Acknowledgment__: Besides the people already mentioned it the post, I'd like to thank my wife, who put up with me during this year-long process, and whose first reactions weren't "You're doing WHAT?!" and "It costs HOW MUCH?!" :)

__Disclaimer__: This post contains lots of pictures and references that are obviosly not my work, and I'm not claiming that they are. GIFs and jokes come from a quick Google searches, and I'm not going to put a "source: something..." under each one. Use your best judgement.

__Footnotes__:

<sup id="footnote-1">1</sup>: notice how my nick is "zealot", which is just a cooler name for fanatic. (I reject the religious connotation, though.)

<sup id="footnote-2">2</sup>: "best" is arguable here, I admit. But, despite my every effort to keep this as objective as possible, the whole topic is somewhat subjective, so bear with me.

<sup id="footnote-3">3</sup>: "heirloom-grade" was a phrase I first encountered on the KeyboardIO site, but subconsciously it has been the driving force behind a lot of the design and material choices I'd made even before then. I freely use it, though, as I find it sums up my intentions really well.

<sup id="footnote-4">4</sup>: My crafts teacher in elementary school (may he rest in peace) once said to me that if there's ever anything to fix around the house, I should just call someone over :D

<sup id="footnote-5">5</sup>: If you think that it's a shame that I didn't go in this 3D-ish direction, then feel free to go dumpster-diving into the [repo](https://github.com/mrzealot/absolem)'s history, and fish out the model to use/modify.