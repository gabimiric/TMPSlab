#include <iostream>
#include <memory>
#include <vector>
#include <iomanip>
#include <chrono>
#include "../domain/models/AuctionModels.hpp"
#include "../domain/Auctioneer.hpp"
#include "../domain/AuctionSimulator.hpp"
#include "../domain/AuctionObservers.hpp"
#include "../domain/SortingAlgorithms.hpp"
#include "../domain/factory/BidderFactory.hpp"

using namespace std;
using namespace auction;
using namespace chrono;

/// Entry point for the demo application.
///
/// The program runs in two phases:
/// 1. Starts a real-time auction simulation with multiple bidders, printing a live leaderboard.
/// 2. Demonstrates the Strategy pattern by sorting the same set of bids using different algorithms.
int main()
{
    // Change sort option: QuickSort, MergeSort, InsertionSort for the strategy demo
    auto sorter = make_shared<BidSorter>();
    sorter->setStrategy(make_shared<QuickSort>());

    // Create auction item being sold in the live simulation
    auto item = make_shared<AuctionItem>("Vintage Watch", "Rare 1950s timepiece", 100.0);

    // Create auctioneer with item details and validation chain
    auto auctioneer = make_shared<Auctioneer>(item->getName(), item->getStartingPrice(), 10.0);

    // Create bidders (80% trusted to reduce rejected bids in the simulation)
    auto bidders = BidderFactory::createMultipleBidders(100, 0.2);

    // Attach LeaderboardObserver with the chosen sorting strategy
    shared_ptr<SortStrategy> leaderboardStrategy;
    if (sorter->getCurrentStrategyName() == "QuickSort") {
        leaderboardStrategy = make_shared<QuickSort>();
    } else if (sorter->getCurrentStrategyName() == "MergeSort") {
        leaderboardStrategy = make_shared<MergeSort>();
    } else {
        leaderboardStrategy = make_shared<InsertionSort>();
    }

    auto leaderboardObserver = make_shared<LeaderboardObserver>(leaderboardStrategy);
    auctioneer->attach(leaderboardObserver);

    cout << "Auction starting with " << sorter->getCurrentStrategyName() << " for leaderboard sorting...\n" << endl;

    // Create simulator and run the auction simulation with many concurrent bidders
    auto simulator = make_shared<AuctionSimulator>(auctioneer);
    simulator->runSimulation(bidders);

    // Demonstrate Strategy Pattern with all sorting algorithms on the same data set
    cout << "\n\n=== Strategy Pattern Demo - Sorting 20 Random Bids ===" << endl;
    cout << "Demonstrating runtime strategy switching with the same sorter object\n" << endl;

    // Generate 20 random bids between $100 and $1000 for the strategy demo
    vector<shared_ptr<Bid>> originalBids;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(100.0, 1000.0);

    for (int i = 1; i <= 20; i++) {
        originalBids.push_back(make_shared<Bid>(dis(gen), "Bidder", i));
    }

    cout << "Original unsorted array:" << endl;
    for (size_t i = 0; i < originalBids.size(); i++) {
        cout << "  [" << setw(2) << i + 1 << "] $" << fixed << setprecision(2)
             << setw(7) << originalBids[i]->getAmount();
        if ((i + 1) % 5 == 0) cout << endl;
    }

    // Test QuickSort - using the same sorter object with a new strategy
    cout << "\n--- sorter->setStrategy(QuickSort) ---" << endl;
    auto quickSortBids = originalBids;   // work on a copy to keep original data unchanged
    sorter->setStrategy(make_shared<QuickSort>());
    sorter->sortBids(quickSortBids);
    cout << "Strategy: " << sorter->getCurrentStrategyName() << endl;
    for (size_t i = 0; i < quickSortBids.size(); i++) {
        cout << "  [" << setw(2) << i + 1 << "] $" << fixed << setprecision(2)
             << setw(7) << quickSortBids[i]->getAmount();
        if ((i + 1) % 5 == 0) cout << endl;
    }

    // Test MergeSort - changing strategy on the same sorter object
    cout << "\n--- sorter->setStrategy(MergeSort) ---" << endl;
    auto mergeSortBids = originalBids;   // reset to original before re-sorting
    sorter->setStrategy(make_shared<MergeSort>());
    sorter->sortBids(mergeSortBids);
    cout << "Strategy: " << sorter->getCurrentStrategyName() << endl;
    for (size_t i = 0; i < mergeSortBids.size(); i++) {
        cout << "  [" << setw(2) << i + 1 << "] $" << fixed << setprecision(2)
             << setw(7) << mergeSortBids[i]->getAmount();
        if ((i + 1) % 5 == 0) cout << endl;
    }

    // Test InsertionSort - changing strategy again on the same sorter object
    cout << "\n--- sorter->setStrategy(InsertionSort) ---" << endl;
    auto insertionSortBids = originalBids;   // again, keep original data intact
    sorter->setStrategy(make_shared<InsertionSort>());
    sorter->sortBids(insertionSortBids);
    cout << "Strategy: " << sorter->getCurrentStrategyName() << endl;
    for (size_t i = 0; i < insertionSortBids.size(); i++) {
        cout << "  [" << setw(2) << i + 1 << "] $" << fixed << setprecision(2)
             << setw(7) << insertionSortBids[i]->getAmount();
        if ((i + 1) % 5 == 0) cout << endl;
    }

    cout << "\n+========================================================+" << endl;
    cout << "Strategy Pattern: Same sorter object, different behaviors!" << endl;
    cout << "+========================================================+" << endl;

    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();

    return 0;
}
