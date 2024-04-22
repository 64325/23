#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <fstream>
using namespace std;

// База данных магазина
map<string, int> store;

// Корзина покупок
map<string, int> cart;

// Функция для добавления товара в корзину
void addProduct(const string& article, int quantity) 
{
    if (store.find(article) == store.end()) 
    {
        throw invalid_argument("Ошибка: товар с указанным артикулом не найден в магазине.");
    }
    if (quantity > store[article]) 
    {
        throw invalid_argument( "Ошибка: запрошенное количество превышает доступное количество на складе.");
    }
    cart[article] += quantity;
    store[article] -= quantity;
    cout << "Товар успешно добавлен в корзину." << endl;
}
// Функция для удаления товара из корзины
void removeProduct(const string& article, int quantity) 
{
    if (cart.find(article) == cart.end() || quantity > cart[article]) 
    {
        throw invalid_argument("Ошибка: товар с указанным артикулом не найден в корзине или указано неверное количество.");
    }
    cart[article] -= quantity;
    store[article] += quantity;
    cout << "Товар успешно удален из корзины." << endl;
}

// Функция для сохранения состояния корзины в файл
void saveCartToFile(const string& filename) 
{
    ofstream file(filename);
    if (file.is_open()) 
    {
        for (const auto& item : cart) 
        {
            file <<"Артикул товара: "<<  item.first << ", количество: " << item.second << endl;
        }
        file.close();
        cout << "Состояние корзины сохранено в файл: " << filename << endl;
    } 
    else 
    {
        throw runtime_error("Ошибка: невозможно открыть файл для сохранения.");
    }
}

// функция для восстановления корзины из файла
void restorecartfromfile(const string& filename) 
{
    ifstream file(filename);
    if (file.is_open()) 
    {
        string article;
        int quantity;
        while (file >> article >> quantity) 
        {
            if (store.find(article) == store.end()) 
            {
                cerr << "ошибка: товар с артикулом " << article << " не найден в магазине." << endl;
            } 
            else 
            {
                if (quantity > store[article]) 
                {
                    cerr << "ошибка: количество товара с артикулом " << article << " в файле превышает доступное количество на складе." << endl;
                } 
                else 
                {
                    cart[article] = quantity;
                    store[article] -= quantity;
                }
            }
        }
        file.close();
        cout << "корзина покупок восстановлена из файла: " << filename << endl;
    } 
    else 
    {
        throw runtime_error("ошибка: невозможно открыть файл для восстановления.");
    }
}

int main() {
    
    cout << "Введите количество товаров для добавления в Базу данных магазина: ";
    int count;
    cin >> count;

    for (int i = 0; i < count; ++i) 
    {
        string article_name;
        int quantity_1;

        cout << "Введите артикул товара " << i + 1 << ": ";
        cin >> article_name;

        cout << "Введите количество этого товара " << i + 1 << ": ";
        cin >> quantity_1;

        store[article_name] = quantity_1; // Добавление товара и его количества в магазин
    }

     // восстановление корзины из файла
    string cart_filename = "cart_data.txt";
    try 
    {
        restorecartfromfile(cart_filename);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }

    string article;
    int quantity;

    // Добавление товара в корзину
    m:cout << "Введите артикул товара для добавления в корзину: ";
    cin >> article;
    cout << "Введите количество товара для добавления: ";
    cin >> quantity;

    try 
    {
        addProduct(article, quantity);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }
    cout << "Вы хотите добавить еще подукты в корзину?(+ или -)"<< endl;
    string str;
    cin >> str;
    if (str == "+")
    {
        goto m;
    }

    // Удаление товара из корзины
    m1:cout << "Введите артикул товара для удаления из корзины: ";
    cin >> article;
    cout << "Введите количество товара для удаления: ";
    cin >> quantity;

    try 
    {
        removeProduct(article, quantity);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }
    cout << "Вы хотите удалить еще подукты из корзины?(+ или -)"<< endl;
    string str1;
    cin >> str1;
    if (str1 == "+")
    {
        goto m1;
    }
    // Сохранение состояния корзины в файл
    try 
    {
        saveCartToFile(cart_filename);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }
    return 0;
}
