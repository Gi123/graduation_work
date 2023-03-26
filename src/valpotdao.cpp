#include "valpotdao.hpp"

#include <iostream>
#include <ctime>

ValPotDAOClickHouse::ValPotDAOClickHouse(const std::string &tableName) {
    try {
        const auto localHostEndpoint = clickhouse::ClientOptions()
                    .SetHost("localhost")
                    .SetPort(9000)
                    .SetUser("default")
                    .SetPassword("")
                    .SetDefaultDatabase("default");
        std::cout << "Create localHostEndpoint" << std::endl;
        m_clickhouse = std::make_unique<clickhouse::Client>(localHostEndpoint);
        //m_clickhouse = new clickhouse::Client(localHostEndpoint);
        m_tableName = tableName;
    } catch (const std::exception& e) {
       std::cerr << "exception : " << e.what() << std::endl;
    }
}
void ValPotDAOClickHouse::ShowTables() {
    m_clickhouse->Select("SHOW TABLES", [](const clickhouse::Block& block) {
        for (size_t i = 0; i < block.GetRowCount(); ++i) {
            std::cout << (*block[0]->As<clickhouse::ColumnString>())[i] << "\n";
        }
    });
}
bool ValPotDAOClickHouse::create(const ValPot& valpot) {
    clickhouse::Block block;
    auto rid = std::make_shared<clickhouse::ColumnUInt64>();
    rid->Append(valpot.getRid());
    auto pid = std::make_shared<clickhouse::ColumnUInt64>();
    pid->Append(valpot.getPid());
    auto mtime = std::make_shared<clickhouse::ColumnDateTime>();
    mtime->Append(valpot.getMTime());
    auto comp = std::make_shared<clickhouse::ColumnString>();
    
    comp->Append(valpot.getComp());
    auto val = std::make_shared<clickhouse::ColumnFloat64>();
    val->Append(valpot.getVal());
    block.AppendColumn("rid", rid);
    block.AppendColumn("pid", pid);
    block.AppendColumn("mtime", mtime);
    block.AppendColumn("comp", comp);
    block.AppendColumn("val", val);

    m_clickhouse->Insert(m_tableName, block);
    return true;
}
bool ValPotDAOClickHouse::findVal(ValPot& valpot) {
    std::string query ("SELECT val from " + m_tableName + " WHERE rid = " + std::to_string(valpot.getRid()) + 
    " AND pid = " + std::to_string(valpot.getPid()) + " AND comp = '" + valpot.getComp() + "' ORDER BY mtime DESC");
    m_clickhouse->Select(query, [&valpot](const clickhouse::Block& block) {
        //std::cout << "Find elem =" << block.GetRowCount() << std::endl;
        if(block.GetRowCount()) {
            const auto val_col = block[0]->As<clickhouse::ColumnFloat64>();
            valpot.setVal((double) val_col->At(0));
            //std::cout << "Val =" << val_col->At(0) << std::endl;
        } 
    });

    // m_clickhouse->Select(query, [](const clickhouse::Block& block) {
            // for (size_t c = 0; c < block.GetRowCount(); ++c) {
            //     auto rid_col = block[0]->As<clickhouse::ColumnUInt64>();
            //     auto pid_col = block[1]->As<clickhouse::ColumnUInt64>();
            //     auto mtime_col = block[2]->As<clickhouse::ColumnDateTime64>();
            //     uint64_t t = mtime_col->As<clickhouse::ColumnDateTime64>()->At(c);
            //     std::time_t ct = t / 1000000;
            //     auto comp_col = block[3]->As<clickhouse::ColumnString>();
            //     auto val_col = block[4]->As<clickhouse::ColumnDecimal>();

            //     std::cout << "rid:" << (int)rid_col->At(c) << " pid: " << (int)pid_col->At(c) << " ctime: ";
            //     std::cout << std::asctime(std::localtime(&ct)) << " comp:" << (*block[4]->As<clickhouse::ColumnString>())[c];
            //     std::cout << " val: " << (double)val_col->At(c) <<std::endl;
            // }
    //     }
    // );

    return true;
}
bool ValPotDAOClickHouse::update(const ValPot& valpot) {
    std::string query ("ALTER TABLE " + m_tableName + " UPDATE  val = " + std::to_string(valpot.getVal()) +
        " WHERE rid = " + std::to_string(valpot.getRid()) + " AND pid = " + std::to_string(valpot.getPid()) +
        " AND comp = '" + valpot.getComp() + "' AND mtime =" + std::to_string(valpot.getMTime()));
    m_clickhouse->Execute(query);
    return true;
}
bool ValPotDAOClickHouse::remove(const ValPot& valpot) {
    std::string query ("ALTER TABLE " + m_tableName + " DELETE WHERE rid = " + std::to_string(valpot.getRid()) +
        " AND pid = " + std::to_string(valpot.getPid()) + " AND comp = '" + valpot.getComp() + "' AND mtime = " +
        std::to_string(valpot.getMTime()));
    m_clickhouse->Execute(query);
    return true;
}
std::vector<avgArray> ValPotDAOClickHouse::getValAvg(std::vector<std::string> &arrCompName) {
    std::vector<avgArray> result_array;
    std::string query_elems;
    for (int i=0; i< arrCompName.size(); ++i) {
        query_elems += "\'" + arrCompName.at(i) + "\'";
        if(i + 1 != arrCompName.size())
            query_elems += ", ";
    }
    std::cout << query_elems << std::endl;
    std::string query ("SELECT comp, toDate(mtime) AS date, AVG(val) FROM " + m_tableName + " WHERE date BETWEEN " +
    " today()-7 AND today() AND comp IN [" + query_elems +"]" + " GROUP BY comp, date" + " ORDER BY comp, date");
    std::cout << query << std::endl;
    m_clickhouse->Select(query, [&result_array](const clickhouse::Block& block) {
        std::cout << "2Find elem =" << block.GetRowCount() << std::endl;
        if(block.GetRowCount()) {
            for (size_t i = 0; i < block.GetRowCount(); ++i) {
                auto comp_col = block[0]->As<clickhouse::ColumnString>();
                auto date_col = block[1]->As<clickhouse::ColumnDate>();
                auto val_col = block[2]->As<clickhouse::ColumnFloat64>();
                std::cout << "comp:" << (*block[0]->As<clickhouse::ColumnString>())[i] << "\n";
                std::time_t t = date_col->As<clickhouse::ColumnDate>()->At(i);
                //std::cout <<" date:" << t << std::endl;
                result_array.emplace_back(avgArray{t, std::string(comp_col->At(0)),
                    (double) val_col->At(0)});
            }
        }

    });      
    return result_array;
}