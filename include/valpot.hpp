#pragma once

#include <string>
#include <ctime>  

class ValPot
{
public:
    ValPot(const int _rid, const int _pid, const std::time_t _mtime, const std::string &comp,
        const double _val);
    int getRid() const;
    int getPid() const;
    std::time_t getMTime() const;
    std::string getComp() const;
    double getVal() const;
    void setRid(const int new_rid);
    void setPid(const int new_pid); 
    void setMTime(const std::time_t new_mtime);
    void setComp(const std::string& new_comp);
    void setVal(const double new_val); 
private:
    int rid;
    int pid;
    std::time_t mtime;
    std::string comp;
    double val;
};
