#include <fib.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>

CAMLprim value caml_fib(value n) {
  CAMLparam1(n);
  CAMLreturn(fib(Int_val(n)));
}
