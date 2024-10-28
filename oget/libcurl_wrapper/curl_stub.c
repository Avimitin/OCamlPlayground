/*
 * This stub file is a simple wrapper of libcurl library for OCaml side to call
 * the libcurl "easy" APIs. This is just a learning example for OCaml and C
 * FFI. The stub file provides only API similar to Rust `reqwest::get()`
 * function.
 *
 * During investigation, I have a couple concerns about creating
 * low-level, detailed FFI wrapper:
 *
 *  1. While a low-level API is more flexible for composing functions and
 *  calling APIs in OCaml. But composing FFI functions is a bunch of foreign
 *  function calls and type conversion, which will definitely have huge impact
 *  on performance.
 *
 *  2. It's already a hassle to read manpage and figure out the function type
 *  and return type for the C library. I want my OCaml to be highly abstracted
 *  and to only "invoke actions" without knowing how those "actions" are
 *  implemented.
 *
 * This makes me wonder: am I writing OCaml, or am I writing C? If OCaml's main
 * task is function calling, it seems I'll have to deal with C for a while ,
 * which means I won't be able to abstract anythings if I have to implement
 * this myself.
 */

#include <curl/curl.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <stdlib.h>
#include <string.h>

struct memory {
  char *response;
  size_t size;
};

static size_t cb(char *data, size_t size, size_t nmemb, void *clientp) {
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;

  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory! */

  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;

  return realsize;
}

CAMLprim value caml_curl_get(value url) {
  CAMLparam1(url);
  const char* raw_url = String_val(url);

  CURL *curl;
  CURLcode res;

  struct memory chunk = { 0 };

  // Initialize libcurl
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  value caml_response;
  if(curl) {
    // Set the URL for the GET request
    curl_easy_setopt(curl, CURLOPT_URL, raw_url);

    // Optionally follow redirections
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Use our custom callback to save response body
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if(res != CURLE_OK) {
      char* fail_reason;
      asprintf(&fail_reason, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
      // FIXME: do we need C return here?
      caml_failwith(fail_reason);
    }

    caml_response = caml_copy_string(chunk.response);
    free(chunk.response);

    // Cleanup
    curl_easy_cleanup(curl);
  } else {
    caml_failwith("curl_easy_init(): fail initialize curl instance");
  }

  // Global cleanup
  curl_global_cleanup();

  CAMLreturn(caml_response);
}
