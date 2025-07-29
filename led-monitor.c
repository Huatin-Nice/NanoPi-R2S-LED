#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define IFACE_NAME "enxae3af448a08e"
#define OPERSTATE_PATH "/sys/class/net/" IFACE_NAME "/operstate"
#define LED_TRIGGER_PATH "/sys/class/leds/lan_led/trigger"
#define POLL_INTERVAL_SEC 3

bool get_link_status() {
    FILE *file = fopen(OPERSTATE_PATH, "r");
    if (file == NULL) {
        return false;
    }

    char state[16] = {0};
    if (fgets(state, sizeof(state), file) == NULL) {
        fclose(file);
        return false;
    }
    fclose(file);

    state[strcspn(state, "\n")] = 0;

    return strcmp(state, "up") == 0;
}

void set_led(const char *trigger) {
    FILE *file = fopen(LED_TRIGGER_PATH, "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "%s", trigger);
    fclose(file);
}

int main() {
    bool last_status = false;

    while (1) {
        bool current_status = get_link_status();

        if (current_status != last_status) {
            if (current_status) {
                set_led("default-on");
            } else {
                set_led("none");
            }
            last_status = current_status;
        }

        sleep(POLL_INTERVAL_SEC);
    }

    return 0;
}
