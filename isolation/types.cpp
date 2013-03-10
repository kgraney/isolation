//
//  types.cpp
//  isolation
//
//  Created by Kevin Graney on 3/9/13.
//  Copyright (c) 2013 Kevin Graney. All rights reserved.
//

#include <iostream>
#include "types.h"

#define X(name, string) string,
std::string kPlayerName[] = {
    kPlayerXMacro
};
#undef X

std::ostream& operator<< (std::ostream& stream, const Player& player)
{
    stream << kPlayerName[player];
    return stream;
}
