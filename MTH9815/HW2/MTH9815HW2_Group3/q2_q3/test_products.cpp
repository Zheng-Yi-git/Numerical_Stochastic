#include <iostream>
#include "products.hpp"
#include "productservice.hpp"
#include "futures.hpp"
#include "future_product_service.hpp"

using namespace std;

int main()
{
    // Create the 10Y treasury note
    date maturityDate(2025, Nov, 16);
    string cusip = "912828M56";
    Bond treasuryBond(cusip, CUSIP, "T", 2.25, maturityDate);

    // Create the 2Y treasury note
    date maturityDate2(2017, Nov, 5);
    string cusip2 = "912828TW0";
    Bond treasuryBond2(cusip2, CUSIP, "T", 0.75, maturityDate2);

    // Create a BondProductService
    BondProductService bondProductService;

    // Add Bonds to the BondProductService
    bondProductService.Add(treasuryBond);
    bondProductService.Add(treasuryBond2);

    // Retrieve and print Bonds by ticker
    string ticker = "T";
    vector<Bond> bonds = bondProductService.GetBonds(ticker);
    cout << "Bonds with ticker 'T':" << endl;
    for (const Bond &bond : bonds)
    {
        cout << bond.GetProductId() << " ==> " << bond << endl;
    }

    // Create Future objects
    Future euroDollarFuture1("FUT1", "EuroDollar", date(2025, Jan, 15), 100000, USD);
    Future euroDollarFuture2("FUT2", "EuroDollar", date(2026, Jan, 15), 100000, USD);
    Future bondFuture("FUT3", "US Treasury Bond", date(2027, Dec, 15), 100000, USD);

    // Create FutureProductService
    FutureProductService futureService;

    // Add futures to the service
    futureService.Add(euroDollarFuture1);
    futureService.Add(euroDollarFuture2);
    futureService.Add(bondFuture);

    // Retrieve and print futures
    cout << "\nFutures:" << endl;
    cout << "Future 1: " << futureService.GetData("FUT1") << endl;
    cout << "Future 2: " << futureService.GetData("FUT2") << endl;
    cout << "Future 3: " << futureService.GetData("FUT3") << endl;

    // Create the Spot 10Y Outright Swap
    date effectiveDate(2015, Nov, 16);
    date terminationDate(2025, Nov, 16);
    string outright10Y = "Spot-Outright-10Y";
    IRSwap outright10YSwap(outright10Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate, terminationDate, USD, 10, SPOT, OUTRIGHT);

    // Create the IMM 2Y Outright Swap
    date effectiveDate2(2015, Dec, 20);
    date terminationDate2(2017, Dec, 20);
    string imm2Y = "IMM-Outright-2Y";
    IRSwap imm2YSwap(imm2Y, THIRTY_THREE_SIXTY, THIRTY_THREE_SIXTY, SEMI_ANNUAL, LIBOR, TENOR_3M, effectiveDate2, terminationDate2, USD, 2, IMM, OUTRIGHT);

    // Create a IRSwapProductService
    IRSwapProductService swapProductService;

    // Add Swaps to the IRSwapProductService
    swapProductService.Add(outright10YSwap);
    swapProductService.Add(imm2YSwap);

    // Retrieve and print swaps by various attributes
    cout << "\nSwaps with Day Count Convention THIRTY_THREE_SIXTY:" << endl;
    vector<IRSwap> swaps = swapProductService.GetSwaps(THIRTY_THREE_SIXTY);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with Payment Frequency SEMI_ANNUAL:" << endl;
    swaps = swapProductService.GetSwaps(SEMI_ANNUAL);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with Floating Index LIBOR:" << endl;
    swaps = swapProductService.GetSwaps(LIBOR);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with term greater than 5 years:" << endl;
    swaps = swapProductService.GetSwapsGreaterThan(5);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with term less than 5 years:" << endl;
    swaps = swapProductService.GetSwapsLessThan(5);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with Swap Type SPOT:" << endl;
    swaps = swapProductService.GetSwaps(SPOT);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    cout << "\nSwaps with Swap Leg Type OUTRIGHT:" << endl;
    swaps = swapProductService.GetSwaps(OUTRIGHT);
    for (const IRSwap &swap : swaps)
    {
        cout << swap.GetProductId() << " ==> " << swap << endl;
    }

    return 0;
}