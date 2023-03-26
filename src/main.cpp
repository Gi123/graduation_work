#include "valpotdao.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

int main(int argc, char const *argv[]) {

    try {
        ValPotDAOClickHouse client("valpot");
        client.ShowTables();
        auto const now = std::chrono::system_clock::now();
        std::time_t newt = std::chrono::system_clock::to_time_t(now);
        ValPot new_elem(1,1, newt, "Amperage", 4.4);
        client.create(new_elem);
        ValPot find_elem(1, 1, newt, "Amperage", 1.1);
        client.findVal(find_elem);
        find_elem.setVal(1.2);
        client.update(find_elem);
        client.remove(new_elem);
        std::vector<std::string> name_coms;
        name_coms.emplace_back("Amperage");
        name_coms.emplace_back("Voltage");
        std::vector<avgArray> array = client.getValAvg(name_coms);
    } catch (const std::exception& e) {
       std::cerr << "exception : " << e.what() << std::endl;
    }

    return 0;
}
