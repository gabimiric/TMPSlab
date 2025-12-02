#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <cmath>
#include "models/AuctionModels.hpp"

using namespace std;

namespace auction
{
    /// Base class in a Chain of Responsibility for validating bids.
    /// Each concrete validator checks a single rule and forwards to the next one.
    class BidValidator {
    protected:
        shared_ptr<BidValidator> next; ///< Next validator in the chain.

    public:
        virtual ~BidValidator() = default;

        /// Sets the next validator in the chain.
        void setNext(shared_ptr<BidValidator> nextValidator) {
            next = nextValidator;
        }

        /// Validates a bid.
        /// @param amount        Proposed bid amount.
        /// @param bidder        Bidder attempting the bid.
        /// @param currentHighest Current highest bid amount (0 if none).
        /// @param minBid        Minimum allowed bid for the item.
        /// @param increment     Required increment between valid bids.
        /// @param reason        Human-readable reason when the bid is rejected.
        /// @return true if the bid passes this validator (and all subsequent ones).
        virtual bool validate(double amount, shared_ptr<Bidder> bidder, double currentHighest, double minBid,
                              double increment, string& reason) = 0;
    };

    /// Validator that ensures the bid is higher than the current highest (or minimum) bid.
    class MinimumBidValidator : public BidValidator {
    public:
        bool validate(double amount, shared_ptr<Bidder> bidder, double currentHighest, double minBid,
                      double increment, string& reason) override {
            double requiredBid = (currentHighest > 0) ? currentHighest : minBid;
            if (amount <= requiredBid) {
                reason = "Bid must be higher than $" + to_string(requiredBid);
                return false;
            }
            if (next) {
                return next->validate(amount, bidder, currentHighest, minBid, increment, reason);
            }
            return true;
        }
    };

    /// Validator that checks if the bid respects the configured increment step.
    class IncrementValidator : public BidValidator {
    public:
        bool validate(double amount, shared_ptr<Bidder> bidder, double currentHighest, double minBid,
                      double increment, string& reason) override {
            double baseBid = (currentHighest > 0) ? currentHighest : minBid;
            double diff = amount - baseBid;

            // Check if difference is a valid multiple of increment
            double ratio = diff / increment;
            double roundedRatio = round(ratio);

            // Allow small floating point errors (0.01)
            if (abs(ratio - roundedRatio) > 0.01 || roundedRatio < 1) {
                reason = "Bid must be in increments of $" + to_string(increment) +
                         " (e.g., $" + to_string(baseBid + increment) + ", $" +
                         to_string(baseBid + 2 * increment) + ", etc.)";
                return false;
            }

            if (next) {
                return next->validate(amount, bidder, currentHighest, minBid, increment, reason);
            }
            return true;
        }
    };

    /// Validator that ensures only trusted bidders are allowed to place bids.
    class TrustedBidderValidator : public BidValidator {
    public:
        bool validate(double amount, shared_ptr<Bidder> bidder, double currentHighest, double minBid,
                      double increment, string& reason) override {
            if (!bidder->isTrusted()) {
                reason = "Bidder " + bidder->getName() + " is not trusted";
                return false;
            }
            if (next) {
                return next->validate(amount, bidder, currentHighest, minBid, increment, reason);
            }
            return true;
        }
    };

    /// Convenience class that wires validators into a reusable validation chain.
    class BidValidationChain {
    private:
        shared_ptr<BidValidator> head; ///< First validator in the chain.

    public:
        BidValidationChain() {
            auto trustedValidator = make_shared<TrustedBidderValidator>();
            auto minValidator = make_shared<MinimumBidValidator>();
            auto incValidator = make_shared<IncrementValidator>();

            // Order: trust check -> minimum amount -> increment rule.
            trustedValidator->setNext(minValidator);
            minValidator->setNext(incValidator);

            head = trustedValidator;
        }

        /// Runs the configured validation chain on a bid.
        bool validateBid(double amount, shared_ptr<Bidder> bidder, double currentHighest, double minBid,
                         double increment, string& reason) {
            if (head) {
                return head->validate(amount, bidder, currentHighest, minBid, increment, reason);
            }
            return true;
        }
    };
}
