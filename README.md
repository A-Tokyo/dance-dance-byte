# Dance Dance Byte

## Description:

The game is simple enough. It has n*n squares in big square board, first of all a player should stand in the middle of the dancing board  to make the Ultrasonic sensor detect a player and start the game, the squares will start to light up in patterns according to the song. At this time a player should put his/her foot on the square which has a pressure sensor that till the program if it's the right square, according to that the score will calculated, if the player wins a sound will be changed to a winner sound as well as the LEDs will light up the winner pattern otherwise the looser one.
The game utilizes Arduino-mega328p.

### Inputs:

Ultrasonic sensor: to detect the presence of anyone wants to play the game, if so the game will start.
Photocell: to detect the intensity of the surrounding light which will be used to balance the game lightings accordingly (similarly like auto brightness in android phones which adjust the brightness according to the surrounding light ).
Pressure sensors: about 8 sensors (maybe more) that will be used to play the game.


### Outputs:

A Song that will be played using Adafruit Wave shield.
About 8 LEDs (maybe more) that will light up with different colors in different patterns according to the song of choice(extracted from the song frequency ).
A scoreboard to display the status of the game as well as current score of the player.


####Contributers:

- Sedki Gabra Sedki Abdelhakim
- Mostafa Ebrahim Hassan
- Nada Bahaa Eldin
- Ahmed Tarek Ahmed Mohamed
