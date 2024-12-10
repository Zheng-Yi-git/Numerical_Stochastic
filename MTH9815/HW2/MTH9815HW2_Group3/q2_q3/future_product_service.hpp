#ifndef FUTURE_PRODUCT_SERVICE_HPP
#define FUTURE_PRODUCT_SERVICE_HPP

#include <iostream>
#include <map>
#include "products.hpp"
#include "soa.hpp"
#include "futures.hpp"

/**
 * Future Product Service to own reference data over a set of future securities.
 * Key is the productId string, value is a Future.
 */
class FutureProductService : public Service<string, Future>
{
public:
    // FutureProductService ctor
    FutureProductService();

    // Return the future data for a particular future product identifier
    Future &GetData(string productId);

    // Add a future to the service (convenience method)
    void Add(Future &future);

private:
    map<string, Future> futureMap; // cache of future products
};

FutureProductService::FutureProductService()
{
    futureMap = map<string, Future>();
}

Future &FutureProductService::GetData(string productId)
{
    return futureMap[productId];
}

void FutureProductService::Add(Future &future)
{
    futureMap.insert(pair<string, Future>(future.GetProductId(), future));
}

#endif