#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>
#include <vector>
#include <cmath>

class Generator
{
public:
    virtual double generate() = 0;
};

class LinearCongruentialGenerator : public Generator
{
private:
    long x;
    int a;
    int c;
    int k;

public:
    LinearCongruentialGenerator(int x0 = 1, int a = 39373, int c = 0, int k = pow(2, 31) - 1)
    {
        x = x0;
        this->a = a;
        this->c = c;
        this->k = k;
    }

    double generate()
    {
        x = (a * x + c) % k;
        return (double)x / k;
    }
};

// Beasley–Springer–Moro algorithm
double beasley_springer_moro_algo(double u)
{
    // Coefficients
    std::vector<double> a = {2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};
    std::vector<double> b = {-8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833};
    std::vector<double> c = {
        0.3374754822726147, 0.9761690190917186, 0.1607979714918209, 0.0276438810333863,
        0.0038405729373609, 0.0003951896511919, 0.0000321767881768, 0.0000002888167364,
        0.0000003960315187};

    double y = u - 0.5;
    double x;

    if (std::abs(y) < 0.42)
    {
        double r = y * y;
        x = y * (((a[3] * r + a[2]) * r + a[1]) * r + a[0]) /
            ((((b[3] * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    }
    else
    {
        double r = u;
        if (y > 0)
        {
            r = 1.0 - u;
        }
        r = std::log(-std::log(r));
        x = ((
                 (((((c[8] * r + c[7]) * r + c[6]) * r + c[5]) * r + c[4]) * r + c[3]) * r + c[2]) *
                 r +
             c[1]) *
                r +
            c[0];

        if (y < 0)
        {
            x = -x;
        }
    }

    return x;
}

// Standard Normal Generator by Inverse Transform
class BSMNormalGenerator
{
private:
    LinearCongruentialGenerator uniform_generator;

public:
    BSMNormalGenerator(LinearCongruentialGenerator uniform_generator)
    {
        this->uniform_generator = uniform_generator;
    }

    double generate()
    {
        double u = uniform_generator.generate();
        return beasley_springer_moro_algo(u);
    }
};

class AcceptanceRejectionNormalGenerator
{
private:
    LinearCongruentialGenerator uniform_generator;

public:
    AcceptanceRejectionNormalGenerator(LinearCongruentialGenerator uniform_generator)
    {
        this->uniform_generator = uniform_generator;
    }

    double generate()
    {
        while (true)
        {
            double u1 = uniform_generator.generate();
            double u2 = uniform_generator.generate();
            double u3 = uniform_generator.generate();

            double X = -std::log(u1);

            if (u2 <= std::exp(-0.5 * std::pow(X - 1, 2)))
            {
                if (u3 > 0.5)
                {
                    X = -X;
                }
                return X;
            }
        }
        return 0;
    }
};

class BoxMullerNormalGenerator
{
private:
    LinearCongruentialGenerator uniform_generator;

public:
    BoxMullerNormalGenerator(LinearCongruentialGenerator uniform_generator)
    {
        this->uniform_generator = uniform_generator;
    }

    double generate()
    {
        double u1 = 2 * uniform_generator.generate() - 1;
        double u2 = 2 * uniform_generator.generate() - 1;

        double X = u1 * u1 + u2 * u2;

        if (X <= 1)
        {
            double Y = std::sqrt(-2 * std::log(X) / X);
            double Z1 = u1 * Y;
            double Z2 = u2 * Y;

            return Z1;
        }
        else
        {
            return generate();
        }
    }
};

class MarsagliaBrayNormalGenerator
{
private:
    LinearCongruentialGenerator uniform_generator;

public:
    MarsagliaBrayNormalGenerator(LinearCongruentialGenerator uniform_generator)
    {
        this->uniform_generator = uniform_generator;
    }

    double generate()
    {
        while (true)
        {
            double X = 1.1;
            double u1 = 0;
            double u2 = 0;
            while (X > 1)
            {
                u1 = 2 * uniform_generator.generate() - 1;
                u2 = 2 * uniform_generator.generate() - 1;
                X = u1 * u1 + u2 * u2;
            }
            double Y = std::sqrt(-2 * std::log(X) / X);
            double Z1 = u1 * Y;
            double Z2 = u2 * Y;
            return Z1;
        }
    }
};

#endif