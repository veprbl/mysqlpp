#include "coldata.h"
#include "const_string.h"

#include <string>

template class mysql_ColData<std::string>;
template class mysql_ColData<const_string>;
