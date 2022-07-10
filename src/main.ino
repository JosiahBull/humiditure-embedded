/******* GENERAL CONFIG *******/
// How often to send data to the webserver in seconds
const int sendInterval = 60; // 300 seconds = 5 minutes

const char* ssid = "__SSID__HERE";
const char* password = "__PASSWORD_HERE";
const char* serverAddress = "__SERVER_ADDRESS_HERE";
const int sensorId = 0;

/******* VISION SETUP *******/
#include <eloquent.h>
#include <eloquent/vision/motion/naive.h>
#include <eloquent/vision/camera/esp32/aithinker/gray/qqvga.h>
#define THUMB_WIDTH 32
#define THUMB_HEIGHT 24
Eloquent::Vision::Motion::Naive<THUMB_WIDTH, THUMB_HEIGHT> detector;
bool motionDetected = false;
bool messageSent = false;

/******* WIFI SETUP *******/
#include <WiFi.h>
#include <HTTPClient.h>

volatile int interruptCounter = 0;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t *timer = NULL;
unsigned long previousMillis = 0;

/******* INTERRUPT HANDLERS *******/

void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

/******* FUNCTIONS *******/

void post_to_webserver() {
    /***** WiFi Setup *****/
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected at ip address: ");
    Serial.println(WiFi.localIP());

    HTTPClient http;
    http.begin(serverAddress);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "application/json");
    http.addHeader("X-Sensor-Id", String(sensorId));
    http.addHeader("X-Sensor-Type", "motion");
    int httpCode = http.POST("{ \"motionDetected\":\"" + String(motionDetected) + "\" }");
    http.end();

    WiFi.disconnect();
}

/******* SETUP *******/

void setup() {
    delay(4000);
    Serial.begin(115200);

    /***** Camera Setup *****/
    // turn on high freq for fast streaming speed
    camera.setHighFreq();

    if (!camera.begin())
        eloquent::abort(Serial, "Camera init error");

    Serial.println("Camera init OK");

    detector.throttle(10);

    // trigger motion when at least 10% of pixels change intensity by
    // at least 15 out of 255
    detector.setPixelChangesThreshold(0.2);
    detector.setIntensityChangeThreshold(25);

    /***** Timer Setup *****/
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true); // triggers once per second
    timerAlarmEnable(timer);

    /***** LED Pinout Setup *****/
    pinMode(16, OUTPUT);
}

void loop() {
    // Check if we need to update the server with our current status
    if (interruptCounter > sendInterval) {
        Serial.println("Sending data to server");
        portENTER_CRITICAL_ISR(&timerMux);
        interruptCounter = 0;
        portEXIT_CRITICAL_ISR(&timerMux);

        post_to_webserver();

        motionDetected = false;
        messageSent = true;
    }

    // Check for motion on the camera
    if (!motionDetected) {
        if (!camera.capture()) {
            Serial.println(camera.getErrorMessage());
            return;
        }

        Serial.println("Processing camera image...");

        // perform motion detection on resized image for fast detection
        camera.image.resize<THUMB_WIDTH, THUMB_HEIGHT>();
        // camera.image.printAsJsonTo(Serial);
        detector.update(camera.image);

        if (detector.isMotionDetected() && !messageSent) {
            Serial.println("Motion detected");
            digitalWrite(16, HIGH);
            motionDetected = true;
        } else {
            digitalWrite(16, LOW);
        }

        // release memory
        camera.free();

        messageSent = false;
    }
}
