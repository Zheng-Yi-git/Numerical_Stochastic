#include "binomial_european.hpp"

int main()
{
    double S0 = 54.0;
    double K = 50.0;
    double r = 0.0375;
    double sigma = 0.29;
    double T = 1.0;
    double q = 0.01;
    // int N = 1280;
    std::vector<int> Ns = {10, 20, 40, 80, 160, 320, 640, 1280};
    std::string type = "put";

    double V_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_price"];
    std::cout << "Black-Scholes: " << V_BS << std::endl;

    // Binomial Tree
    std::cout << "Binomial Tree" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "Binomial", type);
        double price = binomial_european.getPrice();
        std::cout << "N = " << N << ", " << price << ", abs error: " << std::abs(price - V_BS) << std::endl;
    }

    // Average Binomial Tree
    std::cout << "Average Binomial Tree" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "AverageBinomial", type);
        double price = binomial_european.getPrice();
        std::cout << "N = " << N << ", " << price << ", abs error: " << std::abs(price - V_BS) << std::endl;
    }

    // BBS Tree
    std::cout << "BBS Tree" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBS", type);
        double price = binomial_european.getPrice();
        std::cout << "N = " << N << ", " << price << ", abs error: " << std::abs(price - V_BS) << std::endl;
    }

    // BBSR Tree
    std::cout << "BBSR Tree" << std::endl;
    for (int N : Ns)
    {
        if (N == 10)
        {
            continue;
        }
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBSR", type);
        double price = binomial_european.getPrice();
        std::cout << "N = " << N << ", " << price << ", abs error: " << std::abs(price - V_BS) << std::endl;
    }

    return 0;
}