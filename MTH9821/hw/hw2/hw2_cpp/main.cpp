#include "generator.hpp"
#include "black_scholes.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <numeric>
#include <algorithm> 

int main()
{
    double K = 54.0;
    double S0 = 56.0;
    double T = 9.0 / 12.0;
    double r = 0.02;
    double q = 0.0;
    double sigma = 0.27;

    // compute BS price
    auto put_res = black_scholes_values(S0, K, T, sigma, r, q);
    std::cout << "price: " << put_res["put_price"] << std::endl;
    std::cout << "delta: " << put_res["put_delta"] << std::endl;
    std::cout << "vega: " << put_res["vega"] << std::endl;
    std::cout << "gamma: " << put_res["gamma"] << std::endl;

    { // Control Variate Technique
        std::vector<int> n;
        for (int i = 0; i < 10; ++i)
        {
            n.push_back(10000 * (1 << i)); // 1 << i 相当于 2^i
        }

        std::vector<double> V_CV_n_hat_list;
        std::vector<double> CV_error_list;

        auto uniform_generator = LinearCongruentialGenerator();
        auto normal_generator = MarsagliaBrayNormalGenerator(uniform_generator);

        for (auto nn : n)
        {
            std::vector<double> Si_list;
            std::vector<double> Vi_list;

            for (int i = 0; i < nn; ++i)
            {
                double zi = normal_generator.generate();
                double Si = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * zi);
                Si_list.push_back(Si);
                Vi_list.push_back(std::exp(-r * T) * std::max(K - Si, 0.0));
            }

            double Sn_hat = std::accumulate(Si_list.begin(), Si_list.end(), 0.0) / Si_list.size();
            double Vn_hat = std::accumulate(Vi_list.begin(), Vi_list.end(), 0.0) / Vi_list.size();

            // 计算协方差和方差
            double Si_mean = std::accumulate(Si_list.begin(), Si_list.end(), 0.0) / Si_list.size();
            double Vi_mean = std::accumulate(Vi_list.begin(), Vi_list.end(), 0.0) / Vi_list.size();

            double cov_Si_Vi = 0.0;
            double var_Si = 0.0;
            for (int i = 0; i < nn; ++i)
            {
                cov_Si_Vi += (Si_list[i] - Si_mean) * (Vi_list[i] - Vi_mean);
                var_Si += (Si_list[i] - Si_mean) * (Si_list[i] - Si_mean);
            }
            cov_Si_Vi /= (nn - 1);
            var_Si /= (nn - 1);

            double b_hat = cov_Si_Vi / var_Si;

            std::vector<double> Wi_list(nn);
            for (int i = 0; i < nn; ++i)
            {
                Wi_list[i] = Vi_list[i] - b_hat * (Si_list[i] - std::exp(r * T) * S0);
            }

            double V_CV_n_hat = std::accumulate(Wi_list.begin(), Wi_list.end(), 0.0) / Wi_list.size();
            V_CV_n_hat_list.push_back(V_CV_n_hat);

            double error = std::abs(V_CV_n_hat - put_res["put_price"]);
            CV_error_list.push_back(error);

            std::cout << "n: " << nn
                      << " V_CV_n_hat: " << V_CV_n_hat
                      << " error: " << error
                      << std::endl;
        }
    }

    { // Antithetic Variates Technique
        std::vector<int> n;
        for (int i = 0; i < 10; ++i)
        {
            n.push_back(10000 * (1 << i)); // 1 << i equals 2^i
        }

        std::vector<double> V_AV_n_hat_list;
        std::vector<double> AV_error_list;

        auto uniform_generator = LinearCongruentialGenerator();
        auto normal_generator = MarsagliaBrayNormalGenerator(uniform_generator);

        for (auto nn : n)
        {
            std::vector<double> Si1_list, Si2_list;
            std::vector<double> Vi1_list, Vi2_list;

            for (int i = 0; i < nn; ++i)
            {
                double zi = normal_generator.generate();
                double zi_minus = -zi;

                // 计算Si1和Si2
                double Si1 = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * zi);
                double Si2 = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * zi_minus);

                Si1_list.push_back(Si1);
                Si2_list.push_back(Si2);

                // 计算Vi1和Vi2
                Vi1_list.push_back(std::exp(-r * T) * std::max(K - Si1, 0.0));
                Vi2_list.push_back(std::exp(-r * T) * std::max(K - Si2, 0.0));
            }

            // 计算 V_AV_n_hat
            double mean_Vi1 = std::accumulate(Vi1_list.begin(), Vi1_list.end(), 0.0) / Vi1_list.size();
            double mean_Vi2 = std::accumulate(Vi2_list.begin(), Vi2_list.end(), 0.0) / Vi2_list.size();

            double V_AV_n_hat = 0.5 * (mean_Vi1 + mean_Vi2);
            V_AV_n_hat_list.push_back(V_AV_n_hat);

            // 计算误差
            double error = std::abs(V_AV_n_hat - put_res["put_price"]);
            AV_error_list.push_back(error);

            // 输出结果
            std::cout << "n: " << nn
                      << " V_AV_n_hat: " << V_AV_n_hat
                      << " error: " << error
                      << std::endl;
        }
    }
    { // Moment Matching Technique
        std::vector<int> n;
        for (int i = 0; i < 10; ++i)
        {
            n.push_back(10000 * (1 << i)); // 1 << i equals 2^i
        }

        std::vector<double> V_MM_n_hat_list;
        std::vector<double> MM_error_list;

        auto uniform_generator = LinearCongruentialGenerator();
        auto normal_generator = MarsagliaBrayNormalGenerator(uniform_generator);

        for (auto nn : n)
        {
            std::vector<double> Si_list;
            std::vector<double> Vi_list;

            for (int i = 0; i < nn; ++i)
            {
                double zi = normal_generator.generate();

                // Calculate Si
                double Si = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * zi);
                Si_list.push_back(Si);
            }

            // Calculate mean value S_hat
            double S_hat = std::accumulate(Si_list.begin(), Si_list.end(), 0.0) / nn;
            double Si_tilde;

            for (int i = 0; i < nn; ++i)
            {
                // Calculate Si_tilde
                Si_tilde = Si_list[i] * (std::exp(r * T) * S0 / S_hat);

                // Calculate Vi_tilde
                Vi_list.push_back(std::exp(-r * T) * std::max(K - Si_tilde, 0.0));
            }

            // Calculate V_MM_n_hat
            double V_MM_n_hat = std::accumulate(Vi_list.begin(), Vi_list.end(), 0.0) / nn;
            V_MM_n_hat_list.push_back(V_MM_n_hat);

            // Calculate error
            double error = std::abs(V_MM_n_hat - put_res["put_price"]);
            MM_error_list.push_back(error);

            // Output result
            std::cout << "n: " << nn
                << " V_MM_n_hat: " << V_MM_n_hat
                << " error: " << error
                << std::endl;
        }
    }

    { // Simultaneous Moment Matching and Control Variate Technique
        std::vector<int> n;
        for (int i = 0; i < 10; ++i)
        {
            n.push_back(100000 * (1 << i)); // 1 << i equals 2^i
        }

        std::vector<double> V_CV_MM_n_hat_list;
        std::vector<double> CV_MM_error_list;

        auto uniform_generator = LinearCongruentialGenerator();
        auto normal_generator = MarsagliaBrayNormalGenerator(uniform_generator);

        for (auto nn : n)
        {
            std::vector<double> Si_tilde_list(nn);
            for (int i = 0; i < nn; ++i)
            {
                double zi = normal_generator.generate();
                double Si = S0 * std::exp((r - 0.5 * sigma * sigma) * T + sigma * std::sqrt(T) * zi);
                Si_tilde_list[i] = Si;
            }

            // calculate S_hat, moment matching
            double S_hat = std::accumulate(Si_tilde_list.begin(), Si_tilde_list.end(), 0.0) / nn;
            std::vector<double> Vi_tilde_list(nn);

            for (int i = 0; i < nn; ++i)
            {
                double Si_tilde = Si_tilde_list[i] * (std::exp(r * T) * S0 / S_hat);
                Si_tilde_list[i] = Si_tilde;

                // calculate Vi_tilde
                Vi_tilde_list[i] = std::exp(-r * T) * std::max(K - Si_tilde, 0.0);
            }

            // calculate control variate coefficient b_hat
            double Si_mean = std::accumulate(Si_tilde_list.begin(), Si_tilde_list.end(), 0.0) / nn;
            double Vi_mean = std::accumulate(Vi_tilde_list.begin(), Vi_tilde_list.end(), 0.0) / nn;

            double cov_Si_Vi = 0.0;
            double var_Si = 0.0;
            for (int i = 0; i < nn; ++i)
            {
                cov_Si_Vi += (Si_tilde_list[i] - Si_mean) * (Vi_tilde_list[i] - Vi_mean);
                var_Si += (Si_tilde_list[i] - Si_mean) * (Si_tilde_list[i] - Si_mean);
            }
            cov_Si_Vi /= (nn - 1);
            var_Si /= (nn - 1);

            double b_hat = cov_Si_Vi / var_Si;

            std::vector<double> Wi_list(nn);

            for (int i = 0; i < nn; ++i)
            {
                double Wi = Vi_tilde_list[i] - b_hat * (Si_tilde_list[i] - std::exp(r * T) * S0);
                Wi_list[i] = Wi;
            }

            double V_CV_MM_n_hat = std::accumulate(Wi_list.begin(), Wi_list.end(), 0.0) / nn;
            V_CV_MM_n_hat_list.push_back(V_CV_MM_n_hat);

            // calculate error
            double error = std::abs(V_CV_MM_n_hat - put_res["put_price"]);
            CV_MM_error_list.push_back(error);

            // Output results
            std::cout << "n: " << nn
                << " V_CV_MM_n_hat: " << V_CV_MM_n_hat
                << " error: " << error
                << std::endl;
        }
    }

    return 0;
}