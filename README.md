# phpwolf

## What is this?

phpwolf is an IRC bot written in PHP, allowing users to play the popular Werewolf game. This implementation of wolf is very similar to [wolfbot-new](http://code.google.com/p/wolfbot-new/), and takes upon many of the same features.

## What is Werewolf?

Werewolf is a variant of the party game [Mafia](http://en.wikipedia.org/wiki/Mafia_%28party_game%29). In summary, the objective is to eliminate members of the opposing team. At the start of the game, players are privately assigned roles as a wolf, or villager (all unknown to others). At night, the wolves select one victim to kill, and when the sun rises, villagers must persecute (lynch) one person they beleive is the wolf with a majority vote. This is a game of trust, and deception, as no one can be certain of the roles of everyone else. The game continues until there are the same amount of wolves as villagers (wolves win), or all the wolves are dead (villagers win).

## License

This is licensed [GPLv3](http://www.gnu.org/licenses/gpl.html).

> phpwolf is an IRC bot allowing users to play Werewolf, a variation of Mafia.
> Copyright (C) 2012 Brandon Cheng (gluxon)
> Copyright (C) 2012 ceradon

> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.

> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU General Public License for more details.

> You should have received a copy of the GNU General Public License
> along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Status

As of Christmas, the only requirement left to completion is IDLE check. The web implementation will be written with a Node.js server and ideally AJAX communication (WebSockets if not). Follow progress for the server at [WolfSpider](http://github.com/gluxon/WolfSpider).
