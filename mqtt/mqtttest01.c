#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <mosquitto.h>
#include <gpiod.h>

// #define mqtt_host "192.168.1.137" 
#define mqtt_host "192.168.2.100"
#define mqtt_port 1883

#define CONSUMER "internLED.c"

static int run = 1;
// gpio define
int chipnumber = 1;
unsigned int line_num = 19;
struct gpiod_chip *chip;
struct gpiod_line *line;
int ret;

void trim_nr(char *str) {
    if (str == NULL || *str == '\0') return;

    // Trim trailing \n and \r
    char *end = str + strlen(str) - 1;
    while (end >= str && (*end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    // Trim leading \n and \r
    char *start = str;
    while (*start == '\n' || *start == '\r') {
        start++;
    }

    // Shift the trimmed string to the beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

void handle_signal(int s)
{
	run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	chip = gpiod_chip_open_by_number(chipnumber);
	line = gpiod_chip_get_line(chip, line_num);
	ret = gpiod_line_request_output(line , CONSUMER, 0);

	bool match = 0;

	mosquitto_topic_matches_sub("home/BBB/LED_Switch/set", message->topic, &match);
	if (match) {
	//printf("got message for BBB switch set %s\n", message->payload);
		mosquitto_publish(mosq, NULL, "home/BBB/LED_Switch/available", 6, "online", 0, false);
		char* temp_payload = message->payload;
		//trim_nr(temp_payload);
		if( strcmp(temp_payload, "1") == 0 || strcmp(temp_payload, "on") == 0) {
		    ret = gpiod_line_set_value(line , 1);
		    mosquitto_publish(mosq, NULL, "home/BBB/LED_Switch", 3, "ON", 0, false);
		} else {
		    ret = gpiod_line_set_value(line , 0);
		    mosquitto_publish(mosq, NULL, "home/BBB/LED_Switch", 3, "OFF", 0, false);
		}
	printf("got message '%d , %s %d , %s' for topic '%s'\n", message->payloadlen, (char*) message->payload, strlen(temp_payload),  temp_payload, message->topic);
	}
}

int main(int argc, char *argv[])
{

	uint8_t reconnect = true;
	char *clientid = "home/BBB";
	struct mosquitto *mosq;
	int rc = 0;
	int count = 0;

	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);

	mosquitto_lib_init();
	mosq = mosquitto_new(clientid, true, 0);

	if(mosq){
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, message_callback);
//		mosquitto_username_pw_set(mosq, "mqttuser", "1234");

	    rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

		mosquitto_subscribe(mosq, NULL, "home/BBB/LED_Switch/set", 1);
		mosquitto_publish(mosq, NULL, "home/BBB/LED_Switch/available", 6, "online", 0, false);
		mosquitto_publish(mosq, NULL, "home/BBB/LED_Switch", 3, "OFF", 0, false);

		while(run){
			rc = mosquitto_loop(mosq, -1, 1);
			if(run && rc != 0){
				printf("connection error!\n");
				sleep(10);
				rc = mosquitto_reconnect(mosq);
				if( rc != 0 )
				{
					printf("reconnect error!\n");
					count += 1;
				}
			}
			else {
				count = 0;
			}
			if( count > 10 ) {
				mosquitto_lib_cleanup();
				return rc;
			}
		}
		mosquitto_destroy(mosq);
	}
	mosquitto_lib_cleanup();
	return rc;
}
