#define MYSQLPP_NOT_HEADER
#include "platform.h"

#include "field_types.h"

#include "result2.hh"

void FieldTypes::init(const ResUse *res) {
  int num = res->num_fields();
  reserve(num);
  for (int i = 0; i < num; i++) {
    push_back(res->fields(i));
  }
}

