#ifndef BinomialEuropean_hpp 
#define BinomialEuropean_hpp
#include "black_scholes.hpp"
#include "binomial_option.hpp"

#ifndef Option_value
#define Option_value
double option_value(double K, double S, std::string type)
{
    if (type == "call")
    {
        return std::max(0.0, S - K);
    }
    else if (type == "put")
    {
        return std::max(0.0, K - S);
    }
    else
    {
        std::cout << "Option type not found!" << std::endl;
    }
    return 0.0;
}
#endif

class BinomialEuropean : public BinomialOption
{
private:
    /*
    double S0, K, r, sigma, T, q;
    double dt;
    int N;
    double u, d, d_bar;
    double disc_p, disc_1p;
    std::string type;
    std::string method;
    */
    std::vector<std::string> methods{"Binomial", "AverageBinomial", "BBS", "BBSR"}; // KEEP THIS and below
    double S_temp;
    std::vector<double> V;
    double V10, V11, V20, V21, V22;
    double price, delta1, gamma1, theta1;

public:
    BinomialEuropean(double S0_, double K_, double r_, double sigma_, double T_, double q_, int N_, std::string method_, std::string type_)
        : BinomialOption(S0_, K_, r_, sigma_, T_, q_, N_, method_, type_)
    {
        if (method == "Binomial")
        {
            S_temp = S0 * pow(u, N);
        }
        else if (method == "AverageBinomial")
        {
            // do nothing
        }
        else if (method == "BBS")
        {
            S_temp = S0 * pow(u, N - 1);
        }
        else if (method == "BBSR")
        {
            // do nothing
        }
        else
        {
            std::cout << "Method not found!" << std::endl;
        }
    }

    // getter (use const to avoid changing the member variables)
    double getPrice() const { return price; }
    double getDelta() const { return delta1; }
    double getGamma() const { return gamma1; }
    double getTheta() const { return theta1; }

    void runSimulation()
    {
        if (method == "Binomial")
        {
            for (int i = 0; i <= N; i++)
            {
                V.push_back(option_value(K, S_temp, type));
                S_temp *= d_bar;
            }
            for (int j = N - 1; j >= 0; j--)
            {
                if (j == 0)
                {
                    V10 = V[0];
                    V11 = V[1];
                }
                if (j == 1)
                {
                    V20 = V[0];
                    V21 = V[1];
                    V22 = V[2];
                }
                for (int i = 0; i <= j; i++)
                {
                    V[i] = disc_p * V[i] + disc_1p * V[i + 1];
                }
            }
            // calculate price, delta, gamma, theta
            price = V[0];
            delta1 = (V10 - V11) / (S0 * (u - d));
            gamma1 = ((V20 - V21) / (S0 * u * (u - d)) - (V21 - V22) / (S0 * d * (u - d))) / (S0 * (u * u - d * d) / 2);
            theta1 = (V21 - price) / (2 * dt);
        }
        else if (method == "AverageBinomial")
        {
            BinomialEuropean tree_N = BinomialEuropean(S0, K, r, sigma, T, q, N, "Binomial", type);
            BinomialEuropean tree_Nplus1 = BinomialEuropean(S0, K, r, sigma, T, q, N + 1, "Binomial", type);
            tree_N.runSimulation();
            tree_Nplus1.runSimulation();
            price = (tree_N.getPrice() + tree_Nplus1.getPrice()) / 2;
            delta1 = (tree_N.getDelta() + tree_Nplus1.getDelta()) / 2;
            gamma1 = (tree_N.getGamma() + tree_Nplus1.getGamma()) / 2;
            theta1 = (tree_N.getTheta() + tree_Nplus1.getTheta()) / 2;
        }
        else if (method == "BBS")
        {
            for (int i = 0; i <= N - 1; i++)
            {
                V.push_back(black_scholes_values(S_temp, K, dt, sigma, r, q)[type + "_price"]);
                S_temp *= d_bar;
            }
            for (int j = N - 2; j >= 0; j--)
            {
                if (j == 0)
                {
                    V10 = V[0];
                    V11 = V[1];
                }
                if (j == 1)
                {
                    V20 = V[0];
                    V21 = V[1];
                    V22 = V[2];
                }
                for (int i = 0; i <= j; i++)
                {
                    V[i] = disc_p * V[i] + disc_1p * V[i + 1];
                }
            }
            // calculate price, delta, gamma, theta
            price = V[0];
            delta1 = (V10 - V11) / (S0 * (u - d));
            gamma1 = ((V20 - V21) / (S0 * u * (u - d)) - (V21 - V22) / (S0 * d * (u - d))) / (S0 * (u * u - d * d) / 2);
            theta1 = (V21 - price) / (2 * dt);
        }
        else if (method == "BBSR")
        {
            BinomialEuropean tree_N = BinomialEuropean(S0, K, r, sigma, T, q, N, "BBS", type);
            BinomialEuropean tree_halfN = BinomialEuropean(S0, K, r, sigma, T, q, N / 2, "BBS", type);
            tree_N.runSimulation();
            tree_halfN.runSimulation();
            price = 2 * tree_N.getPrice() - tree_halfN.getPrice();
            delta1 = 2 * tree_N.getDelta() - tree_halfN.getDelta();
            gamma1 = 2 * tree_N.getGamma() - tree_halfN.getGamma();
            theta1 = 2 * tree_N.getTheta() - tree_halfN.getTheta();
        }

        else
        {
            std::cout << "Method not found!" << std::endl;
            std::cout << "Method tried: " << method << std::endl;
        }
    }
};

#endif