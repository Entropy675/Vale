#ifndef SUPERTYPE_H__
#define SUPERTYPE_H__

#include "Entity.h"
#include "TagManager.h"
#include "defs.h"


class Supertype : public Entity
{

private:
    std::string tag;
    
public:
    Supertype();
    ~Supertype();

};

#endif
