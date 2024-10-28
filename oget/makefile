BUILD := build
CC := gcc
OCAMLC := ocamlc

LIBCURL_INC_DIR := /usr/include
OCAML_INC_DIR := $(shell $(OCAMLC) -where)

$(BUILD)/curl_stub.o: ./libcurl_wrapper/curl_stub.c
	mkdir -p $(@D)
	$(CC) -I$(LIBCURL_INC_DIR) -I$(OCAML_INC_DIR) -c $< -o $@

$(BUILD)/curl: ./bin/curl.ml $(BUILD)/curl_stub.o
	mkdir -p $(@D)
	$(OCAMLC) -custom -o $@ $? \
		-cclib -I$(LIBCURL_INC_DIR) \
		-cclib -L/usr/lib \
		-cclib -lcurl
