# Final Project Proposal - Super Stickman Golf

For my final project I plan to do a recreation of the iconic smart phone app, Super Stickman Golf. The main goal of this
game if you are unfamiliar is to get your golf ball into the hole in the lowest number of moves_ possible, while avoiding
hazards (water, sand, hill barriers, etc.) and utilizing power-ups (air brakes, extra bounces, etc.) The main reason why 
I want to do this project is because this was my favorite game to play on my phone in high school. As a result, I wished
for some way to play this game on a laptop, rather than just on the phone. 

I believe that my experience using cinder in this class, along with a basic understanding of physics should help me
complete this project without having to do an extraordinary amount of research. Also, from Naive bayes, I have become 
relatively familiar with using operating overloading if I were to load the maps in that way. Additionally, I have 
experience from a high school CS project, where I programmed a variation of 2048 in java where I was able to practice 
taking in and reading key strokes, and I hope that the skills will be relatively transferrable just in c++.

### Sprint 1
* Implement proper golf ball movement
    * Use Kinematics and 2D Vectors 
    * Handle Collisions with a dampening constant
    * Turn off Gravity when on Ground
    * Turn on Friction when On Grass
* Design a way to alter the angle and power of the shot
    * Use key strokes for this 
* Create a rough cinder visual to see if movement appears correct
* Testing

### Sprint 2
* Add hazards to the map
    * Barriers
    * Sand
      * Ball Comes to Complete Rest
    * Water
      * Ball Resets to Original Position
    * Sticky
      * Ball Comes to Complete Rest
* Create a map that can be loaded in and drawn with Cinder
  * Use Operator overload >>
    * Create a New Class For This
* Testing

### Sprint 3
* Add power-up functionality for different types of golf balls
  * Air brakes
  * Extra bounces
  * Mulligans
* Make the game more user friendly
    * Clearer visuals for the specified power and angle that you want to hit your ball with
    * Add move and power-up counter
    * Draw stickman to represent user placement
        * Possibly load in image instead of drawing it
* README
* Testing

### Stretch Goals
* Saving high scores/statistics to a file for future use
* Creating larger maps that require one to zoom in and out
* Having a screen before play that lets you customize your stick man
* Changing the background of the game based off the time of day
* Possibly adding in wind for some stages
* Having a race mode where there is a pre-programmed cpu to compete against