#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

//  БАЗОВЫЙ КЛАСС 
class IntArray {
private:
    int* data;
    int size;

public:
    // Конструктор
    IntArray(int arraySize) {
        if (arraySize <= 0) {
            size = 1; // Минимальный размер
        }
        else {
            size = arraySize;
        }

        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = 0;
        }
        cout << "Создан массив размера " << size << endl;
    }

    // Конструктор копирования
    IntArray(const IntArray& other) {
        size = other.size;
        data = new int[size];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        cout << "Создана копия массива размера " << size << endl;
    }

    // Виртуальный деструктор
    virtual ~IntArray() {
        delete[] data;
        cout << "Массив размера " << size << " удален" << endl;
    }

    // Вывод массива
    void print() const {
        cout << "Массив [размер: " << size << "]: ";
        for (int i = 0; i < size; i++) {
            cout << data[i];
            if (i < size - 1) cout << ", ";
        }
        cout << endl;
    }

    // Установка значения
    bool setValue(int index, int value) {
        if (index < 0 || index >= size) {
            cout << "Ошибка: индекс " << index << " выходит за границы массива [0, " << size - 1 << "]!" << endl;
            return false;
        }
        if (value < -100 || value > 100) {
            cout << "Ошибка: значение " << value << " должно быть в диапазоне [-100, 100]!" << endl;
            return false;
        }
        data[index] = value;
        cout << "Установлено значение " << value << " по индексу " << index << endl;
        return true;
    }

    // Получение значения
    bool getValue(int index, int& value) const {
        if (index < 0 || index >= size) {
            cout << "Ошибка: индекс " << index << " выходит за границы массива [0, " << size - 1 << "]!" << endl;
            return false;
        }
        value = data[index];
        return true;
    }

    // Добавление в конец
    void append(int value) {
        if (value < -100 || value > 100) {
            cout << "Ошибка: значение " << value << " должно быть в диапазоне [-100, 100]!" << endl;
            return;
        }
        int newSize = size + 1;
        int* newData = new int[newSize];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        newData[size] = value;
        delete[] data;
        data = newData;
        size = newSize;
        cout << "Значение " << value << " добавлено в конец массива. Новый размер: " << size << endl;
    }

    // Сложение массивов
    IntArray add(const IntArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        IntArray result(maxSize);
        for (int i = 0; i < maxSize; i++) {
            int sum = 0;
            if (i < size) sum += data[i];
            if (i < other.size) sum += other.data[i];
            if (sum < -100) sum = -100;
            if (sum > 100) sum = 100;
            result.data[i] = sum;
        }
        cout << "Выполнено сложение массивов" << endl;
        return result;
    }

    // Вычитание массивов
    IntArray subtract(const IntArray& other) const {
        int maxSize = (size > other.size) ? size : other.size;
        IntArray result(maxSize);
        for (int i = 0; i < maxSize; i++) {
            int diff = 0;
            if (i < size) diff += data[i];
            if (i < other.size) diff -= other.data[i];
            if (diff < -100) diff = -100;
            if (diff > 100) diff = 100;
            result.data[i] = diff;
        }
        cout << "Выполнено вычитание массивов" << endl;
        return result;
    }

    // Получение размера
    int getSize() const { return size; }

    // Доступ к элементу для наследников
    int getElement(int index) const {
        if (index >= 0 && index < size) return data[index];
        return 0;
    }

    // Виртуальный метод сохранения
    virtual void saveToFile() {
        cout << "Базовый метод saveToFile()" << endl;
    }

    // Получение текущей даты-времени
protected:
    string getCurrentDateTime() const {
        time_t now = time(0);
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", &timeinfo);
        return string(buffer);
    }
};

// НАСЛЕДНИК ДЛЯ TXT
class ArrTxt : public IntArray {
public:
    ArrTxt(int size) : IntArray(size) {}

    void saveToFile() override {
        string filename = getCurrentDateTime() + ".txt";
        ofstream file(filename);
        if (file.is_open()) {
            file << "=== Массив в текстовом формате ===\n";
            file << "Размер: " << getSize() << "\n";
            file << "Элементы:\n";
            for (int i = 0; i < getSize(); i++) {
                file << "[" << i << "] = " << getElement(i) << "\n";
            }
            file.close();
            cout << "Сохранено в: " << filename << endl;
        }
        else {
            cout << "Ошибка создания файла!" << endl;
        }
    }
};

//  НАСЛЕДНИК ДЛЯ CSV 
class ArrCSV : public IntArray {
public:
    ArrCSV(int size) : IntArray(size) {}

    void saveToFile() override {
        string filename = getCurrentDateTime() + ".csv";
        ofstream file(filename);
        if (file.is_open()) {
            file << "Index;Value\n";
            for (int i = 0; i < getSize(); i++) {
                file << i << ";" << getElement(i) << "\n";
            }
            file.close();
            cout << "Сохранено в: " << filename << endl;
        }
        else {
            cout << "Ошибка создания файла!" << endl;
        }
    }
};

//  ГЛАВНАЯ ФУНКЦИЯ 
int main() {
    setlocale(LC_ALL, "RU");
    cout << " ПЗ 5 " << endl << endl;

    // 1. Тестирование базового класса
    cout << "1. Тестирование исходного класса IntArray:" << endl;
    IntArray original(3);
    original.setValue(0, 10);
    original.setValue(1, 50);
    original.setValue(2, -30);
    original.print();
    cout << endl;

    // 2. Класс для TXT файлов
    cout << "2. Создание ArrTxt:" << endl;
    ArrTxt txtArray(4);
    txtArray.setValue(0, 5);
    txtArray.setValue(1, 15);
    txtArray.setValue(2, -25);
    txtArray.setValue(3, 35);
    txtArray.print();
    txtArray.saveToFile();
    cout << endl;

    // 3. Класс для CSV файлов
    cout << "3. Создание ArrCSV:" << endl;
    ArrCSV csvArray(5);
    for (int i = 0; i < 5; i++) {
        csvArray.setValue(i, (i + 1) * 10);
    }
    csvArray.print();
    csvArray.saveToFile();
    cout << endl;

    // 4. Демонстрация полиморфизма
    cout << "4. Демонстрация полиморфизма:" << endl;
    IntArray* arrays[3];
    arrays[0] = new ArrTxt(3);
    arrays[1] = new ArrCSV(3);
    arrays[2] = new ArrTxt(2);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < arrays[i]->getSize(); j++) {
            arrays[i]->setValue(j, (j + 1) * 20);
        }
        cout << "Массив " << i + 1 << ": ";
        arrays[i]->print();
        arrays[i]->saveToFile(); // Полиморфный вызов
        delete arrays[i];
        cout << endl;
    }

    // 5. Операции с массивами
    cout << "5. Операции сложения и вычитания:" << endl;
    ArrTxt arrA(3);
    ArrTxt arrB(3);
    arrA.setValue(0, 10);
    arrA.setValue(1, 20);
    arrA.setValue(2, 30);
    arrB.setValue(0, 5);
    arrB.setValue(1, 15);
    arrB.setValue(2, 25);

    cout << "Массив A: ";
    arrA.print();
    cout << "Массив B: ";
    arrB.print();

    IntArray sum = arrA.add(arrB);
    cout << "A + B: ";
    sum.print();

    IntArray diff = arrA.subtract(arrB);
    cout << "A - B: ";
    diff.print();

    cout << "\n=== Программа завершена ===" << endl;
    return 0;
}