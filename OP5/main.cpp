
/*
# Задание
Реализовать консольное приложение, предназначенное для работы с массивом
экземпляров класса.

# Входные данные: количество элементов массива, экземпляры класса.
# Выходные данные: массив экземпляров класса и массив результатов работы метода по
обработке данных.

# Особенности:
- у класса должны быть перегружены операторы взятия из потока и помещения в
поток;
- должна быть создана функция-член класса, реализующая обработку данных
экземпляра класса согласно варианту;
- должна быть реализована возможность выбора чтения из файла или с консоли;
- должна быть реализована возможность выбора записи в файл или вывода в
консоль.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

const std::string error = " Wrong input, please try again \n\n";
const std::string file_error = "\n Error. Unable to open the file!\n";
const std::string inv_time = " Time is not really\n";

struct Time
{
    int h, m;
    Time() : h(0), m(0) {}
    Time operator= (const Time& t)
    {
        h = t.h;
        m = t.m;
        return *this;
    }
    bool operator==(const Time& t)
    {
        if (h == t.h && m == t.m)
            return true;
        return false;
    }
    bool chek_time() const
    {
        if (h < 0 || h > 23 || m < 0 || m > 59)
            return false;
        return true;
    }
    Time get_time(std::string str)
    {
        h = std::stoi(str.substr(0, 2));
        m = std::stoi(str.substr(3, 2));
        return *this;
    }
    friend std::ostream& operator<< (std::ostream& out, const Time& t)
    {
        out << std::setfill('0') << std::setw(2) << t.h << ':' 
            << std::setw(2) << t.m << '\n' << std::setfill(' ');
        return out;
    }
};
class Trip
{
private:
    std::string point_1, point_2;
    Time T1, T2;
public:
    Time dif_time() const
    {
        Time t;
        t.h = T2.h - T1.h;
        t.m = T2.m - T1.m;
        if (t.m < 0)
        {
            t.m += 60;
            t.h--;
        }
        if (t.h < 0)
            t.h += 24;
        return t;
    }
    friend std::istream& operator>> (std::istream&, Trip&);
    friend std::ostream& operator<< (std::ostream&, const Trip&);
};
std::string ask_str(std::istream& in)
{
    std::string str;
    while (str.length() == 0)
        std::getline(in, str);
    return str;
}
bool chek(std::istream& in, std::string& str)
{
    while (str.length() == 0)
        std::getline(in, str);
    if (str.length() != 5)
        return false;
    for (int i = 0; i < str.length(); ++i)
        if ((str[i] > '9' || str[i] < '0') && str[2] != ':')
            return false;
    return true;
}
std::istream& operator>> (std::istream& in, Trip& tr)
{
    bool krit = true;
    while (krit)
    {
        try
        {
            std::cout << "\n-----------\n";
            std::string str;
            tr.point_1 = ask_str(in);

            if (!chek(in, str))
                throw inv_time;
            tr.T1.get_time(str);
            if (!tr.T1.chek_time())
                throw inv_time;
            str.clear();

            bool cont = true;
            while (cont)
            {
                tr.point_2 = ask_str(in);
                if (tr.point_1 == tr.point_2)
                    std::cout << " Select another point\n";
                else
                    cont = false;
            }

            while (!cont)
            {
                if (!chek(in, str))
                    throw inv_time;
                tr.T2.get_time(str);
                if (!tr.T2.chek_time() || tr.T2 == tr.T1)
                    throw inv_time;
                else
                    cont = true;
            }
            krit = false;
        }
        catch (const std::string inv_time)
        {
            std::cout << inv_time << "\n\n";
        }
    }
    return in;
}
std::ostream& operator<< (std::ostream& out, const Trip& tr)
{
    out << "\n-----------\n"
        << tr.point_1 << '\n' << tr.T1
        << tr.point_2 << '\n' << tr.T2;
    return out;
}
bool wrong_input(std::string input)
{
    bool invalid = false;
    if (input.length() == 0)
        invalid = true;
    for (int i = 0; i < input.length(); ++i)
        if (input[i] > '9' || input[i] < '0')
            invalid = true;
    if (invalid)
        std::cout << error;
    if (input.length() >= 9 && !invalid)
    {
        invalid = true;
        std::cout << " You have exceeded the limit, try again\n";
    }
    return invalid;
}
int ask_number(std::istream& in)
{
    std::string input;
    bool cont = true;
    while (cont)
    {
        std::getline(in, input);
        cont = wrong_input(input);
    }
    return std::stoi(input);
}
void timetable(std::istream& in, std::ostream& out)
{
    int n = ask_number(in);
    Trip* trips = new Trip[n];
    Time* differences = new Time[n];
    std::cout << "\n Input format:\n"
        << " Departure point\n Time (hh:mm)\n"
        << " Arrival point\n Time (hh:mm)\n\n";

    std::cout << "\n Input:\n";
    for (int i = 0; i < n; ++i)
    {
        in >> trips[i];
        differences[i] = trips[i].dif_time();
    }
    std::cout << "\n Output:\n";
    for (int i = 0; i < n; ++i)
        out << '\n' << trips[i] << "\nTrip time: " << differences[i] << '\n';
    delete[] trips, differences;
}
int main()
{
    std::cout << "\n Information about the train's trips\n";
    std::cout << "\n Select the input/output method\n"
        << " 1 - { cin / cout }\n"
        << " 2 - { cin / output.txt }\n"
        << " 3 - { input.txt / cout }\n"
        << " 4 - { input.txt / output.txt }\n";
    std::string method;
    while (method != "1" && method != "2" && method != "3" && method != "4")
    {
        std::cout << " > ";
        std::getline(std::cin, method);
        if (method != "1" && method != "2" && method != "3" && method != "4")
            std::cout << error;
    }
    std::ofstream fout;
    std::ifstream fin;
    switch (method[0])
    {
    case '1':
        std::cout << "\n Input the number of array elements: ";
        timetable(std::cin, std::cout);
        break;
    case '2':
        fout.open("output.txt", std::ios_base::trunc);

        if (!fout.is_open())
            std::cout << file_error;
        else
        {
            std::cout << "\n Input the number of array elements: ";
            timetable(std::cin, fout);
        }

        fout.close();
        break;
    case '3':
        fin.open("input.txt");

        if (!fin.is_open())
            std::cout << file_error;
        else
            timetable(fin, std::cout);

        fin.close();
        break;
    case '4':
        fout.open("output.txt", std::ios_base::trunc);
        fin.open("input.txt");

        if (!fout.is_open() || !fin.is_open())
            std::cout << file_error;
        else
            timetable(fin, fout);

        fout.close();
        fin.close();
        break;
    }
    std::cout << "\n\n End of the program.\n";
}
