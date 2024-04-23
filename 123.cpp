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


//функция на проверку вводимых значений
void correctInput(string str)
{
    if (str.find_first_not_of("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != string::npos) 
    {
        cerr << "ошибка: введен недопустимый символ в артикуле товара." << endl;
    }
}


void correctQuantity(int quantity)
{
    if (!(quantity) || quantity <= 0) 
    {
        cerr << "ошибка: введено недопустимое значение для количества товара." << endl;
    }
}


// функция для добавления товара в корзину
void addproduct(const string& article, int quantity) 
{
    if (store.find(article) == store.end()) 
    {
        throw invalid_argument("ошибка: товар с указанным артикулом не найден в магазине.");
    }
    if (quantity > store[article]) 
    {
        throw invalid_argument( "ошибка: запрошенное количество превышает доступное количество на складе.");
    }
    cart[article] += quantity;
    store[article] -= quantity;
    cout << "товар успешно добавлен в корзину." << endl;
}



// функция для удаления товара из корзины
void removeproduct(const string& article, int quantity) 
{
    if (cart.find(article) == cart.end() || quantity > cart[article]) 
    {
        throw invalid_argument("ошибка: товар с указанным артикулом не найден в корзине или указано неверное количество.");
    }
    cart[article] -= quantity;
    store[article] += quantity;
    cout << "товар успешно удален из корзины." << endl;
}



// функция для сохранения состояния корзины в файл
void savecarttofile(const string& filename) 
{
    ofstream file(filename);
    if (file.is_open()) 
    {
        for (const auto& item : cart) 
        {
            file <<"артикул товара: "<<  item.first << ", количество: " << item.second << endl;
        }
        file.close();
        cout << "состояние корзины сохранено в файл: " << filename << endl;
    } 
    else 
    {
        throw runtime_error("ошибка: невозможно открыть файл для сохранения.");
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
    
    cout << "введите количество товаров для добавления в базу данных магазина: ";
    int count;
    cin >> count;
    correctQuantity(count);
    for (int i = 0; i < count; ++i) 
    {
        string article_name;
        int quantity_1;

        cout << "введите артикул товара " << i + 1 << ": ";
        cin >> article_name;
        correctInput(article_name);
        cout << "введите количество этого товара " << i + 1 << ": ";
        cin >> quantity_1;
        correctQuantity(quantity_1);
        store[article_name] = quantity_1; // добавление товара и его количества в магазин
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

    // добавление товара в корзину
    m:cout << "введите артикул товара для добавления в корзину: ";
    cin >> article;
    correctInput(article);
    cout << "введите количество товара для добавления: ";
    cin >> quantity;
    correctQuantity(quantity);


//использование функции для добавленияц
    try 
    {
        addproduct(article, quantity);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }


    cout << "вы хотите добавить еще подукты в корзину?(+ или -)"<< endl;
    string str;
    cin >> str;
    if (str == "+")
    {
        goto m;
    }


    // удаление товара из корзины
    m1:cout << "введите артикул товара для удаления из корзины: ";
    cin >> article;
    cout << "введите количество товара для удаления: ";
    cin >> quantity;
    correctQuantity(quantity);


    try 
    {
        removeproduct(article, quantity);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }


    cout << "вы хотите удалить еще подукты из корзины?(+ или -)"<< endl;
    string str1;
    cin >> str1;
    if (str1 == "+")
    {
        goto m1;
    }


    // сохранение состояния корзины в файл
    try 
    {
        savecarttofile(cart_filename);
    } 
    catch (const exception& e) 
    {
        cerr << e.what() << endl;
    }


    return 0;
}