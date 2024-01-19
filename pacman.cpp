// Pacman gamedd
// Wallace Su
// December 5th 2022
#include <stdio.h>
#include "colours.h"
#include <stdlib.h>
#include <conio.h>
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

void dotReplace (char** processMap, char** dotMap) { //replacing the dots from the dot map to the process map
    for (int i=0; i<11; i++) {
        for (int j=0; j<11; j++) {
            if (processMap[i][j]!=PACMAN && processMap[i][j]!=WALL && processMap[i][j]!=GHOST) { //replacing everything that is not pacman, wall or ghost
                processMap[i][j]=dotMap[i][j]; //replacing
            }
        }
    }
}

void printMap(char** map, char** dotMap){
    dotReplace(map, dotMap);
    printf("\n");

    for (int y = 0; y < 11; y++) { //Iterate through rows
        for (int x = 0; x < 11; x++) { //Print characters in columns, apply colors according to character

            if (x==0 && y==0) {
                colourChange(BLUE);
            }

            printf("%c  ", map[y][x]); //printing out the map
            colourChange(BLUE); //changing colours
            if (map[y][x+1] == PACMAN) { // changing pacman to yellow
                colourChange(YELLOW);
            }
            if (map[y][x+1] == DOT) { //changing the dots to white
                colourChange(WHITE);
            }
            if (map[y][x+1] == GHOST) { //changing the ghosts to pink
                colourChange(PINK);
            }
        }
        printf("\n");
    }
}

int winCheck(char** dotMap){ //checking for win
    int dots=0;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            if (dotMap[i][j] == DOT) { //if there are no dots left on the board, the game will end
                return 0;
            }
        }
    }
    colourChange(WHITE); //changing the colour of the text to white
    printf("Congratulations! You win! Press any key to exit the game"); //output text
    return 1;
}

int loseCheck(char** map){ //If P does not exist (after being eaten by G)
    int missing = 1;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 11; x++) {
            if (map[y][x] == PACMAN) { //checks if pacman is missing from the board
                missing = 0;
                break;
            }
        }
        if (missing == 0) {
            break;
        }
    }
    if (missing == 1) { //if pacman does not exist anymore, the game displays that the player has lost
        colourChange(WHITE);
        printf("Sorry, you lose. Press any key to exit the game");
    }
    return missing; //If missing, end main loop
}

int isWall(char** map, int x, int y, char input) { //Has access to map, takes coordinates of object and direction of intended travel
    if (input == LEFT) { //If intended direction is left, look left
        if (map[y][x - 1] == WALL) { //If there is a wall on the left, return 1
            return 1;
        }
    } else if (input == RIGHT) { //Similar to above
        if (map[y][x + 1] == WALL) {
            return 1;
        }
    } else if (input == DOWN) { //Similar to above
        if (map[y + 1][x] == WALL) {
            return 1;
        }
    } else if (input == UP) { //Similar to above
        if (map[y - 1][x] == WALL) {
            return 1;
        }
    }
    return 0; //If reached this far, no wall has been reported. Return 0 to indicate no walls
}

void playerMoving(char** map, char** dotMap, char input){ //function to move pacman
    int x, y;
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 10; j++) {
            if (map[i][j] == PACMAN) { //looking for the p in the array to find pacman's coordinates
                y = i;
                x = j;
            }
        }
    }
    if (input == UP) { //taking in user input and moving pacman accordingly
        if (isWall(map, x, y, input) == 0 && map[y-1][x] != GHOST) { //makes sure that there is no wall or ghost to where pacman is moving
            map[y - 1][x] = PACMAN; //moves pacman
            map[y][x] = EMPTY; //sets his original position to empty
            dotMap[y][x] = EMPTY; //edits the dot map
        }
    }
    if (input == DOWN && map[y+1][x] != GHOST) { //repeats for each direction
        if (isWall(map, x, y, input) == 0) {
            map[y + 1][x] = PACMAN;
            map[y][x] = EMPTY;
            dotMap[y][x] = EMPTY;
        }
    }
    if (input == LEFT && map[y][x-1] != GHOST) {
        if (isWall(map, x, y, input) == 0) {
            map[y][x - 1] = PACMAN;
            map[y][x] = EMPTY;
            dotMap[y][x] = EMPTY;
        }
    }
    if (input == RIGHT && map[y][x+1] != GHOST) {
        if (isWall(map, x, y, input) == 0) {
            map[y][x + 1] = PACMAN;
            map[y][x] = EMPTY;
            dotMap[y][x] = EMPTY;
        }
    }
}

void ghostMoving(char** mainMap) { //function to move the ghosts
    int x1=0, y1=0;
    int x2, y2, rng;
    for (int i=0; i<11; i++) {
        for (int j=0; j<11; j++) {
            if (mainMap[i][j]==GHOST) { //finding the coordinates of the ghost
                if (x1==0 && y1==0) { //x1 and y1 are initialized as 0 and when changed, they will be the coordinates of the first ghost. This way, it lets both ghost's coordinates be stored
                    x1=j;
                    y1=i;
                } else { //if the second ghost is found and the first ghost is already found
                    x2=j;
                    y2=i;
                }
            }
        }
    }
    int xp, yp; //Finding pacman coordinates
    for (int i = 1; i < 10; i++) {
        for (int j = 1; j < 10; j++) {
            if (mainMap[i][j] == PACMAN) { //When coords of pacman are found, store as (xp, yp)
                yp = i;
                xp = j;
            }
        }
    }
    if (y1 == yp) { //If first ghost and pacman are in the same row, determine whether ghost sees wall or pacman first. If wall, continue to random movement. If pacman, move towards pacman.
        for (int x = x1; x < 11; x++) { //RIGHT
            if (mainMap[y1][x] == WALL) {
                break; //Continue to random movement
            } else if (mainMap[y1][x] == PACMAN) {
                mainMap[y1][x1 + 1] = GHOST;
                mainMap[y1][x1] = EMPTY;
                goto moved1; //Skip to after random movement
            }
        }
        for (int x = x1; x > 0; x--) { //LEFT
            if (mainMap[y1][x] == WALL) {
                break;
            } else if (mainMap[y1][x] == PACMAN) {
                mainMap[y1][x1 - 1] = GHOST;
                mainMap[y1][x1] = EMPTY;
                goto moved1;
            }
        }
    } else if (x1 == xp) { //Same as above but for same column
        for (int y = y1; y < 11; y++) { //DOWN
            if (mainMap[y][x1] == WALL) {
                break;
            } else if (mainMap[y][x1] == PACMAN) {
                mainMap[y1 + 1][x1] = GHOST;
                mainMap[y1][x1] = EMPTY;
                goto moved1;
            }
        }
        for (int y = y1; y > 0; y--) { //UP
            if (mainMap[y][x1] == WALL) {
                break;
            } else if (mainMap[y][x1] == PACMAN) {
                mainMap[y1 - 1][x1] = GHOST;
                mainMap[y1][x1] = EMPTY;
                goto moved1;
            }
        }
    }
    while (true) { //First ghost randomized movement
        rng=rand()%4; //finds a random number from 0 to 3 and uses it to find a random direction for the ghost to move in. It will keep on looping until it finds a direction that works.
        if (rng == 0 && isWall(mainMap, x1, y1, LEFT) == 0 && mainMap[y1][x1 - 1] != GHOST) { //left movement, it makes sure that the direction is valid and can be moved to (repeated for each direction)
            mainMap[y1][x1 - 1] = GHOST;
            mainMap[y1][x1] = EMPTY;
            break;
        } else if (rng == 1 && isWall(mainMap, x1, y1, RIGHT) == 0 && mainMap[y1][x1 + 1] != GHOST) { //right movement
            mainMap[y1][x1 + 1] = GHOST;
            mainMap[y1][x1] = EMPTY;
            break;
        } else if (rng == 2 && isWall(mainMap, x1, y1, UP) == 0 && mainMap[y1-1][x1] != GHOST) { //up movement
            mainMap[y1 - 1][x1] = GHOST;
            mainMap[y1][x1] = EMPTY;
            break;
        } else if (rng == 3 && isWall(mainMap, x1, y1, DOWN) == 0 && mainMap[y1+1][x1] != GHOST) { //down movement
            mainMap[y1 + 1][x1] = GHOST;
            mainMap[y1][x1] = EMPTY;
            break;
        }
    }
    moved1:; //Place to skip to after ghost 1 movement
    //Second ghost
    if (y2 == yp) {
        for (int x = x2; x < 11; x++) { //RIGHT
            if (mainMap[y2][x] == WALL) {
                break;
            } else if (mainMap[y2][x] == PACMAN) {
                mainMap[y2][x2 + 1] = GHOST;
                mainMap[y2][x2] = EMPTY;
                goto moved2;
            }
        }
        for (int x = x2; x > 0; x--) { //LEFT
            if (mainMap[y2][x] == WALL) {
                break;
            } else if (mainMap[y2][x] == PACMAN) {
                mainMap[y2][x2 - 1] = GHOST;
                mainMap[y2][x2] = EMPTY;
                goto moved2;
            }
        }
    } else if (x2 == xp) {
        for (int y = y2; y < 11; y++) { //DOWN
            if (mainMap[y][x2] == WALL) {
                break;
            } else if (mainMap[y][x2] == PACMAN) {
                mainMap[y2 + 1][x2] = GHOST;
                mainMap[y2][x2] = EMPTY;
                goto moved2;
            }
        }
        for (int y = y2; y > 0; y--) { //UP
            if (mainMap[y][x2] == WALL) {
                break;
            } else if (mainMap[y][x2] == PACMAN) {
                mainMap[y2 - 1][x2] = GHOST;
                mainMap[y2][x2] = EMPTY;
                goto moved2;
            }
        }
    }
    while (true) { //Second ghost random movement, it functions exactly the same as the first
        rng=rand()%4;
        if (rng == 0 && isWall(mainMap, x2, y2, LEFT) == 0 && mainMap[y2][x2 - 1] != GHOST) { //left movement
            mainMap[y2][x2 - 1] = GHOST;
            mainMap[y2][x2] = EMPTY;
            break;
        } else if (rng == 1 && isWall(mainMap, x2, y2, RIGHT) == 0 && mainMap[y2][x2 + 1] != GHOST) { //right movement
            mainMap[y2][x2 + 1] = GHOST;
            mainMap[y2][x2] = EMPTY;
            break;
        } else if (rng == 2 && isWall(mainMap, x2, y2, UP) == 0 && mainMap[y2-1][x2] != GHOST) { //up movement
            mainMap[y2 - 1][x2] = GHOST;
            mainMap[y2][x2] = EMPTY;
            break;
        } else if (rng == 3 && isWall(mainMap, x2, y2, DOWN) == 0 && mainMap[y2+1][x2] != GHOST) { //down movement
            mainMap[y2 + 1][x2] = GHOST;
            mainMap[y2][x2] = EMPTY;
            break;
        }
    }
    moved2:; //Place to skip to after ghost 2 movement
}

int main() { //main function
    int win = 0, lose = 0; //initializing and declaring arrays
    char enteredKey;
    char **processMap = (char **) malloc(11 * sizeof(char *)); //creating and initializing the 2D pointers that will be used to store the map and the dotMap
    char **dotMap = (char **) malloc(11 * sizeof(char *));
    for (int i = 0; i < 11; i++) {
        processMap[i] = (char *) malloc(11 * sizeof(int));
        dotMap[i] = (char *) malloc(11 * sizeof(int));
    }

    FILE *mapFile; //declaring the file as mapFile
    mapFile = fopen("../map.txt", "r"); //opening the file as a read
    if (mapFile == NULL) { //if the map is not found, it prints it out
        printf("FILE NOT FOUND\n");
    }

    for (int i = 0; i < 11; i++) { //Copy from map.txt to our map and dotMap
        if (i == 0 || i == 10) { //First and last rows are walls
            for (int x = 0; x < 11; x++) {
                processMap[i][x] = WALL;
                dotMap[i][x] = EMPTY;
            }
        } else {
            processMap[i][0] = WALL; //first column as wall
            dotMap[i][0] = EMPTY; //first dotMap column as empty
            for (int j = 0; j < 9; j++) { //Copy from map.txt
                fscanf(mapFile, "%c  ", &processMap[i][j + 1]);
                if (processMap[i][j + 1]==DOT) { //making sure that there are only dots being stored in dotMap
                    dotMap[i][j+1] = processMap[i][j + 1];
                } else {
                    dotMap[i][j+1] = EMPTY; //non dots will be stored as empty so that the dotMap is only dots
                }
            }
            processMap[i][10] = WALL; //last map column being set as walls
            dotMap[i][10] = EMPTY; //last dotMap column being set as empty
        }
    }
    printMap(processMap, dotMap); //First print of map
    while (win == 0 && lose == 0) { //Loop gameplay sequence until win or lose
        enteredKey = (char) getch(); //Receive user input
        playerMoving(processMap, dotMap, enteredKey); //Player movement
        ghostMoving(processMap); //Ghost movement
        printMap(processMap, dotMap); //Print updated map
        win = winCheck(dotMap); //Check if win
        lose = loseCheck(processMap); //Check if lose
        if (win == 1 || lose == 1) { //Let user enter key to end program
            getch();
        }
    }
    for (int i=0; i<11; i++) { //freeing the 2D pointers to properly manage memory
        free(processMap[i]);
        free(dotMap[i]);
    }
    fclose(mapFile); //Closing the file because we are finished using it
}