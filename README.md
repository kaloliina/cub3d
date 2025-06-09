# so_long

## Project Overview

So Long is a project that involves creating a simple 2D game using the MLX library. The goal of this project was to understand and implement basic game mechanics, including player movement, collision detection, and level design.

## My Approach

- This project also felt like a complete step into the unknown, and by unknown, I mean the MLX library. Luckily, with the help of other peers from the school and by reading the comprehensive MLX header file, I started to understand how to use the graphics library. I first began by simply testing how to open a window and add a random texture to it. After this, I started all the way from the beginning, creating a struct that included all the necessary components and making sure I could parse the map, check the different error cases. After this was done, I then proceeded to render the images and implement the gameplay logic. I had to go a little bit back and forth as there were a lot of validation checks that needed to be done, and in the end, I did not want to include them in the game struct that I had created. As a last step, I started switching the placeholder assets that I had previously used. I wanted to create a fun little theme for the game, the theme being the "Piscine" month, with the player representing a duck swimming in a pool and having to collect C projects in order to get to the school.

## Learnings

- Minilibx Experience: So Long gave me valuable experience with minilibx, teaching me how to use its features for graphic projects. I learned about handling graphics windows, drawing images, and managing events.

- Error Handling: I learned a lot about handling multiple error cases to ensure the map was valid and functions exited properly. It involved anticipating potential issues and implementing robust checks to prevent crashes.

- Structs: This project helped me deepen my understanding of structs and how to use them effectively. I applied this knowledge to manage game states and objects efficiently.

- Macros: I understood how to use macros in a more effective way, especially when it comes to listing all the possible error messages.

## Future Directions

Oh, what isn't there to build up upon for this project. In all honesty, the outcome of this project was painful to me as I have worked as a game designer for quite a few years. In the end, I was happy with the code itself as I wanted to make as few functions as possible and divide the steps into understandable sections. However, from game design perspective, one could have used months to work on this project. One example being that now the player has to press a key every single time they want to move one tile, this does not really contribute to a great game flow... '^^ Anyways, as I was restricted with time, this will do from now on but hopefully in the future I get to build games that are both satisfying from design and code perspective!

## How to Run the Project

- To use my So Long implementation, clone the repository and compile the source files using the provided Makefile.

- Usage Examples: Run the program with a valid map file as an argument.
