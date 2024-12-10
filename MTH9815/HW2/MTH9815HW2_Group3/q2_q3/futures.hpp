#ifndef FUTURE_HPP
#define FUTURE_HPP

#include "products.hpp"

/**
 * Modeling of a Future Product
 */
class Future : public Product
{
public:
    // Future constructor
    Future(string _productId, string _underlyingAsset, date _maturityDate, float _contractSize, Currency _currency);
    Future();

    // Return the underlying asset of the Future
    string GetUnderlyingAsset() const;

    // Return the maturity date of the Future
    date GetMaturityDate() const;

    // Return the contract size of the Future
    float GetContractSize() const;

    // Return the currency of the Future
    Currency GetCurrency() const;

    // Overload the << operator to print the Future
    friend ostream &operator<<(ostream &output, const Future &future);

private:
    string underlyingAsset; // Underlying asset of the Future
    date maturityDate;      // Maturity date of the Future
    float contractSize;     // Contract size of the Future
    Currency currency;      // Currency of the Future

    // Return a string representation of the currency
    string ToString(Currency currency) const;
};

/**
 * Modeling of a EuroDollar Future
 */
class EuroDollarFuture : public Future
{
public:
    // EuroDollarFuture constructor
    EuroDollarFuture(string _productId, date _maturityDate, float _contractSize);

    // Overload the << operator to print the EuroDollar Future
    friend ostream &operator<<(ostream &output, const EuroDollarFuture &future);
};

/**
 * Modeling of a Bond Future
 */
class BondFuture : public Future
{
public:
    // BondFuture constructor
    BondFuture(string _productId, string _underlyingBond, date _maturityDate, float _contractSize);

    // Return the underlying bond of the Bond Future
    string GetUnderlyingBond() const;

    // Overload the << operator to print the Bond Future
    friend ostream &operator<<(ostream &output, const BondFuture &future);

private:
    string underlyingBond; // Underlying bond for the Bond Future
};

// Implementation of Future class methods
Future::Future(string _productId, string _underlyingAsset, date _maturityDate, float _contractSize, Currency _currency) : Product(_productId, FUTURE)
{
    underlyingAsset = _underlyingAsset;
    maturityDate = _maturityDate;
    contractSize = _contractSize;
    currency = _currency;
}

Future::Future() : Product("", FUTURE)
{
}

string Future::GetUnderlyingAsset() const
{
    return underlyingAsset;
}

date Future::GetMaturityDate() const
{
    return maturityDate;
}

float Future::GetContractSize() const
{
    return contractSize;
}

Currency Future::GetCurrency() const
{
    return currency;
}

ostream &operator<<(ostream &output, const Future &future)
{
    output << "UnderlyingAsset: " << future.GetUnderlyingAsset()
           << " MaturityDate: " << future.GetMaturityDate()
           << " ContractSize: " << future.GetContractSize()
           << " Currency: " << future.ToString(future.GetCurrency());
    return output;
}

string Future::ToString(Currency currency) const
{
    switch (currency)
    {
    case USD:
        return "USD";
    case EUR:
        return "EUR";
    case GBP:
        return "GBP";
    default:
        return "Unknown Currency";
    }
}

// Implementation of EuroDollarFuture methods
EuroDollarFuture::EuroDollarFuture(string _productId, date _maturityDate, float _contractSize) : Future(_productId, "EuroDollar", _maturityDate, _contractSize, USD)
{
}

ostream &operator<<(ostream &output, const EuroDollarFuture &future)
{
    output << "EuroDollarFuture: " << static_cast<const Future &>(future);
    return output;
}

// Implementation of BondFuture methods
BondFuture::BondFuture(string _productId, string _underlyingBond, date _maturityDate, float _contractSize) : Future(_productId, "Bond", _maturityDate, _contractSize, USD)
{
    underlyingBond = _underlyingBond;
}

string BondFuture::GetUnderlyingBond() const
{
    return underlyingBond;
}

ostream &operator<<(ostream &output, const BondFuture &future)
{
    output << "BondFuture: " << future.GetUnderlyingBond() << " " << static_cast<const Future &>(future);
    return output;
}

#endif
