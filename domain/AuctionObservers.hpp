#pragma once
#include "Auctioneer.hpp"
#include "SortingAlgorithms.hpp"
#include <iostream>
#include <iomanip>
#include <mutex>

using namespace std;

namespace auction
{
    /**
     * LeaderboardObserver - Displays the current auction leaderboard.
     *
     * This observer receives all necessary data through the BidPlacedEvent (push model)
     * and does not need to query the Auctioneer for information.
     * Uses Strategy pattern for sorting the leaderboard.
     */
    class LeaderboardObserver : public Observer {
    private:
        mutex displayMutex;                 ///< Protects concurrent console writes.
        shared_ptr<SortStrategy> sortStrategy; ///< Strategy used to sort leaderboard bids.

    public:
        /// Constructs an observer with an initial sorting strategy.
        explicit LeaderboardObserver(shared_ptr<SortStrategy> strategy) : sortStrategy(strategy) {}

        /// Changes the sorting strategy used for the leaderboard.
        void setSortStrategy(shared_ptr<SortStrategy> strategy) {
            sortStrategy = strategy;
        }

        /// Called whenever a new bid has been placed.
        /// Triggers a full leaderboard redraw.
        void onBidPlaced(const BidPlacedEvent &event) override
        {
            displayLeaderboard(event);
        }

        /// Called once when the auction ends; prints a compact summary.
        void onAuctionEnded(const AuctionEndedEvent &event) override
        {
            lock_guard<mutex> lock(displayMutex);

            cout << "\n+========================================================+" << endl;
            cout << "|              AUCTION ENDED - FINAL RESULTS             |" << endl;
            cout << "+========================================================+" << endl;
            cout << "|  WINNER: Bidder " << event.winnerIndex
                 << " with $" << fixed << setprecision(2) << event.finalAmount << "                          |" << endl;
            cout << "|  Total Bids: " << event.totalBids << "                                      |" << endl;
            cout << "+========================================================+" << endl;
        }

    private:
        /// Renders the current leaderboard using the configured sorting strategy.
        void displayLeaderboard(const BidPlacedEvent &event)
        {
            lock_guard<mutex> lock(displayMutex);

            cout << "\n+========================================================+\n";
            cout << "|              CURRENT AUCTION LEADERBOARD               |\n";
            cout << "+========================================================+\n";
            cout << "|  Using: " << (sortStrategy ? sortStrategy->getName() : "Default") << string(45 - (sortStrategy ? sortStrategy->getName().length() : 7), ' ') << "|\n";
            cout << "+========================================================+\n";
            cout << "|  Rank  |  Bidder  |   Current Bid   |    Status      |\n";
            cout << "+========================================================+\n";

            if (event.activeBidders.empty())
            {
                cout << "|                   No active bidders yet                |\n";
            }
            else
            {
                // Convert bidders to Bid objects for sorting
                vector<shared_ptr<Bid>> bidsForSorting;
                for (const auto &pair : event.activeBidders)
                {
                    bidsForSorting.push_back(make_shared<Bid>(
                        pair.second->getCurrentBid(),
                        pair.second->getName(),
                        pair.second->getIndex()
                    ));
                }

                // Use the sorting strategy
                if (sortStrategy) {
                    sortStrategy->sort(bidsForSorting);
                }

                int rank = 1;
                for (const auto &bid : bidsForSorting)
                {
                    string status = (bid->getBidderIndex() == event.currentWinnerIndex) ? "WINNING" : "Active";
                    cout << "|   " << setw(4) << left << rank++
                         << " | " << setw(8) << left << bid->getBidderIndex()
                         << " |  $" << setw(12) << fixed << setprecision(2) << left
                         << bid->getAmount()
                         << " | " << setw(14) << left << status << " |\n";
                }
            }

            cout << "+========================================================+\n" << endl;
        }
    };
}
