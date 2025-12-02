#pragma once
#include <string>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

namespace auction
{
    /// Represents a single bid placed in an auction.
    /// Stores amount, bidder identity and the moment in time when the bid was created.
    class Bid {
    private:
        double amount;
        string bidderName;
        int bidderIndex;
        system_clock::time_point timestamp;

    public:
        Bid(double amt, const string &name, int index)
            : amount(amt), bidderName(name), bidderIndex(index), timestamp(system_clock::now()) {}

        double getAmount() const { return amount; }
        string getBidderName() const { return bidderName; }
        int getBidderIndex() const { return bidderIndex; }
        system_clock::time_point getTimestamp() const { return timestamp; }
    };

    /// Represents a participant in the auction.
    /// A bidder can be marked as "trusted" and is able to generate bid amounts on its own
    /// for simulation purposes.
    class Bidder {
    private:
        string name;
        int index;
        bool trusted;
        double currentBid;

    public:
        Bidder(const string &n, int idx, bool isTrusted = false)
            : name(n), index(idx), trusted(isTrusted), currentBid(0.0) {}

        string getName() const { return name; }
        int getIndex() const { return index; }
        bool isTrusted() const { return trusted; }
        double getCurrentBid() const { return currentBid; }
        void setCurrentBid(double bid) { currentBid = bid; }

        /// Generates a (usually) reasonable bid amount based on the current auction state.
        ///
        /// @param currentHighest  The current highest bid in the auction (0 if none).
        /// @param minBid          The minimum bid allowed for the item.
        /// @param increment       The required increment between valid bids.
        /// @return A randomly generated amount which may intentionally be invalid
        ///         (too low or wrong increment) to exercise the validation chain.
        double generateBidAmount(double currentHighest, double minBid, double increment) const {
            random_device rd;
            mt19937 gen(rd());

            double baseBid = (currentHighest > 0) ? currentHighest : minBid;
            uniform_real_distribution<> chanceDist(0.0, 1.0);
            double chance = chanceDist(gen);

            // 60% - Valid bid following increment rules
            if (chance < 0.6) {
                uniform_int_distribution<> multiplier(1, 5);
                return baseBid + (increment * multiplier(gen));
            }
            // 20% - Bid too low (will be rejected)
            else if (chance < 0.8) {
                uniform_real_distribution<> lowBid(minBid * 0.5, baseBid * 0.95);
                return lowBid(gen);
            }
            // 20% - Invalid increment (will be rejected)
            else {
                uniform_real_distribution<> invalidIncrement(increment * 0.2, increment * 0.8);
                return baseBid + invalidIncrement(gen);
            }
        }
    };

    /// Value object describing an item being auctioned.
    /// Contains display name, textual description and starting price.
    class AuctionItem {
    private:
        string name;
        string description;
        double startingPrice;

    public:
        AuctionItem(const string &n, const string &desc, double price)
            : name(n), description(desc), startingPrice(price) {}

        string getName() const { return name; }
        string getDescription() const { return description; }
        double getStartingPrice() const { return startingPrice; }
    };
}
