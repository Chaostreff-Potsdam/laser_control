ETHERDREAM_DIR = ../src/etherdream
all: etherdream.so

etherdream.so: $(ETHERDREAM_DIR)/driver-unix/etherdream.c
	$(CC) $(CFLAGS) -DNO_ETHERDREAM_TRACE=1 -I$(ETHERDREAM_DIR)/common -shared -Wl,-install_name,$@ -o $@ -fPIC $^

clean:
	$(RM) etherdream.so
