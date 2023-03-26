#include "Idao.hpp"

#include <memory.h>
#include <vector>

#include <clickhouse/client.h>
#include <clickhouse/error_codes.h>
#include <clickhouse/types/type_parser.h>
#include <clickhouse/base/socket.h>

struct avgArray
{
    uint64_t date;
    std::string comp;
    double val;
};

class ValPotDAOClickHouse : public ValPotDAOInterface {
private:
    std::string m_tableName;
    std::unique_ptr<clickhouse::Client> m_clickhouse;
    //clickhouse::Client *m_clickhouse;
public:
    ValPotDAOClickHouse(const std::string &tableName);
    bool create(const ValPot& valpot) override;
    bool findVal(ValPot& valpot) override;
    bool update(const ValPot& valpot) override;
    bool remove(const ValPot& valpot) override;
    void ShowTables();
    std::vector<avgArray> getValAvg(std::vector<std::string> &arrCompName);
};


