# Example bot

This code serves as the boilerplate code that everyone in the MINOR league is
allowed to copy. Although you could probably do this hackathon without ever
taking a look at most of the code. I would advise you to go through it as it
contains many tips to improve your bot.

## Where should I begin?
The eval function is in `src/evaluate.c`, it very primitive. You should probably
improve that :)


Also the searching currently is limited to depth 4 in `src/search.h`, while this
probably will work in the tournament (no guarantees). You probably will want to
somehow keep track of the time spent searching.

## Major League
For people in the major league, you are allowed to copy only and absolutely only
the code for handling UCI and FENs. See src/uci.c and src/position.c.
