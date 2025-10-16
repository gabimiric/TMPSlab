# Tehnici și mecanisme de proiectare software
### Lab 1 - SOLID
### Author: Gabriel Miricinschi

------

## Objectives:

* Implement 3 SOLID letters(principles) in a simple project.;


## Used Design Patterns: 

* **S - Single Responsibility**: A class should have only one reason to change, meaning it should have a single, well-defined responsibility.
* **O - Open/Closed**: Software entities should be open for extension but closed for modification.
* **D - Dependency Inversion**: High-level modules should not depend on low-level mofules; both should depened on abstractions.

## Implementation

This project is a simulation of a coffee shop(s) with different types of coffee that can be ordered, prepared and paid for.
```c++
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
    virtual double getPrice() const {
        double multiplier = 1.0;
        switch (size) {
            case SMALL: multiplier = 1.0; break;
            case MEDIUM: multiplier = 1.35; break;
            case LARGE: multiplier = 1.5; break;
        }
        return basePrice * multiplier;
    }
    virtual void prepare() const {
        cout << "Preparing a " << sizeToString(size) << " " << name << "...\n";
    }
};

class Espresso : public Coffee {
public:
    Espresso(double p) : Coffee("Espresso", p, SMALL) {}
    void prepare() const override {
        cout << "Grinding coffee beans...\n";
        cout << "Brewing a shot of espresso...\n";
        cout << "Pouring into a small cup.\n";  // always small
    }
};
```
The coffee types are all created through the _Open/Closed Principle_ in order to leave room for later additions or other coffee types. `Coffee` acts as the parent class and is being extended into other coffee types instead of being directly modified.
```c++
class Payment {
public:
    virtual ~Payment() = default;
    virtual void pay(double amount) = 0;  // generic payment behavior
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
```
The payment methods were made through the _Dependency Inversion Principle_, with `Payment` being an abstract class that `CoffeeShop` depends on.
```c++
class CoffeeShop {
private:
    string shopName;
    Payment* paymentMethod;
public:
    CoffeeShop(string name, Payment* method)
        : shopName(name), paymentMethod(method) {}
    void orderCoffee(Coffee& coffee) {
        cout << "Welcome to " << shopName << "!\n";
        cout << "Order: " << sizeToString(coffee.getSize()) << " "
             << coffee.getName() << " - $" << coffee.getPrice() << "\n";
        coffee.prepare();
        paymentMethod->pay(coffee.getPrice());
        cout << "Thank you for your purchase!\n\n";
    }
};
```
Lastly, the `CoffeeShop` class uses the _Single Responsibility Principle_ by only being responsible for taking the order, requesting the payment(not processing it) and preparing the coffee.
```c++
int main() {
    Espresso yeOldeEspresso(2.0);
    Latte yeNewLatte(3.0);
    CashPayment cash;
    CardPayment card("1277448787638764");
    CoffeeShop oldShop("Ye Olde Coffee", &cash);
    CoffeeShop newShop("Ye New Coffee", &card);
    oldShop.orderCoffee(yeOldeEspresso);
    newShop.orderCoffee(yeNewLatte);
}
```
This is the `main` code used for testing. The default value of Latte is set to MEDIUM, while espressos are always SMALL.


## Results & Screenshots
<img width="374" height="381" alt="image" src="https://github.com/user-attachments/assets/fa4dd185-f3b5-4343-a854-20fd3044a9ee" />
