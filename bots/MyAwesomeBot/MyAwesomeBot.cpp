//Stanley Cen and Tony Wang, 2013

#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <ctime>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "mm-client.h"

using namespace std;

#define sz(x) ((int) x.size())

typedef long double ld;
typedef long long ll;
typedef pair<int, int> pii;

const double pi = acos(-1);
const double tau = 2*pi;
const double epsilon = 1e-9;

const char* BOT_NAME = "Xxx5up3Rl337420H4x0r13376969xxX"; //This is the name that the driver will refer to your bot as.

vector<pii> SORTEDMILKVALUES;

struct playerMilkHistory
{
    int cowFrequency[MAXCOWS];
};

struct orderedPlayerMilkHistory
{
    int cowFrequency[MAXCOWS];
    list<int> cowMilked;
    int sz;
};

playerMilkHistory allPlayerHistory[MAXPLAYERS];

orderedPlayerMilkHistory orderedAllPlayerHistory[MAXPLAYERS];

//*******************************************************************
//algorithm 1

int get_expected_cow1(int id, int roundnum)
{
    if(roundnum == 0) return SORTEDMILKVALUES[0].second;
    for(int i = 0, rval = 1+(rand()%roundnum), sum = 0; i < MAXCOWS; i++)
    {
        sum += allPlayerHistory[id].cowFrequency[i];
        if(sum >= rval)
            return i;
    }
    
    return -1; //wat?
}

int expected_count1[MAXCOWS];
void get_expected_count1(int numplayers, int roundnum)
{
    memset(expected_count1, 0, sizeof(expected_count1));
    for(int i = 0; i < numplayers; i++)
        expected_count1[get_expected_cow1(i, roundnum)]++;
}

int best_choice1()
{
    int choice = 0;
    for(int i = 0, best_share = 0; i < NUMCOWS; i++)
    {
        int r = MILKVALUES[i] / (expected_count1[i]+1);
        if(r > best_share)
        {
            best_share = r;
            choice = i;
        }
    }
    
    return choice;
}

int algorithm1(int numplayers, int roundnum)
{
    get_expected_count1(numplayers, roundnum);
    
    return best_choice1();
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


//*******************************************************************
//algorithm 2

double expected_count2[MAXCOWS];
void get_expected_count2(int numplayers, int roundnum)
{
    if(roundnum == 0) return;
    memset(expected_count2, 0, sizeof(expected_count2));
    for(int i = 0; i < numplayers; i++)
	if(i != SELF.id)
            for(int j = 0; j < MAXCOWS; j++)
           	expected_count2[j] += ((double)allPlayerHistory[i].cowFrequency[j])/((double)roundnum);
}

int best_choice2()
{
    int choice = 0;
    double max_milk = 0;
    for(int i = 0; i < NUMCOWS; i++)
    {
        double r = ((double)MILKVALUES[i]) / ((double)(expected_count2[i]+1.0));
        if(r > max_milk)
        {
            max_milk = r;
            choice = i;
        }
    }
    
    return choice;
}

int algorithm2(int numplayers, int roundnum)
{
    get_expected_count2(numplayers, roundnum);
    
    return best_choice2();
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//*******************************************************************
//algorithm 3

double expected_count3[MAXCOWS];
void get_expected_count3(int numplayers, int roundnum)
{
    if(roundnum == 0) return;
    memset(expected_count3, 0, sizeof(expected_count3));
    for(int i = 0; i < numplayers; i++)
	if(i != SELF.id)
            for(int j = 0; j < MAXCOWS; j++)
           	 expected_count3[j] += ((double)allPlayerHistory[i].cowFrequency[j])/((double)roundnum);
}

int best_choice3()
{
    int choice = 0;
    double max_milk = 0;
    for(int i = 0; i < NUMCOWS; i++)
    {
        int l = expected_count3[i]+1.0, lm = MILKVALUES[i]/l;
        int h = l+1, hm = MILKVALUES[i]/h;
        double dlm = lm, dhm = hm;
        double r = dhm + (dlm-dhm)*(1.0-(expected_count3[i]-floor(expected_count3[i])));
        if(r > max_milk)
        {
            max_milk = r;
            choice = i;
        }
    }
    
    return choice;
}

int algorithm3(int numplayers, int roundnum)
{
    get_expected_count3(numplayers, roundnum);
    
    return best_choice3();
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//*******************************************************************
//algorithm 4

double expected_count4[MAXCOWS];
void get_expected_count4(int numplayers, int roundnum)
{
    if(roundnum == 0) return;
    memset(expected_count4, 0, sizeof(expected_count4));
    for(int i = 0; i < numplayers; i++)
	if(i != SELF.id)
            for(int j = 0; j < MAXCOWS; j++)
            	expected_count4[j] += ((double)allPlayerHistory[i].cowFrequency[j])/((double)roundnum);
}

vector<int> choices4;
int best_choice4()
{
    double max_milk = 0;
    for(int i = 0; i < NUMCOWS; i++)
    {
        int l = expected_count4[i]+1.0, lm = MILKVALUES[i]/l;
        int h = l+1, hm = MILKVALUES[i]/h;
        double dlm = lm, dhm = hm;
        double r = dhm + (dlm-dhm)*(1.0-(expected_count4[i]-floor(expected_count4[i])));
        if(r > max_milk-epsilon)
        {
            max_milk = r;
            choices4.clear();
            choices4.push_back(i);
        }else if(abs(r-max_milk) < epsilon)
        {
            choices4.push_back(i);
        }
    }
        
    return choices4[rand()%sz(choices4)];
}

int algorithm4(int numplayers, int roundnum)
{
    get_expected_count4(numplayers, roundnum);
    
    return best_choice4();
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//*******************************************************************
//algorithm 5

const int MAXCOWS_REMEMBER = 15;

double expected_count5[MAXCOWS];
void get_expected_count5(int numplayers, int roundnum)
{
    if(roundnum == 0) return;
    memset(expected_count5, 0, sizeof(expected_count5));
    for(int i = 0; i < numplayers; i++)
	 if(i != SELF.id)
             for(int j = 0; j < MAXCOWS; j++)
            	 expected_count5[j] += ((double)orderedAllPlayerHistory[i].cowFrequency[j])/(min(((double)roundnum), ((double) MAXCOWS_REMEMBER)));
}

vector<int> choices5;
int best_choice5()
{
    double max_milk = 0;
    for(int i = 0; i < NUMCOWS; i++)
    {
        int l = expected_count5[i]+1.0, lm = MILKVALUES[i]/l;
        int h = l+1, hm = MILKVALUES[i]/h;
        double dlm = lm, dhm = hm;
        double r = dhm + (dlm-dhm)*(1.0-(expected_count5[i]-floor(expected_count5[i])));
        if(r > max_milk-epsilon)
        {
            max_milk = r;
            choices5.clear();
            choices5.push_back(i);
        }else if(abs(r-max_milk) < epsilon)
        {
            choices5.push_back(i);
        }
    }
        
    return choices5[rand()%sz(choices5)];
}

int algorithm5(int numplayers, int roundnum)
{
    get_expected_count5(numplayers, roundnum);
    
    return best_choice5();
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



int client_setup(int *argc, char ***argv) //Return whether setup was successful, bot dies if 0.
{
    memset(allPlayerHistory, 0, sizeof(allPlayerHistory));
    timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
    return 1;
}

void game_setup(const struct player_data* players, unsigned int numplayers) //This function is called when the game begins, and provides initial player pools via the players array.
{
    for(int i = 0; i < NUMCOWS; i++)
        SORTEDMILKVALUES.push_back(pii(MILKVALUES[i],i));
    
    sort(SORTEDMILKVALUES.rbegin(), SORTEDMILKVALUES.rend());

    for(int i = 0; i < numplayers; i++)
        orderedAllPlayerHistory[i].sz = 0;
}

int player_turn(unsigned int roundnum, const struct player_data* players, unsigned int numplayers) //When this function is called, your bot should respond with your move.
{    
    for(int i = 0; i < numplayers; i++)
        if(players[i].loc != -1)
	{
            ++allPlayerHistory[i].cowFrequency[players[i].loc];
	    ++orderedAllPlayerHistory[i].cowFrequency[players[i].loc];
	    orderedAllPlayerHistory[i].cowMilked.push_back(players[i].loc);
	    ++orderedAllPlayerHistory[i].sz;
	    if(orderedAllPlayerHistory[i].sz > MAXCOWS_REMEMBER)
	    {
		--orderedAllPlayerHistory[i].cowFrequency[orderedAllPlayerHistory[i].cowMilked.front()];
		orderedAllPlayerHistory[i].cowMilked.pop_front();
		--orderedAllPlayerHistory[i].sz;
	    }
	}
    
    if(SORTEDMILKVALUES[0].first/numplayers >= SORTEDMILKVALUES[1].first) return SORTEDMILKVALUES[0].second;
    
    //^^^^^^^^^^^^^^^^^^READING NEW INFO + CORNER-CASES^^^^^^^^^^^^^^^^^^
    
    //return algorithm1(numplayers, roundnum); //retarded "expected value"
    //return algorithm2(numplayers, roundnum); //real expected value
    //return algorithm3(numplayers, roundnum); //real expected value with extrapolation for rounding
    //return algorithm4(numplayers, roundnum); //real expected value with extrapolation for rounding + randomization for same highest expected value
    return algorithm5(numplayers, roundnum); //real expected value with extrapolation for rounding from last 20 rounds+ randomization
}

/* Algorithm Results:
 * ----------------------------------------------------------
 * Algorithm 1: Complete failure. No further explanation needed.
 *
 * Algorithm 2:
 *          -Random (large) # of Cows, Random (large) Milk Values, One of Each Standard Bot: (3821/5000)
 *          -Random (medium) # of Cows, Random (medium) Milk Values, One of Each Standard Bot: (627/1000), (59/100), (58/100), (70/100)
 *          -Random (large) # of Cows, Random (low) Milk Values, One of Each Standard Bot:  (883,1000), (89/100), (80/100), (88/100)
 *          -Random (large) # of Cows, Random (large) Milk Values, One of Each Standard Bot: (741/1000)
 *
 * Algorithm 3:
 *          -Random (large) # of Cows, Random (large) Milk Values, One of Each Standard Bot: (3863/5000)
 *          -Random (medium) # of Cows, Random (medium) Milk Values, One of Each Standard Bot: (610/1000), (61/100), (72/100), (69/100)
 *          -Random (large) # of Cows, Random (low) Milk Values, One of Each Standard Bot: (869,1000), (81/100), (85/100), (88,100)
 *          -Random (large) # of Cows, Random (large) Milk Values, One of Each Standard Bot: (3831/5000), (779/1000)
 *
 * Algorithm 4:
 *          -Slight improvement, not really noticeable.
 * 
 * Algorithm 5:
 *	    -Still getting *****
 *
 */


void game_end() //This function is called at the end of the game, as a courtesy.
{
    
}
