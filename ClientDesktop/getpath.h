#pragma once

#include <QString>

#include "enums.h"

class Get_My_Path
{
public:
    Get_My_Path();
    QString GetPath(Directorys target, Files directory);
};

