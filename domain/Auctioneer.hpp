#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "models/AuctionModels.hpp"
#include "BidValidation.hpp"

using namespace std;
using namespace chrono;

namespace auction
{
    /// Event structure describing a successful bid placement.
    ///
    /// This is sent to observers using a push model so they don't need
    /// to query the Auctioneer for state.
    struct BidPlacedEvent {
        /// Name of the auction item.
        string itemName;
        /// The bid amount.
        double amount;
        /// Name of the bidder who placed the bid.
        string bidderName;
        /// Index of the bidder who placed the bid.
        int bidderIndex;
        /// Current active bidders indexed by their bidder index.
        map<int, shared_ptr<Bidder>> activeBidders;
        /// Index of the current winner.
        int currentWinnerIndex;
        /// The current highest bid amount.
        double currentHighestBid;
    };

    /// Event structure describing the final state when an auction ends.
    struct AuctionEndedEvent {
        /// Name of the auction item.
        string itemName;
        /// Name of the winner.
        string winnerName;
        /// Index of the winner.
        int winnerIndex;
        /// Final bid amount by the winner.
        double finalAmount;
        /// Total number of bids placed during the auction.
        int totalBids;
    };

    /// Observer interface used to receive auction events.
    /// Implementations can react to bid placement and auction end.
    class Observer {
    public:
        virtual ~Observer() = default;
        /// Called whenever a new bid has been accepted.
        virtual void onBidPlaced(const BidPlacedEvent &event) = 0;
        /// Called once when the auction is ended.
        virtual void onAuctionEnded(const AuctionEndedEvent &event) = 0;
    };

    /// Subject base class implementing thread-safe observer management.
    class Subject {
    protected:
        vector<shared_ptr<Observer> > observers; ///< List of attached observers.
        mutex observerMutex; ///< Mutex for thread-safe observer access.

    public:
        virtual ~Subject() = default;

        /// Registers an observer to receive future events.
        void attach(shared_ptr<Observer> observer)
        {
            lock_guard<mutex> lock(observerMutex);
            observers.push_back(observer);
        }

        /// Unregisters a previously attached observer.
        void detach(shared_ptr<Observer> observer)
        {
            lock_guard<mutex> lock(observerMutex);
            observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
        }

        /// Notifies all observers about a newly placed bid.
        void notifyBidPlaced(const BidPlacedEvent &event)
        {
            lock_guard<mutex> lock(observerMutex);
            for (auto &obs: observers)
            {
                obs->onBidPlaced(event);
            }
        }

        /// Notifies all observers that the auction has ended.
        void notifyAuctionEnded(const AuctionEndedEvent &event)
        {
            lock_guard<mutex> lock(observerMutex);
            for (auto &obs: observers)
            {
                obs->onAuctionEnded(event);
            }
        }
    };

    /// Core class that manages the auction state and applies validation rules.
    /// Inherits from Subject to broadcast events to observers (e.g., leaderboard).
    class Auctioneer : public Subject {
    private:
        string itemName; ///< Name of the auction item.
        double minimumBid; ///< Minimum allowed bid.
        double bidIncrement; ///< Required increment between valid bids.
        double currentHighestBid; ///< Current highest bid amount.
        string currentWinner; ///< Name of the current winner.
        int currentWinnerIndex; ///< Index of the current winner.
        vector<shared_ptr<Bid> > allBids; ///< All bids placed in the auction.
        map<int, shared_ptr<Bidder> > activeBidders; ///< Active bidders in the auction.
        shared_ptr<BidValidationChain> validator; ///< Bid validation chain.

    public:
        /// Constructs an auctioneer for a specific item.
        /// @param item       Display name of the auctioned item.
        /// @param minBid     Minimum allowed bid.
        /// @param increment  Required increment between valid bids.
        Auctioneer(const string &item, double minBid, double increment)
            : itemName(item), minimumBid(minBid), bidIncrement(increment),
              currentHighestBid(0.0), currentWinner(""), currentWinnerIndex(-1)
        {
            validator = make_shared<BidValidationChain>();
        }

        /// Validates and attempts to place a bid.
        ///
        /// @param amount        Proposed bid amount.
        /// @param bidder        Bidder attempting the bid.
        /// @param resultMessage Human-readable log of validation and outcome.
        /// @return true if the bid was accepted and became the new highest bid.
        bool attemptBid(double amount, shared_ptr<Bidder> bidder, string &resultMessage)
        {
            stringstream ss;
            ss << "[BID ATTEMPT] Bidder " << bidder->getIndex()
               << " bids $" << fixed << setprecision(2) << amount;

            string reason;
            bool isValid = validator->validateBid(amount, bidder, currentHighestBid, minimumBid, bidIncrement, reason);

            if (isValid)
            {
                ss << " - APPROVED by validation chain" << endl;
                if (placeBid(amount, bidder, true))
                {
                    ss << "  -> Bid ACCEPTED. New highest bid!" << endl;
                    resultMessage = ss.str();
                    return true;
                }
            }
            else
            {
                ss << " - REJECTED: " << reason << endl;
            }

            resultMessage = ss.str();
            return false;
        }

        /// Places a bid assuming all validation has already passed.
        /// Updates internal state and notifies observers.
        bool placeBid(double amount, shared_ptr<Bidder> bidder, bool validationPassed)
        {
            if (!validationPassed)
            {
                return false;
            }

            currentHighestBid = amount;
            currentWinner = bidder->getName();
            currentWinnerIndex = bidder->getIndex();

            auto bid = make_shared<Bid>(amount, bidder->getName(), bidder->getIndex());
            allBids.push_back(bid);

            bidder->setCurrentBid(amount);
            activeBidders[bidder->getIndex()] = bidder;

            // Create event with all necessary data for observers (push model)
            BidPlacedEvent event;
            event.itemName = itemName;
            event.amount = amount;
            event.bidderName = bidder->getName();
            event.bidderIndex = bidder->getIndex();
            event.activeBidders = activeBidders;
            event.currentWinnerIndex = currentWinnerIndex;
            event.currentHighestBid = currentHighestBid;

            notifyBidPlaced(event);
            return true;
        }

        /// Ends the auction and notifies observers with the final result.
        void endAuction()
        {
            if (!currentWinner.empty())
            {
                // Create event with all necessary data for observers (push model)
                AuctionEndedEvent event;
                event.itemName = itemName;
                event.winnerName = currentWinner;
                event.winnerIndex = currentWinnerIndex;
                event.finalAmount = currentHighestBid;
                event.totalBids = allBids.size();

                notifyAuctionEnded(event);
            }
            // Display is now handled by LeaderboardObserver
        }

        // Getters for current auction state - do not modify internal data.
        double getCurrentHighestBid() const { return currentHighestBid; }
        double getMinimumBid() const { return minimumBid; }
        double getBidIncrement() const { return bidIncrement; }
        vector<shared_ptr<Bid> > getAllBids() const { return allBids; }
        map<int, shared_ptr<Bidder> > getActiveBidders() const { return activeBidders; }
        string getCurrentWinner() const { return currentWinner; }
        int getCurrentWinnerIndex() const { return currentWinnerIndex; }
        string getItemName() const { return itemName; }
    };
}
