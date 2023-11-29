
/*
## Задание
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
- должна быть реализована возможность выбора записи в файла или вывода в
консоль.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

const std::string error = " Wrong input, please try again \n\n";
const std::string file_error = "\n Error. Unable to open the file!\n";
const std::string inv_time = " Time is not really\n Aborting\n";
const std::string numbers = "0123456789";

std::string ask_str(std::istream& in)
{
    std::string str;
    while (str.length() == 0)
        std::getline(in, str);
    return str;
}
bool chek(std::istream& in, std::string& str, char ch)
{
    str = ask_str(in);
    if (str.length() != 5 || str[2] != ch || str.find_first_not_of(numbers) != 2 || str.find_last_not_of(numbers) != 2)
        return false;
    return true;
}
bool wrong_input(std::string input)
{
    bool invalid = false;
    if (input.length() == 0)
        invalid = true;
    if (input.find_first_not_of(numbers) != std::string::npos)
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
class Time
{
private:
    int h, m;
public:
    Time() : h(0), m(0) {}
    Time(int h, int m) : h(h), m(m) {}
    Time(std::string& str)
    {
        h = std::stoi(str.substr(0, 2));
        m = std::stoi(str.substr(3, 2));
        if (!chek_time())
            throw inv_time;
    }
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
    bool operator<(const Time& t)
    {
        if (h < t.h)
            return true;
        else
            if (h == t.h && m < t.m)
                return true;
        return false;
    }
    bool operator>(const Time& t)
    {
        if (h > t.h)
            return true;
        else
            if (h == t.h && m > t.m)
                return true;
        return false;
    }
    bool chek_time() const
    {
        if (h < 0 || h > 23 || m < 0 || m > 59)
            return false;
        return true;
    }
    int get_h() const { return h; }
    int get_m() const { return m; }
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
        int h = T2.get_h() - T1.get_h();
        int m = T2.get_m() - T1.get_m();
        if (m < 0)
        {
            m += 60;
            h--;
        }
        if (h < 0)
            h += 24;
        Time t(h, m);
        return t;
    }
    std::string get_dep_point() const { return point_1; }
    std::string get_ar_point() const { return point_2; }
    friend std::istream& operator>> (std::istream&, Trip&);
    friend std::ostream& operator<< (std::ostream&, const Trip&);
};
std::istream& operator>> (std::istream& in, Trip& tr)
{
    bool krit = true;
    while (krit)
    {
        try
        {
            std::string str;
            tr.point_1 = ask_str(in);

            if (!chek(in, str, ':'))
                throw inv_time;
            Time T(str);
            tr.T1 = T;
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
                if (!chek(in, str, ':'))
                    throw inv_time;
                Time T(str);
                if (T < tr.T1)
                    throw inv_time;
                else
                    cont = true;
                tr.T2 = T;
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
    out << tr.point_1 << '\n' << tr.T1
        << tr.point_2 << '\n' << tr.T2;
    return out;
}
class Date
{
private:
    int month, day;
public:
    Date() : day(1), month(1) {}
    Date(std::string str)
    {
        day = std::stoi(str.substr(0, 2));
        month = std::stoi(str.substr(3, 2));
        if (!chek_date())
            throw inv_time;
    }
    Date operator= (const Date& d)
    {
        day = d.day;
        month = d.month;
        return *this;
    }
    bool chek_date() const
    {
        if (month < 1 || month > 12)
            return false;
        switch (month)
        {
        case 4:
        case 6:
        case 9:
        case 11:
            if (day < 1 || day > 30)
                return false;
            break;
        case 2:
            if (day < 1 || day > 28)
                return false;
            break;
        default:
            if (day < 1 || day > 31)
                return false;
            break;
        }
        return true;
    }
    friend std::ostream& operator<< (std::ostream& out, const Date& d)
    {
        out << std::setfill('0') << std::setw(2) << d.day << '.'
            << std::setw(2) << d.month << std::setfill(' ');
        return out;
    }
};
class Info
{
private:
    int num;
    Date d;
    Trip* tr;
public:
    Info() : num(0), tr(nullptr) {}
    ~Info() { delete[] tr; }
    std::string shortest_trip(Time*& trips) const
    {
        int a = 0;
        for (int q = 0; q < num; ++q)
        {
            Time T1(trips[q]);
            Time T2(trips[a]);
            a = (T1 < T2) ? q : a;
        }
        return tr[a].get_dep_point() + "---" + tr[a].get_ar_point();
    }
    int get_size() const { return num; }
    Trip trip(int i) const { return tr[i]; }
    Date get_date() const { return d; }
    friend std::istream& operator>> (std::istream&, Info&);
    friend std::ostream& operator<< (std::ostream&, const Info&);
};
std::istream& operator>> (std::istream& in, Info& i)
{
    bool krit = true;
    while (krit)
    {
        try
        {
            std::string str;
            if (!chek(in, str, '.'))
                throw inv_time;
            Date d(str);
            i.d = d;

            i.num = ask_number(in);
            i.tr = new Trip[i.num];
            for (int q = 0; q < i.num; ++q)
                in >> i.tr[q];
            krit = false;
        }
        catch (const std::string inv_time)
        {
            std::cout << inv_time << "\n\n";
        }
    }
    return in;
}
std::ostream& operator<< (std::ostream& out, const Info& i)
{
    out << "\n Output:\n"
        << "\n >>>  " << i.d << '\n'
        << "\n Number of train trips: " << i.num << '\n';
    return out;
}
void timetable(std::istream& in, std::ostream& out)
{
    int n = ask_number(in);
    Info* inf = new Info[n];
    Time** differences = new Time*[n];
    std::string* shortests_trips = new std::string[n];
    std::cout << "\n ## Input format\n\n"
        << " Date (dd.mm)\n"
        << " Number of trips\n\n"
        << " # Trip format\n"
        << " Departure point\n Time (hh:mm)\n"
        << " Arrival point\n Time (hh:mm)\n\n";

    for (int q = 0; q < n; ++q)
    {
        in >> inf[q];
        int size = inf[q].get_size();
        differences[q] = new Time[size];
        for (int a = 0; a < size; ++a)
            differences[q][a] = inf[q].trip(a).dif_time();
        shortests_trips[q] = inf[q].shortest_trip(differences[q]);
    }

    for (int q = 0; q < n; ++q)
    {
        out << inf[q];
        int size = inf[q].get_size();
        for (int a = 0; a < size; ++a)
            out << inf[q].trip(a) << "\n Trip time: " << differences[q][a] << '\n';
        out << "\n The shortest trip on " << inf[q].get_date() << " > " << shortests_trips[q];
        inf[q].~Info();
        delete[] differences[q];
    }
    delete[] differences, inf, shortests_trips;
}
int main()
{
    std::cout << "\n Information about the train's Trip\n";
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
        std::cout << "\n Input the number of dates: ";
        timetable(std::cin, std::cout);
        break;
    case '2':
        fout.open("output.txt", std::ios_base::trunc);
        if (!fout.is_open())
            std::cout << file_error;
        else
        {
            std::cout << "\n Input the number of dates: ";
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
