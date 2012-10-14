#include <vector>
#include <random>
#include <set>
#include <iostream>

const int GAME_SIZE = 8;
const int GAME_NUMBER = 1000;


int main()
{
	int counter = 1;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0,1);

	std::vector<int> rate(GAME_SIZE, 0);
	for (int i = 1; i < GAME_SIZE; ++i)
		if(i <= GAME_SIZE / 2)
			rate[i] = i;
		else
			rate[i] = GAME_SIZE - i;

	bool bets_are_new = true;
	while(bets_are_new)
	{
		bets_are_new = false;

		std::vector<int> get_number(GAME_SIZE, 0);
		std::vector<int> wins_number(GAME_SIZE, 0);

		for (int i = 1; i < GAME_SIZE; ++i)
		{
			for (int current_game_number = 0; current_game_number < GAME_NUMBER; ++current_game_number)
			{
				int bet = rate[i];
				int money = i;
				std::set<int> positions;
				bool game_is_won = false;

				while(money > 0 && money < GAME_SIZE)
				{
					positions.insert(money);
					int coin_sign = distribution(generator);
					if(coin_sign == 1)
						money += bet;
					else
						money -= bet;

					if(money == GAME_SIZE)
						game_is_won = true;
					else if(money > 0)
						bet = rate[money];
				}

				for (std::set<int>::iterator it = positions.begin(); it != positions.end(); ++it)
				{
					if(game_is_won)
						wins_number[*it] += 1;
					get_number[*it] += 1;
				}
			}
		}

		std::vector<double> probabilities(GAME_SIZE + 1, 0);
		for (int i = 1; i < GAME_SIZE; ++i)
			probabilities[i] = static_cast<double>(wins_number[i]) / get_number[i];
		probabilities[GAME_SIZE] = 1;

		bets_are_new = false;
		for (int i = 1; i < GAME_SIZE; ++i)
		{
			double best_prob = 0;
			int best_bet = 0;

			for (int j = 1; j <= i && i + j <= GAME_SIZE; ++j)
			{	
				double wins_prob = 0.5 * probabilities[i - j] + 0.5 * probabilities[i + j];
				if(wins_prob > best_prob)
				{
					best_prob = wins_prob;
					best_bet = j;
				}
				if(best_bet != rate[i])
				{
					rate[i] = best_bet;
					bets_are_new = true;
				}
			}
		}


		// std::cout << "get_number: " << std::endl;
		// for (int i = 1; i < GAME_SIZE; ++i)
		// 	std::cout << get_number[i] << "\t ";
		// std::cout << std::endl;

		// std::cout << "wins_number: " << std::endl;
		// for (int i = 1; i < GAME_SIZE; ++i)
		// 	std::cout << wins_number[i] << "\t ";
		// std::cout << std::endl;

		// std::cout << "probabilities: " << std::endl;
		// for (int i = 1; i < GAME_SIZE; ++i)
		// 	std::cout << probabilities[i] << "\t ";
		// std::cout << std::endl;

		// // std::cout << "rate: " << std::endl;
		// for (int i = 1; i < GAME_SIZE; ++i)
		// 	std::cout << i << "\t ";
		// std::cout << std::endl;

		// std::cout << "rate: " << std::endl;
		++counter;
		std::cout << counter << ") \t";
		for (int i = 1; i < GAME_SIZE; ++i)
			std::cout << rate[i] << "\t ";
		// 
		// std::cout << "counter = " << counter << std::endl;
		std::cout << std::endl;
	}

	return 0;
}