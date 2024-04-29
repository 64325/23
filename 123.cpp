#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cctype>


// база данных магазина
std::map<std::string, int> store;

// корзина покупок
std::map<std::string, int> cart;

// функция на проверку вводимых значений
std::string correctInput()
{
    using namespace std;
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
    using namespace std;
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
void addproduct(const std::string& article, int quantity)
{
    using namespace std;
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
void removeproduct(const std::string& article, int quantity)
{
    using namespace std;
    if (cart.find(article) == cart.end() || quantity > cart[article])
    {
        throw invalid_argument("Ошибка: товар с указанным артикулом не найден в корзине или указано неверное количество.");
    }
    cart[article] -= quantity;
    store[article] += quantity;
    cout << "Товар успешно удален из корзины." << endl;
}

// функция для сохранения состояния корзины в файл
void savecarttofile(const std::string& filename)
{
    using namespace std;
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
void restoreCartFromFile(const std::string& filename, std::map<std::string, int>& cart)
{
    using namespace std;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        size_t pos = line.find(",");
        if (pos == string::npos)
        {
            cerr << "Неверный формат файла." << endl;
            continue;
        }
        string article = line.substr(0, pos);
        int quantity = stoi(line.substr(pos + 1));
        cart[article] += quantity;
    }

    file.close();
}


int main()
{
    using namespace std;
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
    string st;
    cout << "Вы хотите востановить прошлые данные в корзине?(Y/N): ";
    cin >> st;
    if (st == "Y" || st == "y")
    {
        try
        {
            restoreCartFromFile(cart_filename, cart);
        }
        catch (const exception& e)
        {
            cout << e.what() << endl;
        }
    }


    string article;
    int quantity;

    // Добавление товара в корзину
    while (true)
    {
        cout << "Введите артикул товара для добавления в корзину: ";
        article = correctInput();
        cout << "Введите количество товара для добавления: ";
        quantity = correctQuantity();
        try
        {
            addproduct(article, quantity);
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
        quantity = correctQuantity();
        try
        {
            removeproduct(article, quantity);
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