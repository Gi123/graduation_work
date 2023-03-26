#include "valpot.hpp"

ValPot::ValPot(const int _rid, const int _pid, const std::time_t _mtime, const std::string & _comp,
    const double _val) : rid(_rid), pid(_pid), mtime(_mtime), comp(_comp), val(_val) {}
int ValPot::getRid() const { return rid; } 
int ValPot::getPid() const{ return pid;} 
std::time_t ValPot::getMTime() const{ return mtime;} 
std::string ValPot::getComp() const { return comp;}
double ValPot::getVal() const {return val;}
void ValPot::setRid(const int new_rid){ rid = new_rid;}
void ValPot::setPid(const int new_pid){pid = new_pid;}
void ValPot::setMTime(const std::time_t new_mtime){mtime = new_mtime;}
void ValPot::setComp(const std::string& new_comp){comp = new_comp;}
void ValPot::setVal(const double new_val){val = new_val;}
 