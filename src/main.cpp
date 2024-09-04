#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <random>

class Symbol
{
public:
    Symbol(char l, size_t size)
        : literal(l), probabilities(size, 0.0f)
    {
    }

    char literal;
    std::vector<double> probabilities;

    void CalculateProbabilities()
    {
        double total = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
        for (auto &i : probabilities)
        {
            i = i / total;
        }
    }
};

void PrintStats(const Symbol &symbol)
{
    std::cout << symbol.literal << ":\n";
    for (size_t i = 0; i <= ('z' - 'a'); i++)
    {
        std::cout << "\t" << static_cast<char>('a' + i) << ": " << symbol.probabilities[i] << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    const std::string filename = "../data/words.txt";
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    std::vector<Symbol> alphabet;
    for (size_t i = 'a'; i <= 'z'; i++)
    {
        alphabet.emplace_back(std::move(Symbol(i, 'z' - 'a' + 1)));
    }

    std::string line;
    while (std::getline(file, line))
    {
        char f, s;
        for (size_t i = 1; i < line.size(); i++)
        {
            s = line[i], f = line[i - 1];
            if (std::isalpha(s))
            {
                alphabet[f - 'a'].literal = f;
                alphabet[f - 'a'].probabilities[s - 'a'] += 1;
                f = s;
            }
        }
    }

    for (auto &symbol : alphabet)
    {
        symbol.CalculateProbabilities();
        // PrintStats(symbol);
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> intDist(1, 9);      // Generates integers between 1 and 100
    std::uniform_real_distribution<> dblDist(0.0, 1.0); // Generates floats between 0.0 and 1.0

    for (size_t i = 0; i < intDist(gen); i++)
    {
        for (size_t j = 0; j < dblDist(gen); j++)
        {
        }
        file.close();
    }