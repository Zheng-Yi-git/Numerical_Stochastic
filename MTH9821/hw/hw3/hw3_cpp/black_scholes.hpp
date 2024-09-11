#ifndef BlackScholes_hpp
#define BlackScholes_hpp

#include <iostream>
#include <cmath>
#include <map>
#include <boost/math/distributions/normal.hpp>

std::map<std::string, double> black_scholes_values(double S, double K, double T, double sigma, double r, double q)
{
    using boost::math::normal;
    normal norm_dist(0.0, 1.0);

    // calculate d1 and d2
    double d1 = (std::log(S / K) + (r - q + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    // Call and Put price
    double call_price = S * std::exp(-q * T) * cdf(norm_dist, d1) - K * std::exp(-r * T) * cdf(norm_dist, d2);
    double put_price = K * std::exp(-r * T) * cdf(norm_dist, -d2) - S * std::exp(-q * T) * cdf(norm_dist, -d1);

    // Delta
    double delta_call = std::exp(-q * T) * cdf(norm_dist, d1);
    double delta_put = std::exp(-q * T) * (cdf(norm_dist, d1) - 1.0);

    // Vega
    double vega = S * std::exp(-q * T) * pdf(norm_dist, d1) * std::sqrt(T);

    // Gamma
    double gamma = std::exp(-q * T) * pdf(norm_dist, d1) / (S * sigma * std::sqrt(T));

    return {
        {"call_price", call_price},
        {"put_price", put_price},
        {"call_delta", delta_call},
        {"put_delta", delta_put},
        {"vega", vega},
        {"gamma", gamma}};
}

#endif