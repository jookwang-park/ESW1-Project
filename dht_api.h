
struct dht_data {
	int humidity;
	int temperature;
};

void DHT_on(int farm_fd);
void DHT_off(int farm_fd);
void DHT_set_input(int farm_fd);
void DHT_set_output(int farm_fd);
int DHT_read_data(int farm_fd);
struct dht_data DHT_get_data(int farm_fd);
