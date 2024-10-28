external curl_get: string -> string = "caml_curl_get"

let () = curl_get "https://api.github.com"
  |> print_endline
