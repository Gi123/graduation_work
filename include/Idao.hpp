#include "valpot.hpp"
#include <vector>

class ValPotDAOInterface{
    virtual bool create(const ValPot& valpot) = 0;
    virtual bool findVal(ValPot& valpot) = 0;
    virtual bool update(const ValPot& valpot) = 0;
    virtual bool remove(const ValPot& valpot) = 0;
};


