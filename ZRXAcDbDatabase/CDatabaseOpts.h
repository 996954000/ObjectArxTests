#pragma once
#include "stdArx.h"

class CDatabaseOpts
{
public:
   static void CreateDatabase();
   static void CreateDWG();
   static void ReadDWG();
   static bool PartialOpenDatabase(AcDbDatabase* dataBase);
};

