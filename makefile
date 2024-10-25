BUILD := build
CC := gcc
OCAMLC := ocamlc

.PHONY: clean
clean:
	rm $(BUILD)/*

$(BUILD)/fib.o: ./fib/src/fib.c ./fib/include/fib.h
	@mkdir -p $(@D)
	$(CC) -c -fPIC -I./fib/include $< -o $@

$(BUILD)/libfib.so: $(BUILD)/fib.o
	$(CC) -shared $< -o $@

$(BUILD)/caml_fib_stub.o: ./fib-wrapper/fib_stub.c ./fib/include/fib.h
	$(CC) -c -I$(shell $(OCAMLC) -where) -I./fib/include $< -o $@

$(BUILD)/fib.cmo: ./lib/fib.ml
	$(OCAMLC) -c $< -o $@

$(BUILD)/main: ./bin/main.ml $(BUILD)/libfib.so $(BUILD)/caml_fib_stub.o $(BUILD)/fib.cmo
	$(OCAMLC) -I $(BUILD) -custom -o $@ \
		$(BUILD)/caml_fib_stub.o $(BUILD)/fib.cmo ./bin/main.ml \
		-cclib -L$(BUILD) -cclib -lfib -cclib -Wl,-rpath,$(realpath $(BUILD))
