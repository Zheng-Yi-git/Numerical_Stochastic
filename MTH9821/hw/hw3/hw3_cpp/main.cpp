#include "binomial_european.hpp"

int main()
{
    std::cout << std::fixed << std::setprecision(8);
    double S0 = 54.0;
    double K = 50.0;
    double r = 0.0375;
    double sigma = 0.29;
    double T = 1.0;
    double q = 0.01;
    // int N = 1280;
    std::vector<int> Ns = {10, 20, 40, 80, 160, 320, 640, 1280};
    std::string type = "put";

    auto BS_res = black_scholes_values(S0, K, T, sigma, r, q);
    double V_BS = BS_res["put_price"];
    double delta_BS = BS_res["put_delta"];
    double gamma_BS = BS_res["gamma"];
    double theta_BS = BS_res["put_theta"];
    std::cout << "Black-Scholes: " << std::endl;
    std::cout << "Price: " << V_BS << std::endl;
    std::cout << "Delta: " << delta_BS << std::endl;
    std::cout << "Gamma: " << gamma_BS << std::endl;
    std::cout << "Theta: " << theta_BS << std::endl;

    // Binomial Tree
    std::cout << "Binomial Tree" << std::endl;
    // formatted print control the width as 12 char for each column
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tdelta abs error\tgamma\t\tgamma abs error\ttheta\t\ttheta abs error" << std::endl;
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

        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << std::abs(delta - delta_BS) << "\t" << gamma << "\t" << std::abs(gamma - gamma_BS) << "\t" << theta << "\t" << std::abs(theta - theta_BS) << std::endl;
    }

    // Average Binomial Tree
    std::cout << "Average Binomial Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tdelta abs error\tgamma\t\tgamma abs error\ttheta\t\ttheta abs error" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "AverageBinomial", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << std::abs(delta - delta_BS) << "\t" << gamma << "\t" << std::abs(gamma - gamma_BS) << "\t" << theta << "\t" << std::abs(theta - theta_BS) << std::endl;
    }

    // BBS Tree
    std::cout << "BBS Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tdelta abs error\tgamma\t\tgamma abs error\ttheta\t\ttheta abs error" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBS", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << std::abs(delta - delta_BS) << "\t" << gamma << "\t" << std::abs(gamma - gamma_BS) << "\t" << theta << "\t" << std::abs(theta - theta_BS) << std::endl;
    }

    // BBSR Tree
    std::cout << "BBSR Tree" << std::endl;
    std::cout << "N\tprice\t\tabs error\tN * abs error\tN^2 * abs error\tdelta\t\tdelta abs error\tgamma\t\tgamma abs error\ttheta\t\ttheta abs error" << std::endl;
    for (int N : Ns)
    {
        BinomialEuropean binomial_european(S0, K, r, sigma, T, q, N, "BBSR", type);
        binomial_european.runSimulation();
        double price = binomial_european.getPrice();
        double delta = binomial_european.getDelta();
        double gamma = binomial_european.getGamma();
        double theta = binomial_european.getTheta();
        std::cout << N << "\t" << price << "\t" << std::abs(price - V_BS) << "\t" << N * std::abs(price - V_BS) << "\t" << N * N * std::abs(price - V_BS) << "\t" << delta << "\t" << std::abs(delta - delta_BS) << "\t" << gamma << "\t" << std::abs(gamma - gamma_BS) << "\t" << theta << "\t" << std::abs(theta - theta_BS) << std::endl;
    }

    return 0;
}