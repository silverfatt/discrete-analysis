#include <iostream>
#include <map>
#include <chrono>

int main()
{
    std::map<std::string, unsigned long long> data;
    std::string buf;
    unsigned long long value;
    unsigned amount;
    std::cin >> amount;
    // TEST 1 - ADDING ELEMENTS TO MAP
    std::chrono::steady_clock::time_point begin_of_adding = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf >> value;
        if (data.count(buf))
        {
            std::cout << "Exist\n";
            continue;
        }
        else
        {
            data[buf] = value;
            std::cout << "OK\n";
        }
    }
    std::chrono::steady_clock::time_point end_of_adding = std::chrono::steady_clock::now();
    unsigned addition_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_adding - begin_of_adding).count();

    // TEST 2 - FINDING ELEMENTS IN MAP
    std::chrono::steady_clock::time_point begin_of_finding = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf;
        if (data.count(buf))
        {
            std::cout << "OK: " << data[buf] << "\n";
        }
        else
        {
            std::cout << "NoSuchWord\n";
        }
    }
    std::chrono::steady_clock::time_point end_of_finding = std::chrono::steady_clock::now();
    unsigned finding_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_finding - begin_of_finding).count();

    // TEST 3 - REMOVING ELEMENTS FROM MAP
    std::chrono::steady_clock::time_point begin_of_deleting = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf;
        if (data.count(buf))
        {
            data.erase(buf);
            std::cout << "OK\n";
        }
        else
        {
            std::cout << "NOSuchWord\n";
        }
    }
    std::chrono::steady_clock::time_point end_of_deleting = std::chrono::steady_clock::now();
    unsigned deleting_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_deleting - begin_of_deleting).count();

    std::cout << "Pushed " << amount << " elements in " << addition_time << "ms\n";
    std::cout << "Found " << amount << " elements in " << finding_time << "ms\n";
    std::cout << "Deleted " << amount << " elements in " << deleting_time << "ms\n";
    return 0;
}