# Game Physics Cookbook

This book is a comprehensive guide to game collision detection, but only briefly touches on the topic of collision resolution (Physics). Originally this book was planned as two books:

1) Primitive Collisions Cookbook
2) Game Physics Cookbook

I was only able to write one book. This means a LOT of information had to be packed into this book. The general breakdown is as follows

[TODO] Chapter breakdowns

# Issues

There are various issues in the physics implementation of the source code. These issues come from the fact that the physics part of this book had to be condensed into three chapters. There was just not enough time to cover everything needed to make a robust rigid-body physics system. The biggest problem with the engine is the fact that __there is no Arbiter__. 

Without an arbiter, we can't build a sequential impulse solver. This leaves us with a rather bare-bones naive impulse solver. The problem is, impulses are solved per contact per frame. This causes excessive sliding. I've compensated for the lack of sequential impulse with linear projection and agressive friciton biasing. Older physics engines do something similar, by relying on strong handed sleeping to mask the issue. Needless to say, sleeping was not implemented.

# Future work

I still want to create a book dedicated to robust rigidbody physics. I'm also exploring various other outlets for this, perhaps a series of youtube videos or maybe even blogs. Time is the biggest factor in all of this, as of right now i have no time frame for when i could even start on this project

# Book Info

__TODO: Imge_

* ISBN-13:
* Publisher Website
* Amazon Page