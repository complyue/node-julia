#include <iostream>
#include <julia.h>
#include "Immediate.h"
#include "error.h"
#include "lvalue.h"

using namespace std;

nj::Result nj::Immediate::eval(vector<shared_ptr<nj::Value>> &args)
{
   vector<shared_ptr<nj::Value>> res;

   if(args.size() != 1 || !args[0]->isPrimitive()) return res;

   Primitive &text = static_cast<Primitive&>(*args[0]);
   jl_value_t *jl_res = (jl_value_t*)jl_eval_string((char*)text.toString().c_str());
   jl_value_t *jl_ex = jl_exception_occurred();
  
   if(jl_ex)
   {
     JL_GC_PUSH1(&jl_ex);
     shared_ptr<Exception> ex = genJuliaError(jl_ex);

     JL_GC_POP();
     return Result(ex);
   }
   else
   {
      JL_GC_PUSH1(&jl_res);
      res = lvalue(jl_res);
      JL_GC_POP();
      return Result(res);
   }
}
