#include <Process.h>

#define LED_1               11
#define LED_2               12
#define LED_3               13
#define LED_ON              1
#define LED_OFF             0
#define LED_PIN_IDX         0
#define LED_NEW_STATE_IDX   1
#define LED_OLD_STATE_IDX   2
#define MAX_PATH            16

static const char *led_path    = "/tmp";
static const char *door_sh_path   = "/demo.sh";

int ledPins[][3] =
{
    /* Pin  , New stat, Old state*/
    { LED_1 , LED_OFF , LED_OFF },
    { LED_2 , LED_OFF , LED_OFF },
    { LED_3 , LED_OFF , LED_OFF }
};

static unsigned int led_tbl_sz = (sizeof(ledPins) / sizeof(ledPins[0]));

void setup() {
    // initialize the digital pin as an output.
    for (int led = 0; led < led_tbl_sz; led++) {
        pinMode(ledPins[led][LED_PIN_IDX], OUTPUT);
    }
  
    // Initialize the Bridge
    Bridge.begin();
    
    // Initialize the Serial
    Serial.begin(9600);

    // Wait until a Serial Monitor is connected.
    while (!Serial);

    randomSeed(analogRead(0));
}

void led_update(int idx, int ret)
{
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

void runLedHandler()
{
    Process proc;
    char fp[MAX_PATH]={0};
    
    for (int idx = 0; idx < led_tbl_sz; idx++) {
        // set path
        sprintf(fp, "%s/%d", led_path, ledPins[idx][LED_PIN_IDX]);
        
        // run cat command
        proc.begin("cat");
        proc.addParameter(fp); 
        proc.run();
        
        // do nothing until the process finishes
        while (proc.running());
        
        // Success if exit value is zero
        if (proc.exitValue() == 0) {
            // LED Control
            led_update(idx, proc.parseInt());
        } else {
            // Serial.println("Command Failed without return value");
        }

        // Serial.flush();
        proc.flush();
        memset(fp, 0, MAX_PATH);
    }
}

void runDoorHandler()
{
    /* dummy data */
    int door_inst = random(0, 5);
    /* dummy data */

    // set path
    char cmd[MAX_PATH]={0};
    sprintf(cmd, "%s %d", door_sh_path, door_inst);

    // run cat command
    Process proc;
    proc.runShellCommand(cmd);

    // do nothing until the process finishes
    while (proc.running());
    
    // Serial.flush();
    proc.flush();
}

void loop() {
    runLedHandler();
    delay(500);
    runDoorHandler();
    delay(500);
}
