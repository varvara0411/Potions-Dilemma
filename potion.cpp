
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <random>

std::mutex mutex;
int ingredients[3] = {0, 0, 0};

class creature {
    public:
    // Существо добавляет два случайных ингредиента каждую секунду
    void addIngredients() {
        while (true) {
            mutex.lock();
            int first = rand() % 3;
            int second = rand() % 3;
            if (first != second) {
                ingredients[first]++;
                ingredients[second]++;
                std::cout << "Magical creature added ingredients: " << first << " and " << second << std::endl;
            }
            mutex.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

class master {
    private:
        int id;
    public:
        master(int id1) {
            id = id1;
        }
        void makePotion(int ing) {
            //Каждый мастер проверяет наличие всех трёх необходимых ингредиентов в массиве ингредиентов, 
            //и если они есть, делает зелье, уменьшает количество всех трёх использованных ингредиентов 
            //и выводит сообщение, содержащее идентификатор мастера зелий и номер, отвечающий за его изначальный (собственный) ингредиент
            while (true) {
                mutex.lock();
                if ((ingredients[ing] >= 2) && (ingredients[(ing + 1) % 3] >= 1) && (ingredients[(ing + 2) % 3] >= 1)) {
                    ingredients[ing] -= 1;
                    ingredients[(ing + 1) % 3] -= 1;
                    ingredients[(ing + 2) % 3] -= 1;
                    std::cout << "Master " << id << " made a potion with own ingredient: " << ing << std::endl;
                }
                mutex.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
};

int main() {
    creature t; // магическое существо
    master m1(1); // мастер
    master m2(2); // мастер
    master m3(3); // мастер
    std::thread t1([&t] {
        t.addIngredients();
    });
    std::thread t2([&m1] {
        m1.makePotion(0); // 0 - волос единорога
    });
    std::thread t3([&m2] {
        m2.makePotion(1); // 1 - перо феникса
    });
    std::thread t4([&m3] {
        m3.makePotion(2); // 2 - чешуя дракона
    });
    t1.join();
    t2.join(); 
    t3.join(); 
    t4.join();
    return 0;
}