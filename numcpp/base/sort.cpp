#include "sort.h"
#include "../core.h"
#include "initializers.h"

namespace numcpp
{

DynTypeArray& sort_(DynTypeArray& x)
{
  CALL_TEMPLATE_FUNC_INT_TYPES(return sort_,x)
  CALL_TEMPLATE_FUNC_FLOAT_TYPES(return sort_,x)
  throw std::invalid_argument("unsupported type");
}

Array<size_t> argsort(const DynTypeArray& x)
{
  CALL_TEMPLATE_FUNC_INT_TYPES(return argsort,x)
  CALL_TEMPLATE_FUNC_FLOAT_TYPES(return argsort,x)
  throw std::invalid_argument("unsupported type");
}


}
