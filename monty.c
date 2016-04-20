// Monty Hall Problem Simulator
//
// Monty Hall Problem Simulator allows the user to simulate 
// the Monty Hall Problem with any number of doors and any
// number of times within INT_MAX and 1.
//
// Date: 4/15/2016
// Written By: Toby Wang


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>


// init_random_number_generator() initiates random number generator
// time: O(1)
void init_random_number_generator(void)
{
  srand(time(NULL));
}


// random_number() returns a random number between 0 and RAND_MAX
// time: O(1)
int random_number(void)
{
  int r = rand();
  return r;
}


// roll_die(numSides) returns a digit between 1 and numSides
// requires: numSides >= 1
// time: O(1)
int roll_die(const int numSides)
{
  assert(numSides >= 1);
  int r = (random_number() % numSides) + 1;
  return r;
}


// setup_doors(numDoors) Sets up numDoors doors for Monty Hall Problem
// requires: numDoors >= 1
// effects: allocates memory (caller must free)
// time: O(n) where n is numDoors
bool **setup_doors(const int numDoors)
{
  assert(numDoors >= 1);

  // Set-up doors (win and losing) 
  // doors points to a is a multidimensional array with 
  //   dimensions 2x1 (rows x cols)
  // first row holds winning/losing door info
  // second row holds whether door is open or not
  bool *doorsPrize = malloc(numDoors * sizeof(bool));
  bool *doorsOpen = malloc(numDoors * sizeof(bool));
  bool **doors = malloc(2 * sizeof(bool *));

  doors[0] = doorsPrize;
  doors[1] = doorsOpen;

  // choose a winning door and set all other doors
  // to losing also close all doors
  const int winDoor = roll_die(numDoors);
  for (int i = 0; i < numDoors; ++i)
  {
    if (i == winDoor - 1)
    {
      doorsPrize[i] = true;
    }
    else
    {
      doorsPrize[i] = false;
    }
    doorsOpen[i] = false;
  }

  return doors;
}


// destroy_doors(doors) Destroys doors
// effects: Frees memory at doors and the addresses it points to
// time: O(n) where n is numDoors
void destroy_doors(bool **doors)
{
  free(doors[0]);
  free(doors[1]);
  free(doors);
  return;
}


int main(void)
{
  const int numDoors = 3;
  // results[0] = wins, results[1] = losses
  int results[2] = {0};
  char playAgain = 'Y';

  // Set-up RNG
  init_random_number_generator();

  // Intro to simulator
  printf("Welcome to the Monty Hall Problem Simulator!\n\n");
  printf("Monty Hall Problem:\nSuppose you're on a game show, \
and you're given the choice of three doors: Behind one door \
is a car; behind the others, goats. You pick a door, say No. \
1, and the host, who knows what's behind the doors, opens \
another door, say No. 3, which has a goat. He then says to \
you, \"Do you want to pick door No. 2?\" Is it to your \
advantage to switch your choice?\n");

  while (playAgain == 'Y' || playAgain == 'y')
  {
    // How many repeated experiments?
    int numPlays = 1;
    printf("\nHow many plays: ");
    scanf("%d", &numPlays);

    // Launch experiments
    for (int j = 0; j < numPlays; j++)
    {
      // Always start with door 1 as choice
      int choice = 0;

      // Set-up doors (2 x numDoors array)
      // first row holds winning/losing door info
      // second row holds info about whether door is open
      bool **doors = setup_doors(numDoors);

      // Open first false door that we didn't pick
      int losingDoor = 0;
      while (losingDoor == choice || doors[0][losingDoor])
      {
        ++losingDoor;
      }
      doors[1][losingDoor] = true;

      // Switch choice to other door 
      //   or the first door we didnt pick and is not opened
      //   (for experiments with more than 3 doors)
      int otherDoor = 0;
      while (otherDoor == losingDoor || otherDoor == choice)
      {
        ++otherDoor;
      }
      choice = otherDoor;

      // record results
      doors[1][choice] = true;
      if (doors[0][choice] == false)
      {
        // loss
        results[1]++;
      }
      else
      {
        // win
        results[0]++;
      }

      // clean-up memory
      destroy_doors(doors);
    }

    // print results
    printf("Times win car after switching choice: %d\n", results[0]);
    printf("Times win goat after switching choice: %d\n\n", results[1]);
    
    printf("Play Again? (Y/N): ");
    scanf(" %c", &playAgain);
  }
  printf("Bye!\n");

  return 0;
}
