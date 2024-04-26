#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cctype>
using namespace std;

// база данных магазина
map<string, int> store;

// корзина покупок
map<string, int> cart;

// функция на проверку вводимых значений
string correctInput() 
{
    string article;
    cin >> article;
    while (article.find_first_not_of("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != string::npos) 
    {
        cin.clear();
        getline(cin, article);
        cout << "Ошибка: введен недопустимый символ в артикуле товара. Попробуйте снова: " << endl;
        cin >> article;
    }
    return article;
}

// функция для получения корректного количества товара
int correctQuantity() 
{
    int count;
    cin >> count;
    while (cin.fail() || count <= 0) 
    {
        cin.clear();
        string line;
        getline(cin, line);
        cout << "Ошибка: введено недопустимое значение для количества товара. Попробуйте снова: ";
        cin >> count;
    }
    return count;
}

// функция для добавления товара в корзину
void addproduct(const string& article, int quantity) 
{
    if (store.find(article) == store.end()) 
    {
        throw invalid_argument("Ошибка: товар с указанным артикулом не найден в магазине.");
    }
    if (quantity > store[article]) 
    {
        throw invalid_argument("Ошибка: запрошенное количество превышает доступное количество на складе.");
    }
    cart[article] += quantity;
    store[article] -= quantity;
    cout << "Товар успешно добавлен в корзину." << endl;
}

// функция для удаления товара из корзины
void removeproduct(const string& article, int quantity) 
{
    if (cart.find(article) == cart.end() || quantity > cart[article]) 
    {
        throw invalid_argument("Ошибка: товар с указанным артикулом не найден в корзине или указано неверное количество.");
    }
    cart[article] -= quantity;
    store[article] += quantity;
    cout << "Товар успешно удален из корзины." << endl;
}

// функция для сохранения состояния корзины в файл
void savecarttofile(const string& filename) 
{
    ofstream file(filename);
    if (file.is_open()) 
    {
        for (const auto& item : cart) 
        {
            file << item.first << "," << item.second << endl;
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
                cout << "Ошибка: товар с артикулом " << article << " не найден в магазине." << endl;
            } 
            else 
            {
                if (quantity > store[article]) 
                {
                    cout << "Ошибка: количество товара с артикулом " << article << " в файле превышает доступное количество на складе." << endl;
                } 
                else 
                {
                    cart[article] = quantity;
                    store[article] -= quantity;
                }
            }
        }
        file.close();
        cout << "Корзина покупок восстановлена из файла: " << filename << endl;
    } 
    else 
    {
        throw runtime_error("Ошибка: невозможно открыть файл для восстановления.");
    }
}

int main() 
{
    cout << "Введите количество товаров для добавления в базу данных магазина: ";
    int num_products = correctQuantity();

    for (int i = 0; i < num_products; ++i) 
    {
        string article_name;
        int quantity_1;

        cout << "Введите артикул товара " << i + 1 << ": ";
        article_name = correctInput();
        
        cout << "Введите количество этого товара " << i + 1 << ": ";
        quantity_1 = correctQuantity();

        store[article_name] = quantity_1; // добавление товара и его количества в магазин
    }

    // Восстановление корзины из файла
    string cart_filename = "cart_data.txt";
    try 
    {
        restorecartfromfile(cart_filename);
    } 
    catch (const exception& e) 
    {
        cout << e.what() << endl;
    }

    string article;

    // Добавление товара в корзину
    while (true) 
    {
        cout << "Введите артикул товара для добавления в корзину: ";
        article = correctInput();
        cout << "Введите количество товара для добавления: ";
        try 
        {
            addproduct(article, correctQuantity());
        } 
        catch (const exception& e) 
        {
            cout << e.what() << endl;
        }
        cout << "Вы хотите добавить еще товары в корзину? (Y/N): ";
        string str;
        cin >> str;
        if (str != "Y" && str != "y") 
        {
            break;
        }
    }

    // Удаление товара из корзины
    while (true) 
    {
        cout << "Введите артикул товара для удаления из корзины: ";
        article = correctInput();
        cout << "Введите количество товара для удаления: ";
        try 
        {
            removeproduct(article, correctQuantity());
        } 
        catch (const exception& e) 
        {
            cout << e.what() << endl;
        }
        cout << "Вы хотите удалить еще товары из корзины? (Y/N): ";
        string str1;
        cin >> str1;
        if (str1 != "Y" && str1 != "y") 
        {
            break;
        }
    }

    // Сохранение состояния корзины в файл
    try 
    {
        savecarttofile(cart_filename);
    } 
    catch (const exception& e) 
    {
        cout << e.what() << endl;
    }
    return 0;
}