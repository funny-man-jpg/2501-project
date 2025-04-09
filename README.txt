Author: Erin Jones (101310356) and Sabin Caragea ()
OS: Windows

How to play:
You play as the dragon ship and your goal is to survive as long as possible. Kill enemies as they spawn in because as time passes they will 
only spawn more frequently. Enemies have a chance to drop health or your secondary weapon ammo on death as well. Try to aim for a high score
by lasting longer and killing more than anyone else. There's even a special boss for you to defeat if you can find him.

Controls:
Press Q to move left
Press E to move right
Press W to move forwards
Press S to move back
Press A to turn to the left
Press D to turn to the right
Press SPACE to shoot your primary weapon
Press LEFT_SHIFT to use your secondary weapon (you need ammo to do this)

Requirements:
Game Mechanics:
You play until you die, trying to get as many points as possible on the way. Surviving naturally gives you points (in the Game class), and
killing enemies will also award varying amounts of points (in Game class and various Enemy classes). The boss enemy awards a particularly
large amount of points, and you can find him hanging out _________ (see Game and Boss classes). Additionally, as time goes on enemies will
spawn in more frequently, making it more difficult for you to survive (the difficulty increase can be made more drastic by changing the 
DIFFICULTY definition in game.h to a much smaller number, the spawning code however with the actual decreasing spawn time is in game.cpp's 
Update()).

Enemies:
There are 3 enemy types, attackers, patrols, and runners. They all have their own textures and will spawn in at decreasing time intervals 
somewhere near the player. Attackers will wander until you get too close then they will intercept you and circle you like a shark while 
shooting at you. Patrols will wander until you get near them, then they will swap between pursuit and chasing you (based on distance),
all while shooting pairs of projectiles that move in a side-by-side sine wave motion, at you. Runners will evade you if you get too close,
dropping large bombs behind them as they run away. See the Enemy and Projectile classes (attacker_enemy_game_object.h/cpp, 
patrol_enemy_game_object.h/cpp, runner_enemy_game_object.h/cpp, bomb.h/cpp, projectile.h/cpp, wave_projectile.h/cpp). Finally we have the 
boss which stands in place and can target and shoot at you from far away (see the Boss class).

Weapons:
Your main weapon is a single straight shot (see Projectile class) and will kill any enemy but the boss in a single hit. Use the spacebar to
fire this weapon (Game class). There is a cooldown between shots (Player class). Both the player and the attacker share this type of attack 
and it uses ray circle collision. See Player, Projectile and Attacker classes.
Your secondary weapon is an emp ring centered on you. Use the left shift to shoot it, but only if you have the ammo for it (see the top 
right corner of your screen). This weapon will destroy projectiles and stun enemies. See EmpRing class and Player class.

Collectible Items:
There are hearts that are dropped by enemies which give you back health (Player, Health and Enemy classes). 
Batteries for your emp ring are dropped by the runner enemies (Player, Battery and Runner classes).
There are invincibility stars that give you invincibility if you collect 5 of them at a time. They are spawned randomly around the map.
(Game, Player and Collectible classes).

Movement:
Check any of the Player, or Enemy classes.

Collisions Detection:
Check any of the Collectible, Player, or Enemy classes or subclasses. There is circle circle and ray circle collision.

Game World:
Check the GameObject class (Render()) and the SetupGameWorld() method in the Game class for the tiling of the background. Also the vertex 
shader.

Particle Systems:
We have jets coming from behind the player and the player also explodes on death.

Hierarchy:
The boss is a hiearchy.

UI:
Check the HUD class.

Advanced Method:
Enemy movement as mentionned before.