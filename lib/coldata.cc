#include <string>
#include "const_string3.hh"
#include "coldata.h"

template class mysql_ColData<std::string>;
template class mysql_ColData<const_string>;
