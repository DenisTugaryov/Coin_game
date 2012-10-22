#include <vector>
#include <random>
#include <set>
#include <iostream>
#include <utility>
#include <cmath>

const int GAME_SIZE = 10;
const int GAME_NUMBER = 100;
const double HEAD_PROB = 0.5;
const double ALPHA = 0.9;
const double EPSILON = pow(10,-3);

struct cmp
{
  bool operator() (std::pair<int, int> p1, std::pair<int, int> p2)
  {
    return p1.first < p2.first;
  }
};

int coin0604()
{
  static std::default_random_engine generator0604;
  static std::uniform_int_distribution<int> distribution0604(0,4);
  if (distribution0604(generator0604) < 2)
    return 0;
  return 1;
}

int coin0406()
{
  static std::default_random_engine generator0406;
  static std::uniform_int_distribution<int> distribution0406(0,4);
  if (distribution0406(generator0406) < 3)
    return 0;
  return 1;
}

int honest_coin()
{
  static std::default_random_engine generator;
  static std::uniform_int_distribution<int> distribution(0,1);
  return distribution(generator);
}

int main()
{
  int counter = 1;

  std::vector<int> rate(GAME_SIZE, 0);
  for (int i = 1; i < GAME_SIZE; ++i)
    if (i <= GAME_SIZE / 2)
      rate[i] = i;
    else
      rate[i] = GAME_SIZE - i;

  std::cout << counter << ") \t";
  for (int i = 1; i < GAME_SIZE; ++i)
    std::cout << rate[i] << "\t ";
  std::cout << std::endl;

  bool bets_are_new = true;
  while (bets_are_new)
  {
    std::vector<int> old_rate = rate;
    bets_are_new = false;

    std::vector<int> get_number(GAME_SIZE, 0);
    std::vector<double> wins_number(GAME_SIZE, 0);

    for (int i = 1; i < GAME_SIZE; ++i)
    {
      for (int current_game_number = 0; current_game_number < GAME_NUMBER; ++current_game_number)
      {
        int bet = rate[i];
        int money = i;
        int step_number = 0;
        std::pair<int, int> money_stepnumber = std::make_pair(money, step_number);
        std::set<std::pair<int, int>, cmp > positions;
        bool game_is_won = false;

        while(money > 0 && money < GAME_SIZE)
        {
          positions.insert(money_stepnumber);

          int coin_sign = 0;

          if (HEAD_PROB == 0.5)
            coin_sign = honest_coin();
          else if (HEAD_PROB == 0.6)
            coin_sign = coin0604();
          else if (HEAD_PROB == 0.4)
            coin_sign = coin0406();

          if (coin_sign == 1)
            money += bet;
          else
            money -= bet;

          ++step_number;

          if (money == GAME_SIZE)
            game_is_won = true;
          else if (money > 0)
          {
            bet = rate[money];
            money_stepnumber = std::make_pair(money, step_number);
          }
        }

        for (std::set<std::pair<int, int>, cmp >::iterator it = positions.begin(); it != positions.end(); ++it)
        {
          if (game_is_won)
            wins_number[it->first] += 1;//pow(0.9, step_number - it->second);
          get_number[it->first] += 1;
        }
      }
    }

    std::vector<double> old_probabilities(GAME_SIZE + 1, 0);
    std::vector<double> probabilities(GAME_SIZE + 1, 0);
    for (int i = 1; i < GAME_SIZE; ++i)
      probabilities[i] = wins_number[i] / get_number[i];
    probabilities[GAME_SIZE] = 1;

    bets_are_new = false;
    for (int i = 1; i < GAME_SIZE; ++i)
    {
      double best_prob = 0;
      int best_bet = old_rate[i];

      for (int j = 1; (j <= i) && (i + j <= GAME_SIZE); ++j)
      {  
        double wins_prob = HEAD_PROB * probabilities[i - j] + (1 - HEAD_PROB) * probabilities[i + j];
        if (wins_prob - best_prob > EPSILON)
        {
          best_prob = wins_prob;
          best_bet = j;
        }
        if (best_bet != old_rate[i])
        {
          rate[i] = best_bet;
          if (std::abs(best_prob - old_probabilities[i]) > EPSILON)
            bets_are_new = true;
        }
      }
    }
    old_probabilities = probabilities;

    // std::cout << "get_number: " << std::endl;
    // for (int i = 1; i < GAME_SIZE; ++i)
    //   std::cout << get_number[i] << "\t ";
    // std::cout << std::endl;

    // std::cout << "wins_number: " << std::endl;
    // for (int i = 1; i < GAME_SIZE; ++i)
    //   std::cout << wins_number[i] << "\t ";
    // std::cout << std::endl;

    // std::cout << "probabilities: " << std::endl;
    // for (int i = 1; i < GAME_SIZE; ++i)
    //   std::cout << probabilities[i] << "\t ";
    // std::cout << std::endl;

    // // std::cout << "rate: " << std::endl;
    // for (int i = 1; i < GAME_SIZE; ++i)
    //   std::cout << i << "\t ";
    // std::cout << std::endl;

    // std::cout << "rate: " << std::endl;
    ++counter;
    std::cout << counter << ") \t";
    for (int i = 1; i < GAME_SIZE; ++i)
      std::cout << rate[i] << "\t ";
    std::cout << std::endl;
    
  }

  return 0;
}