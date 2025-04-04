#include <cstdlib> 
#include <iostream>

#define NUM_DICE 6

int rand_dice (void)
{
    return (rand() % NUM_DICE) + 1;
}

int main (){
    srand(time(0)); // Seed the random number generator with the current time
    std::cout << rand_dice() << std::endl;
    std::cout << rand_dice() << std::endl;
    std::cout << rand_dice() << std::endl;
}