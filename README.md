# battle-ship
Private side project.
This project is a side project I did between classes. The overall goal of the project was to create a computer that can
play as realistically as possible. By this, I mean that if it should hit a ship, it should do nothing else but try to sink that ship.
Until that ship has sunk, the computer will aggressively attempt to sink the ship.
I have constructed its algorithm for searching for a move in such a way that it will always sink a ship if it is able to hit it once.

Game starts by user initilializing board, since this is a console based game, user inputs row and column to place the end of each ship
They then decide on if their ship is placed horizontally or veritcally. 
From there, gameplay begins. User just inputs row and col they'd like to go and the computer's move follows.
Game continues until all of user's or computer's ships are sunk.
