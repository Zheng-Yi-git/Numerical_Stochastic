#ifndef EUROPEANOPTION_H
#define EUROPEANOPTION_H

class EuropeanOption {
public:
    // Member variables
    double t;      // Current time
    double S;      // Current stock price
    double K;      // Strike price
    double T;      // Time to maturity
    double sigma;  // Volatility of the stock
    double r;      // Risk-free rate
    double q;      // Dividend yield

    // Constructor
    EuropeanOption();
    EuropeanOption(double _t, double _S, double _K, double _T, double _sigma, double _r, double _q);

    // Destructor
    ~EuropeanOption();
};

#endif // EUROPEANOPTION_H