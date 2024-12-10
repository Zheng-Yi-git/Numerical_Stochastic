#ifndef PRODUCT_SERVICES_HPP
#define PRODUCT_SERVICES_HPP

#include <iostream>
#include <vector>
#include <map>
#include "products.hpp"
#include "soa.hpp"

/**
 * Bond Product Service to own reference data over a set of bond securities.
 * Key is the productId string, value is a Bond.
 */
class BondProductService : public Service<string, Bond>
{

public:
    // BondProductService ctor
    BondProductService();

    // Return the bond data for a particular bond product identifier
    Bond &GetData(string productId);

    // Add a bond to the service (convenience method)
    void Add(Bond &bond);

    // Get all Bonds with the specified ticker
    std::vector<Bond> GetBonds(string &ticker);

private:
    map<string, Bond> bondMap; // cache of bond products
};

/**
 * Interest Rate Swap Product Service to own reference data over a set of IR Swap products.
 * Key is the productId string, value is an IRSwap.
 */
class IRSwapProductService : public Service<string, IRSwap>
{
public:
    // IRSwapProductService ctor
    IRSwapProductService();

    // Return the IR Swap data for a particular swap product identifier
    IRSwap &GetData(string productId);

    // Add an IR Swap to the service (convenience method)
    void Add(IRSwap &swap);

    // Get all Swaps with the specified fixed leg day count convention
    std::vector<IRSwap> GetSwaps(DayCountConvention _fixedLegDayCountConvention);

    // Get all Swaps with the specified fixed leg payment frequency
    std::vector<IRSwap> GetSwaps(PaymentFrequency _fixedLegPaymentFrequency);

    // Get all Swaps with the specified floating index
    std::vector<IRSwap> GetSwaps(FloatingIndex _floatingIndex);

    // Get all Swaps with a term in years greater than the specified value
    std::vector<IRSwap> GetSwapsGreaterThan(int _termYears);

    // Get all Swaps with a term in years less than the specified value
    std::vector<IRSwap> GetSwapsLessThan(int _termYears);

    // Get all Swaps with the specified swap type
    std::vector<IRSwap> GetSwaps(SwapType _swapType);

    // Get all Swaps with the specified swap leg type
    std::vector<IRSwap> GetSwaps(SwapLegType _swapLegType);

private:
    map<string, IRSwap> swapMap; // cache of IR Swap products
};

BondProductService::BondProductService()
{
    bondMap = map<string, Bond>();
}

Bond &BondProductService::GetData(string productId)
{
    return bondMap[productId];
}

void BondProductService::Add(Bond &bond)
{
    bondMap.insert(pair<string, Bond>(bond.GetProductId(), bond));
}

std::vector<Bond> BondProductService::GetBonds(string &ticker)
{
    std::vector<Bond> result;
    for (const auto &pair : bondMap)
    {
        if (pair.second.GetTicker() == ticker)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

IRSwapProductService::IRSwapProductService()
{
    swapMap = map<string, IRSwap>();
}

IRSwap &IRSwapProductService::GetData(string productId)
{
    return swapMap[productId];
}

void IRSwapProductService::Add(IRSwap &swap)
{
    swapMap.insert(pair<string, IRSwap>(swap.GetProductId(), swap));
}

std::vector<IRSwap> IRSwapProductService::GetSwaps(DayCountConvention _fixedLegDayCountConvention)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetFixedLegDayCountConvention() == _fixedLegDayCountConvention)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwaps(PaymentFrequency _fixedLegPaymentFrequency)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetFixedLegPaymentFrequency() == _fixedLegPaymentFrequency)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwaps(FloatingIndex _floatingIndex)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetFloatingIndex() == _floatingIndex)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwapsGreaterThan(int _termYears)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetTermYears() > _termYears)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwapsLessThan(int _termYears)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetTermYears() < _termYears)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwaps(SwapType _swapType)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetSwapType() == _swapType)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

std::vector<IRSwap> IRSwapProductService::GetSwaps(SwapLegType _swapLegType)
{
    std::vector<IRSwap> result;
    for (const auto &pair : swapMap)
    {
        if (pair.second.GetSwapLegType() == _swapLegType)
        {
            result.push_back(pair.second);
        }
    }
    return result;
}

#endif