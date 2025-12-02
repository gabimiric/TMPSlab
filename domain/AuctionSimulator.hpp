#pragma once
#include "Auctioneer.hpp"
#include <thread>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace chrono;

namespace auction
{
    /**
     * AuctionSimulator - Handles simulation of bidder behavior for testing/demo.
     *
     * Separates simulation logic from the core Auctioneer domain class.
     * Spawns multiple threads, one per bidder, which periodically attempt bids
     * until a global inactivity timeout is reached.
     */
    class AuctionSimulator {
    private:
        shared_ptr<Auctioneer> auctioneer;  ///< Auction controller used to place bids and end the auction.
        bool simulationActive;              ///< Global flag used to stop all bidder threads.
        system_clock::time_point lastBidTime; ///< Time of the last successful bid.
        const int NO_BID_TIMEOUT = 2;       ///< Number of seconds without bids before the auction ends.
        mutex displayMutex;                 ///< Protects interleaved console output.

        // Simulate a single bidder's behavior (runs on its own thread).
        void simulateBidder(shared_ptr<Bidder> bidder)
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> delayDist(0.1, 5.0);
            uniform_real_distribution<> bidChanceDist(0.0, 1.0);

            while (simulationActive)
            {
                // Random delay between bid attempts
                this_thread::sleep_for(milliseconds(static_cast<int>(delayDist(gen) * 1000)));

                if (!simulationActive) break;

                // 30% chance to attempt a bid
                if (bidChanceDist(gen) < 0.3)
                {
                    // Bidder generates their own bid amount
                    double bidAmount = bidder->generateBidAmount(
                        auctioneer->getCurrentHighestBid(),
                        auctioneer->getMinimumBid(),
                        auctioneer->getBidIncrement()
                    );

                    // Try to place the bid through the auctioneer
                    string resultMessage;
                    bool success = auctioneer->attemptBid(bidAmount, bidder, resultMessage);

                    // Display bid attempt result
                    {
                        lock_guard<mutex> lock(displayMutex);
                        cout << resultMessage;

                        if (success)
                        {
                            lastBidTime = system_clock::now();
                        }
                    }
                }
            }
        }

        // Check for auction timeout - ends auction if no bids for NO_BID_TIMEOUT seconds.
        void checkTimeout()
        {
            while (simulationActive)
            {
                this_thread::sleep_for(milliseconds(500));

                auto secondsSinceLastBid = duration_cast<seconds>(system_clock::now() - lastBidTime).count();

                if (secondsSinceLastBid >= NO_BID_TIMEOUT && auctioneer->getCurrentHighestBid() > 0)
                {
                    simulationActive = false;
                    lock_guard<mutex> lock(displayMutex);
                    cout << "\n+========================================================+" << endl;
                    cout << "|  No new bids for " << NO_BID_TIMEOUT << " seconds. Auction ending!           |" << endl;
                    cout << "+========================================================+\n" << endl;
                }
            }
        }

    public:
        /// Constructs the simulator for the given auctioneer instance.
        explicit AuctionSimulator(shared_ptr<Auctioneer> auc)
            : auctioneer(auc), simulationActive(false), lastBidTime(system_clock::now()) {}

        /// Run the auction simulation with the provided bidders.
        /// Launches a dedicated thread per bidder plus a timeout watchdog thread.
        void runSimulation(vector<shared_ptr<Bidder>> bidders)
        {
            simulationActive = true;
            lastBidTime = system_clock::now();

            cout << "+========================================================+" << endl;
            cout << "|              AUCTION SIMULATION STARTED                |" << endl;
            cout << "+========================================================+" << endl;
            cout << "|  Item: " << auctioneer->getItemName() << string(46 - auctioneer->getItemName().length(), ' ') << "|" << endl;
            cout << "|  Starting Price: $" << fixed << setprecision(2) << auctioneer->getMinimumBid() << "                               |" << endl;
            cout << "|  Bid Increment: $" << fixed << setprecision(2) << auctioneer->getBidIncrement() << "                                |" << endl;
            cout << "|  Total Bidders: " << bidders.size() << "                                  |" << endl;
            cout << "|  Timeout: " << NO_BID_TIMEOUT << " seconds with no bids                    |" << endl;
            cout << "+========================================================+\n" << endl;

            // Start timeout thread
            thread timeoutThread(&AuctionSimulator::checkTimeout, this);

            // Start bidder threads
            vector<thread> bidderThreads;
            for (const auto &bidder : bidders)
            {
                bidderThreads.emplace_back(&AuctionSimulator::simulateBidder, this, bidder);
            }

            // Wait for timeout
            timeoutThread.join();

            // Stop bidder threads
            for (auto &t : bidderThreads)
            {
                if (t.joinable()) t.join();
            }

            // End the auction
            auctioneer->endAuction();
        }
    };
}
