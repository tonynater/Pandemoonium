#include <stdio.h>
#include <unistd.h>

#include "mm-client.h"

// This is the name that the driver will refer to your bot as.
const char* BOT_NAME = "Random";

// Return whether setup was successful, bot dies if 0.
int client_setup(int *argc, char ***argv)
{
	return 1;
}

// This function is called when the game begins, and provides initial player pools via the players array.
void game_setup(const struct player_data* players, unsigned int numplayers)
{

}

// When this function is called, your bot should respond with your move.
int player_turn(unsigned int roundnum, const struct player_data* players, unsigned int numplayers)
{
	return rand() % NUMCOWS;
}

// This function is called at the end of the game, as a courtesy.
void game_end()
{

}

