#include <iostream> // дл€ cout и т.п.
#include <vector> // дл€ класса vector
#include <string> // дл€ класса string
#include <algorithm> // дл€ алгоритма сортировки
#include <time.h> // дл€ случайных величин
#include <math.h> // дл€ abs()
#include <conio.h>
#include <algorithm>

#define GA_POPSIZE		10048		// размер попул€ции
#define GA_MAXITER		16384		// максимальное число итераций
#define GA_ELITRATE		0.10f		// элитарность
#define GA_MUTATIONRATE	0.25f			// мутации
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE

#define GA_TARGET std::string("Prroffessorr")
int iteration = 0;
using namespace std;


struct ga_struct
{
	string str;						// строка
	unsigned int fitness;			// пригодность
};

typedef vector<ga_struct> ga_vector;

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = GA_TARGET.size();

	for (int i = 0; i<GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.str.erase();

		for (int j = 0; j<tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;

	for (int i = 0; i<GA_POPSIZE; i++) {
		fitness = 0;
		for (int j = 0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j]));
		}

		population[i].fitness = fitness;
	}
}

bool fitness_sort(ga_struct x, ga_struct y)
{
	return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i<esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
}

//мутаци€ с перестановкой
void mutate2(ga_struct &member)
{

	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	for (int i = 0; i < ipos; i++) {
		member.str[i] = ((member.str[i] + delta) % 122);
	}

}

//—кремблирование(≈ще не проверн до конца, использовать на ваш страх и риск)

void mutate3(ga_struct &member)
{

	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int spos = rand() % tsize;

	//—кремблирование всей строки
	std::size_t index = rand() % tsize;
	
	for (std::size_t j = 0; j < tsize - index; ++j)
	{
		int index = rand() % tsize;
		char temp = member.str[j];
		member.str[j] = member.str[abs(index)];
		member.str[abs(index)] = temp;
	}

}

//ћутаци€ с перестановкой двух элементов
void mutate4(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int spos = rand() % tsize;

	char temp;
	temp = member.str[tsize-ipos];
	member.str[tsize-ipos] = member.str[tsize-spos];
	member.str[tsize - spos] = temp;
}


//ћутаци€ трех генов
void mutate5(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;
	int spos = rand() % tsize;
	int esize = GA_POPSIZE * GA_ELITRATE;


	int tmp = member.str[tsize - ipos];//получаем послед элем масива

	member.str[abs(tsize - spos- ipos/2)] = tmp;//мен€ем конец с началом

	member.str[abs(tsize - spos)] = member.str[abs(tsize - spos- ipos/2)];//мен€ем середину с началом

	member.str[tsize - ipos] = member.str[abs(tsize - spos)];// мен€ем конец с началом


}
void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population, buffer, esize);
	

	// Mate the rest
	for (int i = esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		//One point crossover

		//buffer[i].str = population[i1].str.substr(0, spos) +
			//population[i2].str.substr(spos, esize - spos);

		//ћноготочечный кроссовер

		int cross = rand() % 10+1;
		
		if (cross > 5) {
			
			buffer[i].str = population[i2].str.replace(abs(tsize - spos-1), abs(spos - cross), population[i1].str, abs(tsize - spos-1), abs(spos-cross));
		}
		else  {
			
			buffer[i].str = population[i1].str.replace(abs(tsize-spos-1), abs(spos - cross), population[i2].str, abs(tsize - spos-1), abs(spos-cross));
		}

		if (rand() < GA_MUTATION) mutate3(buffer[i]);
	}
}

inline void print_best(ga_vector &gav)
{
	cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << " Iteration:" << iteration++ << endl;
}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}

int main()
{
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i = 0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// вычисл€ем пригодность
		sort_by_fitness(*population);		// сортируем попул€цию
		print_best(*population);		// выводим лучшую попул€цию

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// спариваем попул€ции
		swap(population, buffer);		// очищаем буферы
	}

	_getch();
	return 0;
}