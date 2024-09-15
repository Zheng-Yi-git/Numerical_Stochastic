#include "binomial_european.hpp" 
#include "binomial_american.hpp"
#include "binomial_option.hpp"

void runBinomialCalculator(BinomialOption& binop, double V_actual, double D_actual, double G_actual, double T_actual, int N){
    binop.runSimulation();
    double price = binop.getPrice();
    double delta = binop.getDelta();
    double gamma = binop.getGamma();
    double theta = binop.getTheta();
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual)<< "\t" << gamma << "\t" <<std::abs(gamma - G_actual)<< "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}

// FOR VARIANCE REDUCTION ONLY
void runBinomialCalculator_VR(BinomialOption& binop, BinomialOption& binop2, double V_actual, double D_actual, double G_actual, double T_actual, double V_BS, double D_BS, double G_BS, double T_BS, int N){
    binop.runSimulation();
    binop2.runSimulation();
    double price = binop.getPrice() - binop2.getPrice() + V_BS;
    double delta = binop.getDelta() - binop2.getDelta() + D_BS;
    double gamma = binop.getGamma() - binop2.getGamma() + G_BS;
    double theta = binop.getTheta() - binop2.getTheta() + T_BS;
    std::cout << N << "\t" << price << "\t" << std::abs(price - V_actual) << "\t" << N * std::abs(price - V_actual) << "\t" << N * N * std::abs(price - V_actual) << "\t";
    std::cout << delta << "\t" << std::abs(delta - D_actual)<< "\t" << gamma << "\t" <<std::abs(gamma - G_actual)<< "\t" << theta << "\t" << std::abs(theta - T_actual) << std::endl;
}


int main()
{
    std::cout << std::fixed << std::setprecision(6);
    double S0 = 54.0;
    double K = 50.0;
    double r = 0.0375;
    double sigma = 0.29;
    double T = 1.0;
    double q = 0.01;
    // int N = 1280;
    std::vector<int> Ns = {10, 20, 40, 80, 160, 320, 640, 1280};
    std::string type = "put";
    std::vector<std::string> methods = {"Binomial", "AverageBinomial", "BBS", "BBSR"};

    // EUROPEAN OPTIONS
    std::cout << "\n-EUROPEAN OPTION\n"<< std::endl;
    double V_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_price"];
    double D_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_delta"];
    double G_BS = black_scholes_values(S0, K, T, sigma, r, q)["gamma"];
    double T_BS = black_scholes_values(S0, K, T, sigma, r, q)[type + "_theta"];
    std::cout << "V_BS: " << V_BS << std::endl;
    std::cout << "D_BS: " << D_BS << std::endl;
    std::cout << "G_BS: " << G_BS << std::endl;
    std::cout << "T_BS: " << T_BS << std::endl;
    /*
    // Binomial Tree
    std::cout << "Binomial Tree" << std::endl;
    // formatted print control the width as 12 char for each column
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "Binomial", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        // format: N, price, abs error, N * abs error, N^2 * abs error, delta, gamma, theta (using \t)
        // each at most 6 decimal places

        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << gamma << "\t" << theta << std::endl;
    }

    // Average Binomial Tree
    std::cout << "Average Binomial Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "AverageBinomial", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << gamma << "\t" << theta << std::endl;
    }

    // BBS Tree
    std::cout << "BBS Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBS", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << gamma << "\t" << theta << std::endl;
    }

    // BBSR Tree
    std::cout << "BBSR Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBSR", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << gamma << "\t" << theta << std::endl;
    }
    */
    
    for (std::string method : methods)
    {
        std::cout << method <<" Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns)
        {
            BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, method, type);
            runBinomialCalculator(binomial_european, V_BS, D_BS, G_BS, T_BS, N);
        }
    }
    
    // AMERICAN OPTIONS
    std::cout << "\n-AMERICAN OPTION\n"<< std::endl;
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
        std::cout << method <<" Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        //std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns)
        {
            BinomialAmerican binomial_american(S0, K, r, sigma, T, q, N, method, type);
            runBinomialCalculator(binomial_american, V_Exact, D_Exact, G_Exact, T_Exact, N);
        }
    }

    // AMERICAN OPTIONS -- Variance Reduction
    std::cout << "\n-AMERICAN OPTION - VARIANCE REDUCTION\n"<< std::endl;
    for (std::string method : methods)
    {
        std::cout << method <<" Tree" << std::endl;
        // formatted print control the width as 12 char for each column
        //std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tgamma\t\ttheta" << std::endl;
        std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tD abs error\tgamma\t\tG abs error\ttheta\t\tT abs error\t" << std::endl;
        for (int N : Ns)
        {
            BinomialAmerican binomial_american(S0, K, r, sigma, T, q, N, method, type);
            BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, method, type);
            runBinomialCalculator_VR(binomial_american, binomial_european, V_Exact, D_Exact, G_Exact, T_Exact, V_BS, D_BS, G_BS, T_BS, N);
        }
    }

    return 0;
}