All:
	gcc keys.c -o my_keys `pkg-config --cflags --libs gtk+-3.0 libwnck-3.0`
