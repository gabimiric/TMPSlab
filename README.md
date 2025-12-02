# Tehnici și mecanisme de proiectare software

### Lab 3 - Behavioral Design Patterns

### Author: Gabriel Miricinschi

------

## Objectives

1. Study and understand Behavioral Design Patterns.
2. Implement at least 3 behavioral design patterns in the sample project.

## Implemented Behavioral Patterns

In this lab the domain was refactored around an **auction** scenario. The following behavioral patterns are used:

1. **Observer** – keeps the auction logic separate from the UI by sending event updates.
2. **Strategy** – encapsulates multiple bid-sorting algorithms and allows switching them at runtime.
3. **Chain of Responsibility** – builds a configurable pipeline that validates bids step by step.

Below is a short description of how each pattern appears in the code.

### 1. Observer Pattern – Live Auction Updates

**Intent:** Notify multiple views/observers when the auction state changes, without the `Auctioneer` knowing who is listening.

**Key classes (in `domain/Auctioneer.hpp` and `domain/AuctionObservers.hpp`):**

- `Observer` – interface with two callbacks:
  - `onBidPlaced(const BidPlacedEvent&)`
  - `onAuctionEnded(const AuctionEndedEvent&)`
- `Subject` – base class that manages a list of observers and provides:
  - `attach`, `detach` and notification methods (`notifyBidPlaced`, `notifyAuctionEnded`).
- `Auctioneer` – derives from `Subject` and represents the core auction controller:
  - Validates and accepts bids.
  - Emits `BidPlacedEvent` and `AuctionEndedEvent` to all attached observers.
- `LeaderboardObserver` – concrete observer that prints a textual leaderboard to the console:
  - Receives full event data (push model) and does not query `Auctioneer` directly.
  - Uses the current sorting strategy to order bidders before displaying them.

### 2. Strategy Pattern – Pluggable Sorting Algorithms

**Intent:** Define a family of algorithms, encapsulate each one, and make them interchangeable at runtime.

**Key classes (in `domain/SortingAlgorithms.hpp`):**

- `SortStrategy` – strategy interface:
  - `sort(vector<shared_ptr<Bid>>& bids)` sorts bids in-place.
  - `getName()` returns a human-readable algorithm name.
- `QuickSort`, `MergeSort`, `InsertionSort` – concrete strategies implementing different sorting algorithms
  that all sort **descending by bid amount**.
- `BidSorter` – context class that holds a `SortStrategy` and delegates `sortBids` to it.

**Usage (in `client/main.cpp` and `LeaderboardObserver`):**

- `LeaderboardObserver` receives a `shared_ptr<SortStrategy>` and uses it to sort active bidders
  before printing the leaderboard.
- In `main`, the same `BidSorter` instance is reused to demonstrate runtime strategy switching:
  - Start with `QuickSort`.
  - Switch to `MergeSort`.
  - Switch to `InsertionSort`.

### 3. Chain of Responsibility – Bid Validation Pipeline

**Intent:** Avoid coupling the sender of a request to its receiver by passing the request along a chain of
handlers until one of them handles it (or they all approve it).

**Key classes (in `domain/BidValidation.hpp`):**

- `BidValidator` – abstract base handler in the chain:
  - Holds a pointer to `next`.
  - Exposes `setNext` and a virtual `validate(...)` function.
- `TrustedBidderValidator` – first handler in the chain:
  - Rejects bids from untrusted bidders.
- `MinimumBidValidator` – second handler:
  - Ensures the new bid is strictly higher than the current highest (or minimum) bid.
- `IncrementValidator` – third handler:
  - Enforces that the bid is a valid multiple of the configured increment.
- `BidValidationChain` – convenience class that wires the concrete validators together in order and exposes
  a single `validateBid(...)` method.

**Usage (in `Auctioneer::attemptBid`)**

- Before a bid is accepted, `Auctioneer` calls `validator->validateBid(...)`.
- If any validator rejects the bid, the chain stops and a detailed textual reason is returned to the caller.
- Only when all validators approve, `Auctioneer::placeBid` is invoked and observers are notified.

## Code Structure Overview

The project is split into the following main areas:

- `client/main.cpp` – Demo entry point:
  - Sets up the auction item, `Auctioneer`, bidders, and `LeaderboardObserver`.
  - Runs a multi-threaded auction simulation using `AuctionSimulator`.
  - Demonstrates Strategy switching by sorting the same set of bids with each algorithm.
- `domain/models/AuctionModels.hpp` – Core models:
  - `Bid`, `Bidder`, `AuctionItem`.
- `domain/Auctioneer.hpp` – Auction controller, events, and Observer base types.
- `domain/AuctionObservers.hpp` – `LeaderboardObserver` implementation.
- `domain/SortingAlgorithms.hpp` – Strategy interface and concrete sorting strategies.
- `domain/BidValidation.hpp` – Chain of Responsibility for validating bids.
- `domain/AuctionSimulator.hpp` – Multi-threaded simulation for bidders and auction timeout.
- `domain/factory/BidderFactory.hpp` – Helper for creating many bidders with random trusted status.

