#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// Enum for coffee sizes
enum Size { SMALL, MEDIUM, LARGE };

// Helper to convert enum to string
string sizeToString(Size s) {
    switch (s) {
        case SMALL: return "Small";
        case MEDIUM: return "Medium";
        case LARGE: return "Large";
        default: return "Unknown";
    }
}

/* O: Open/Closed Principle
Base Coffee class — new coffee types can be added by creating new subclasses
with their own recipes and base prices without changing existing code. */
class Coffee {
protected:
    string name;
    double basePrice;
    Size size;

public:
    Coffee(string n, double p, Size s) : name(n), basePrice(p), size(s) {}
    virtual ~Coffee() = default;

    string getName() const { return name; }
    Size getSize() const { return size; }

    // Calculates final price based on size
    virtual double getPrice() const {
        double multiplier = 1.0;
        switch (size) {
            case SMALL: multiplier = 1.0; break;
            case MEDIUM: multiplier = 1.35; break;
            case LARGE: multiplier = 1.5; break;
        }
        return basePrice * multiplier;
    }

    // Common interface for coffee preparation
    virtual void prepare() const {
        cout << "Preparing a " << sizeToString(size) << " " << name << "...\n";
    }
};

class Espresso : public Coffee {
public:
    // No size parameter — always Small
    Espresso(double p) : Coffee("Espresso", p, SMALL) {}

    void prepare() const override {
        cout << "Grinding coffee beans...\n";
        cout << "Brewing a shot of espresso...\n";
        cout << "Pouring into a small cup.\n";  // always small
    }
};

class Latte : public Coffee {
public:
    // Default size assumed as Medium
    Latte(double p, Size s = MEDIUM) : Coffee("Latte", p, s) {}

    void prepare() const override {
        cout << "Steaming milk...\n";
        cout << "Grinding coffee beans...\n";
        cout << "Brewing a shot of espresso...\n";
        cout << "Combining espresso with milk and adding foam.\n";
        cout << "Pouring into a " << sizeToString(size) << " cup.\n";
    }
};

/* D: Dependency Inversion Principle
Payment is an abstraction — CoffeeShop depends on it instead of concrete classes. */
class Payment {
public:
    virtual ~Payment() = default;
    virtual void pay(double amount) = 0;  // generic payment behavior
};

class CashPayment : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid $" << fixed << setprecision(2) << amount << " in cash\n";
    }
};

class CardPayment : public Payment {
private:
    string cardNumber;
public:
    CardPayment(string number) : cardNumber(number) {}
    void pay(double amount) override {
        cout << "Charged $" << fixed << setprecision(2) << amount << " to card ending with "
             << cardNumber.substr(cardNumber.size() - 4) << "\n";
    }
};

/* S: Single Responsibility Principle
CoffeeShop handles taking orders, preparing the coffee, and using a Payment method. */
class CoffeeShop {
private:
    string shopName;
    Payment* paymentMethod; // depends on abstraction, not a specific type
public:
    CoffeeShop(string name, Payment* method)
        : shopName(name), paymentMethod(method) {}

    // Handles full order: preparation + payment
    void orderCoffee(Coffee& coffee) {
        cout << "Welcome to " << shopName << "!\n";

        // Show order info
        cout << "Order: " << sizeToString(coffee.getSize()) << " "
             << coffee.getName() << " - $" << coffee.getPrice() << "\n";

        // Prepare the coffee using its own recipe
        coffee.prepare();

        // Pay using the chosen payment method
        paymentMethod->pay(coffee.getPrice());

        cout << "Thank you for your purchase!\n\n";
    }
};

int main() {
    // Create coffee orders with enum-based sizes
    Espresso yeOldeEspresso(2.0);
    Latte yeNewLatte(3.0);

    // Payment methods
    CashPayment cash;
    CardPayment card("1277448787638764");

    // Coffee shops with different payment methods
    CoffeeShop oldShop("Ye Olde Coffee", &cash);
    CoffeeShop newShop("Ye New Coffee", &card);

    // Place orders — coffee will be prepared automatically
    oldShop.orderCoffee(yeOldeEspresso);
    newShop.orderCoffee(yeNewLatte);

    return 0;
}

