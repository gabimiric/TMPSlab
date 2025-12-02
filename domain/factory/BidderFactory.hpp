#pragma once
#include <memory>
#include <vector>
#include <random>
#include "../models/AuctionModels.hpp"

using namespace std;

namespace auction
{
    /// Factory for creating single or multiple Bidder instances used in simulations.
    class BidderFactory {
    public:
        /// Creates a single bidder with the given properties.
        static shared_ptr<Bidder> createBidder(const string &name, int index, bool trusted = false) {
            return make_shared<Bidder>(name, index, trusted);
        }

        /// Creates a collection of bidders, randomly marking some as trusted.
        ///
        /// @param count             Total number of bidders to create.
        /// @param trustedPercentage Fraction in [0, 1] representing the probability that a bidder is trusted.
        static vector<shared_ptr<Bidder>> createMultipleBidders(int count, double trustedPercentage = 0.2) {
            vector<shared_ptr<Bidder>> bidders;
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0.0, 1.0);

            for (int i = 0; i < count; i++) {
                bool isTrusted = dis(gen) < trustedPercentage; // probabilistic assignment
                string name = "Bidder_" + to_string(i);
                bidders.push_back(createBidder(name, i, isTrusted));
            }

            return bidders;
        }
    };
}
