#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>

std::random_device rd;
std::mt19937 gen(rd());

std::uniform_int_distribution<> charDist('a', 'z'); // Generates integers between 1 and 9
std::uniform_int_distribution<> intDist(3, 9);      // Generates integers between 1 and 9
std::uniform_real_distribution<> dblDist(0.0, 1.0); // Generates floats between 0.0 and 1.0

class Symbol
{
public:
    Symbol(char l, size_t size)
        : literal(l), probabilities(size, {'\0', 0.0f})
    {
    }

    char literal;
    std::vector<std::pair<char, double>> probabilities;

    void CalculateProbabilities()
    {
        double total = std::accumulate(probabilities.begin(), probabilities.end(), 0.0,
                                       [](double acc, const std::pair<char, double> &p)
                                       {
                                           return acc + p.second;
                                       });
        for (auto &i : probabilities)
        {
            i.second /= total;
        }
        std::sort(probabilities.begin(), probabilities.end(), [](const auto &a, const auto &b)
                  { return a.second > b.second; });
    }
};

void PrintStats(const Symbol &symbol)
{
    std::cout << symbol.literal << ":\n";
    for (size_t i = 0; i < symbol.probabilities.size(); i++)
    {
        std::cout << "\t" << symbol.probabilities[i].first << ": " << symbol.probabilities[i].second << std::endl;
    }
    std::cout << std::endl;
}

void GenSymbolSequence(char out, const std::vector<Symbol> &alphabet)
{
    int wordLenght = intDist(gen);
    for (size_t i = 0; i < wordLenght; i++)
    {
        std::cout << out;

        double cumulative = 0.0;
        double entry = dblDist(gen);

        for (size_t j = 0; j < alphabet[out - 'a'].probabilities.size(); j++)
        {
            cumulative += alphabet[out - 'a'].probabilities[j].second;
            if (entry <= cumulative)
            {
                out = alphabet[out - 'a'].probabilities[j].first;
                break;
            }
        }
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
                alphabet[f - 'a'].probabilities[s - 'a'].second += 1;
                if (!alphabet[f - 'a'].probabilities[s - 'a'].first)
                {
                    alphabet[f - 'a'].probabilities[s - 'a'].first = s;
                }
                f = s;
            }
        }
    }

    for (auto &symbol : alphabet)
    {
        symbol.CalculateProbabilities();
        PrintStats(symbol);
    }

    for (size_t i = 0; i < 10; i++)
    {
        GenSymbolSequence(charDist(gen), alphabet);
    }

    file.close();
}