#include "trinomial_european.hpp"
#include "trinomial_american.hpp"
#include "trinomial_option.hpp"
#include "binomial_european.hpp"
#include "binomial_american.hpp"
#include "binomial_option.hpp"
//#include "trinomial_implied_volatility.hpp"
#include <iostream>

void runBinomialCalculatorOnlyPrice(BinomialOption &binop, double V_actual, double D_actual, double G_actual, double T_actual, int N)
{
    binop.runSimulation();
    double price = binop.getPrice();
    std::cout << price << std::endl;
}
/*
void runTrinomialCalculatorOnlyPrice(TrinomialOption &trinop, double V_actual, double D_actual, double G_actual, double T_actual, int N)
{
    trinop.runSimulation();
    double price = trinop.getPrice();
    std::cout << price << std::endl;
}
*/

void runBinomialCalculator(BinomialOption &binop, double V_actual, double D_actual, double G_actual, double T_actual, int N)
{
    binop.runSimulation();
    double price = binop.getPrice();
    double delta = binop.getDelta();
    double gamma = binop.getGamma();
    double theta = binop.getTheta();
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual) << "\t" << gamma << "\t" << std::abs(gamma - G_actual) << "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}

void runTrinomialCalculator(TrinomialOption &trinop, double V_actual, double D_actual, double G_actual, double T_actual, int N)
{
    trinop.runSimulation();
    double price = trinop.getPrice();
    double delta = trinop.getDelta();
    double gamma = trinop.getGamma();
    double theta = trinop.getTheta();
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual) << "\t" << gamma << "\t" << std::abs(gamma - G_actual) << "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}

// FOR VARIANCE REDUCTION ONLY
void runBinomialCalculator_VR(BinomialOption &binop, BinomialOption &binop2, double V_actual, double D_actual, double G_actual, double T_actual, double V_BS, double D_BS, double G_BS, double T_BS, int N)
{
    binop.runSimulation();
    binop2.runSimulation();
    double price = binop.getPrice() - binop2.getPrice() + V_BS;
    double delta = binop.getDelta() - binop2.getDelta() + D_BS;
    double gamma = binop.getGamma() - binop2.getGamma() + G_BS;
    double theta = binop.getTheta() - binop2.getTheta() + T_BS;
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual) << "\t" << gamma << "\t" << std::abs(gamma - G_actual) << "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}

void runTrinomialCalculator_VR(TrinomialAmerican &triam, TrinomialEuropean &trieu, double V_actual, double D_actual, double G_actual, double T_actual, double V_BS, double D_BS, double G_BS, double T_BS, int N)
{
    triam.runSimulation();
    trieu.runSimulation();
    double price = triam.getPrice() - trieu.getPrice() + V_BS;
    double delta = triam.getDelta() - trieu.getDelta() + D_BS;
    double gamma = triam.getGamma() - trieu.getGamma() + G_BS;
    double theta = triam.getTheta() - trieu.getTheta() + T_BS;
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual) << "\t" << gamma << "\t" << std::abs(gamma - G_actual) << "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}

int main()
{
    std::cout << std::fixed << std::setprecision(6);
    double S0 = 41.0;
    double K = 43.0;
    double r = 0.035;
    double sigma = 0.24;
    double T = 1.0;
    double q = 0.0075;
    // int N = 1280;
    std::vector<int> Ns = {10, 20, 40, 80, 160, 320, 640, 1280};
    std::vector<int> Ns2 = {10, 20, 40, 80, 160, 320, 640, 1280, 10000, 20000};
    std::string type = "put";
    std::vector<std::string> methods = {"Trinomial", "TBS", "TBSR"};

    // EUROPEAN OPTIONS
    std::cout << "\n-EUROPEAN OPTION\n" << std::endl;
    double V_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_price"];
    double D_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_delta"];
    double G_BS = black_scholes_values(S0, K, T, sigma, r, q)["gamma"];
    double T_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_theta"];
    std::cout << "V_BS: " << V_BS << std::endl;
    std::cout << "D_BS: " << D_BS << std::endl;
    std::cout << "G_BS: " << G_BS << std::endl;
    std::cout << "T_BS: " << T_BS << std::endl;
    for (std::string method : methods)
    {
        std::cout << method << " Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns)
        {
            TrinomialEuropean trinomial_european(S0, K, r, sigma, T, q, N, method, type);
            runTrinomialCalculator(trinomial_european, V_BS, D_BS, G_BS, T_BS, N);
        }
    }

    // AMERICAN OPTIONS
    std::cout << "\n-AMERICAN OPTION\n" << std::endl;
    BinomialAmerican binomial_american(S0, K, r, sigma, T, q, 10000, "AverageBinomial", type);
    binomial_american.runSimulation();
    double V_Exact = binomial_american.getPrice();
    double D_Exact = binomial_american.getDelta();
    double G_Exact = binomial_american.getGamma();
    double T_Exact = binomial_american.getTheta();
    std::cout << "V_Exact  " << V_Exact << std::endl;
    std::cout << "D_Exact  " << D_Exact << std::endl;
    std::cout << "G_Exact  " << G_Exact << std::endl;
    std::cout << "T_Exact  " << T_Exact << std::endl;
    
    for (std::string method : methods)
    {
        std::cout << method << " Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        // std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns)
        {
            TrinomialAmerican trinomial_american(S0, K, r, sigma, T, q, N, method, type);
            runTrinomialCalculator(trinomial_american, V_Exact, D_Exact, G_Exact, T_Exact, N);
        }
    }

    // AMERICAN OPTIONS -- Variance Reduction
    std::cout << "\n-AMERICAN OPTION - VARIANCE REDUCTION\n"
              << std::endl;
    for (std::string method : methods)
    {
        std::cout << method << " Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        // std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns2)
        {
            TrinomialAmerican trinomial_american(S0, K, r, sigma, T, q, N, method, type);
            TrinomialEuropean trinomial_european(S0, K, r, sigma, T, q, N, method, type);
            runTrinomialCalculator_VR(trinomial_american, trinomial_european, V_Exact, D_Exact, G_Exact, T_Exact, V_BS, D_BS, G_BS, T_BS, N);
        }
    }

    // { // Implied Volatility Computation with Binomial Trees

    //     double marketPrice = 6.36; // Market Price of the put option
    //     double S0 = 58.0;          // current price of underlying stock
    //     double K = 60.0;           // strike price
    //     double r = 0.02;           // risk-free rate
    //     double T = 0.75;           // time of maturity, 9 months
    //     double q = 0.01;           // stock payment rate
    //     std::string type = "put";  // type of option

    //     // Find N
    //     int N_fixed = find_N_fixed(sigma, S0, K, r, T, q, type);

    //     // Calculate implied volatility
    //     double impliedVol = impliedVolatility_BBSR(marketPrice, S0, K, r, T, q, N_fixed, type);
    //     std::cout << "Implied Volatility (BBSR Model): " << impliedVol << std::endl;
    // }
    
    return 0;
}