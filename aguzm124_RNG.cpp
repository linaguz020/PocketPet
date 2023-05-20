/*         Angelina Guzman aguzm124@ucr.edu
 *         Discussion Section: 21
 *         Assignment: Custom Lab Project

 *         Random Number Generator
 *         RNG() and randomizePet() functions.

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.
 *         Algorithm for RNG() from Stephen K. Park and Keith W. Miller
 *         "Random Number Generations: Good Ones Are Hard to Find" (1988)

 */
double seed = 7607.0; // prime number as 1st seed
unsigned int RNG() {
    const double m = 2147483647.0; // modulus - a large prime number
    const double a = 16807.0; // multiplier
    const double q = 127773.0;
    const double r = 2836.0;
    double test;
    double hi;
    double lo;
    hi = trunc(seed/q);
    lo = seed - (q * hi);
    test = (a * lo) - (r * hi);
    if(test > 0.0) {
      seed = test;
      } else {
        seed = test + m;
        }
    return seed;
}

/* implements RNG and using modulus to pick from an array of petNames */
void randomizePet() {
  int index = RNG() % numNames;
  petName = petNames[index];
}
