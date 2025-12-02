#pragma once
#include <vector>
#include <memory>
#include "models/AuctionModels.hpp"

using namespace std;

namespace auction
{
    /// Interface for bid sorting strategies (Strategy pattern).
    /// Implementations must sort the given vector of bids in-place.
    class SortStrategy {
    public:
        virtual ~SortStrategy() = default;
        virtual void sort(vector<shared_ptr<Bid>> &bids) = 0;
        virtual string getName() const = 0;
    };

    /// QuickSort-based strategy that orders bids in descending order by amount.
    class QuickSort : public SortStrategy {
    private:
        /// Partitions the range [low, high] around a pivot, placing higher bids first.
        static int partition(vector<shared_ptr<Bid>> &bids, int low, int high) {
            double pivot = bids[high]->getAmount();
            int i = low - 1;

            for (int j = low; j < high; j++) {
                if (bids[j]->getAmount() >= pivot) {
                    i++;
                    swap(bids[i], bids[j]);
                }
            }
            swap(bids[i + 1], bids[high]);
            return i + 1;
        }

        /// Recursive helper implementing QuickSort on the given sub-range.
        static void quickSortHelper(vector<shared_ptr<Bid>> &bids, int low, int high) {
            if (low < high) {
                int pi = partition(bids, low, high);
                quickSortHelper(bids, low, pi - 1);
                quickSortHelper(bids, pi + 1, high);
            }
        }

    public:
        /// Sorts bids in-place in descending order using QuickSort.
        void sort(vector<shared_ptr<Bid>> &bids) override {
            if (!bids.empty()) {
                quickSortHelper(bids, 0, bids.size() - 1);
            }
        }

        string getName() const override { return "QuickSort"; }
    };

    /// MergeSort-based strategy that orders bids in descending order by amount.
    class MergeSort : public SortStrategy {
    private:
        /// Merges two sorted sub-ranges [left, mid] and [mid+1, right]
        /// into a single range sorted in descending order.
        static void merge(vector<shared_ptr<Bid>> &bids, int left, int mid, int right) {
            int n1 = mid - left + 1;
            int n2 = right - mid;

            vector<shared_ptr<Bid>> L(n1);
            vector<shared_ptr<Bid>> R(n2);

            for (int i = 0; i < n1; i++)
                L[i] = bids[left + i];
            for (int j = 0; j < n2; j++)
                R[j] = bids[mid + 1 + j];

            int i = 0, j = 0, k = left;

            while (i < n1 && j < n2) {
                if (L[i]->getAmount() >= R[j]->getAmount()) {
                    bids[k] = L[i];
                    i++;
                } else {
                    bids[k] = R[j];
                    j++;
                }
                k++;
            }

            while (i < n1) {
                bids[k] = L[i];
                i++;
                k++;
            }

            while (j < n2) {
                bids[k] = R[j];
                j++;
                k++;
            }
        }

        /// Recursive helper implementing MergeSort on the given sub-range.
        static void mergeSortHelper(vector<shared_ptr<Bid>> &bids, int left, int right) {
            if (left < right) {
                int mid = left + (right - left) / 2;
                mergeSortHelper(bids, left, mid);
                mergeSortHelper(bids, mid + 1, right);
                merge(bids, left, mid, right);
            }
        }

    public:
        /// Sorts bids in-place in descending order using MergeSort.
        void sort(vector<shared_ptr<Bid>> &bids) override {
            if (!bids.empty()) {
                mergeSortHelper(bids, 0, bids.size() - 1);
            }
        }

        string getName() const override { return "MergeSort"; }
    };

    /// Insertion sort strategy that orders bids in descending order by amount.
    class InsertionSort : public SortStrategy {
    public:
        /// Sorts bids in-place in descending order using insertion sort.
        void sort(vector<shared_ptr<Bid>> &bids) override {
            for (size_t i = 1; i < bids.size(); i++) {
                auto key = bids[i];
                int j = i - 1;

                while (j >= 0 && bids[j]->getAmount() < key->getAmount()) {
                    bids[j + 1] = bids[j];
                    j--;
                }
                bids[j + 1] = key;
            }
        }

        string getName() const override { return "InsertionSort"; }
    };

    /// Context class that uses a pluggable SortStrategy to order bids.
    /// Allows changing the sorting algorithm at runtime.
    class BidSorter {
    private:
        shared_ptr<SortStrategy> strategy;

    public:
        BidSorter() : strategy(nullptr) {}

        explicit BidSorter(shared_ptr<SortStrategy> initialStrategy)
            : strategy(initialStrategy) {}

        /// Sets or replaces the current sorting strategy.
        void setStrategy(shared_ptr<SortStrategy> newStrategy) {
            strategy = newStrategy;
        }

        /// Sorts the given bids using the active strategy.
        /// Throws if no strategy has been configured.
        void sortBids(vector<shared_ptr<Bid>> &bids) {
            if (!strategy) {
                throw runtime_error("No sorting strategy set! Call setStrategy() first.");
            }
            strategy->sort(bids);
        }

        /// Returns the name of the current strategy, or "None" if not set.
        string getCurrentStrategyName() const {
            return strategy ? strategy->getName() : "None";
        }

        /// Indicates whether a strategy has been configured.
        bool hasStrategy() const {
            return strategy != nullptr;
        }
    };
}
