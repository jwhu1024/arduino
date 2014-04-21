#include <FileIO.h>

#define LED_11              11
#define LED_12              12
#define LED_13              13
#define LED_ON              1
#define LED_OFF             0
#define LED_PIN_IDX         0
#define LED_NEW_STATE_IDX   1
#define LED_OLD_STATE_IDX   2
#define LIMIT_SWITCH_PIN    3  // digital
#define MAX_PATH            16

static const char *led_path    = "/tmp";
static const char *door_path   = "/tmp/door";

short ledPins[][3] =
{
    /* Pin   , New stat, Old state*/
    { LED_11 , LED_OFF , LED_OFF },
    { LED_12 , LED_OFF , LED_OFF },
    { LED_13 , LED_OFF , LED_OFF }
};

static short led_tbl_sz = (sizeof(ledPins) / sizeof(ledPins[0]));

void setup() {
    // initialize the digital pin as an output.
    for (short led = 0; led < led_tbl_sz; led++) {
        pinMode(ledPins[led][LED_PIN_IDX], OUTPUT);
    }

    // set pin as input
    pinMode(LIMIT_SWITCH_PIN , INPUT);
  
    // Initialize the Bridge
    Bridge.begin();
    
    // Initialize the Serial
    Serial.begin(9600);

    // Setup File IO
    FileSystem.begin();

    // Wait until a Serial Monitor is connected.
    // while (!Serial);

    randomSeed(analogRead(0));
}

void led_update(short idx, int ret) {
    ledPins[idx][LED_NEW_STATE_IDX] = (ret == 1) ? LED_ON : LED_OFF;
    if (ledPins[idx][LED_NEW_STATE_IDX] == ledPins[idx][LED_OLD_STATE_IDX]) {
        // Serial.println("Status NOT change, do nothing");
        return;
    }
    
    // write
    digitalWrite(ledPins[idx][LED_PIN_IDX], ledPins[idx][LED_NEW_STATE_IDX]);

    // save status
    ledPins[idx][LED_OLD_STATE_IDX] = ledPins[idx][LED_NEW_STATE_IDX];
    return;
}

void read_led_data(char *fp, String &data) {
    if (true == FileSystem.exists(fp)) {
        File leds = FileSystem.open(fp, FILE_READ);
        while (leds.available() > 0) {
            char c = leds.read();
            data += c;
        }
        leds.close();
    }
    return;
}

void runLedHandler() {
    for (short idx = 0; idx < led_tbl_sz; idx++) {
        String buf = "";
        char fp[MAX_PATH]={0};

        // set path
        sprintf(fp, "%s/%d", led_path, ledPins[idx][LED_PIN_IDX]);

        // read data
        read_led_data(fp, buf);

        // control led when data is available
        if (buf.length() != 0) {
            led_update(idx, buf.toInt());
        }
    }
}

void runDoorHandler() {
    /* 1 : open , 0 : close */
    short limit_val = digitalRead(LIMIT_SWITCH_PIN);
    File door = FileSystem.open(door_path, FILE_WRITE);
    if (door) {
        door.println(limit_val);
    }
    door.close();
}

void loop() {
    runLedHandler();
    delay(500);
    runDoorHandler();
    delay(500);
}
