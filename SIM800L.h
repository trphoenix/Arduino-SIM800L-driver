/********************************************************************************
 * Arduino-SIM800L-driver                                                       *
 * ----------------------                                                       *
 * Arduino driver for GSM module SIM800L                                        *
 * Author: Olivier Staquet                                                      *
 * Last version available on https://github.com/ostaquet/Arduino-SIM800L-driver *
 *******************************************************************************/
#ifndef _SIM800L_H_
#define _SIM800L_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

#define DEFAULT_TIMEOUT 5000

enum PowerMode {MINIMUM, NORMAL, POW_UNKNOWN, SLEEP, POW_ERROR};
enum NetworkRegistration {NOT_REGISTERED, REGISTERED_HOME, SEARCHING, DENIED, NET_UNKNOWN, REGISTERED_ROAMING, NET_ERROR};

class SIM800L {
  public:
    // Initialize the driver
    SIM800L(uint8_t _pinTx, uint8_t _pinRx, uint8_t _pinRst, uint16_t _internalBufferSize = 128, uint16_t _recvBufferSize = 256, bool _enableDebug = false);
    ~SIM800L();

    // Force a reset of the module
    void reset();

    // Status functions
    bool isReady();
    uint8_t getSignal();
    PowerMode getPowerMode();
    NetworkRegistration getRegistrationStatus();

    // Define the power mode
    bool setPowerMode(PowerMode powerMode);

    // Enable/disable GPRS
    bool setupGPRS(const char *apn);
    bool connectGPRS();
    bool disconnectGPRS();

    // HTTP methods
    uint16_t doGet(const char* url, uint16_t serverReadTimeoutMs);
    uint16_t doPost(const char* url, const char* contentType, const char* payload, uint16_t clientWriteTimeoutMs, uint16_t serverReadTimeoutMs);

    // Obtain results after HTTP successful connections (size and buffer)
    uint8_t getDataSizeReceived();
    char* getDataReceived();

  protected:
    // Send command
    void sendCommand(const char* command);
    // Send comment from PROGMEM
    void sendCommand_P(const char* command);
    // Send command with parameter within quotes (template : command"parameter")
    void sendCommand(const char* command, const char* parameter);
    // Send command with parameter within quotes from PROGMEM (template : command"parameter")
    void sendCommand_P(const char* command, const char* parameter);
    // Read from module (timeout in millisec)
    bool readResponse(uint16_t timeout, uint8_t crlfToWait = 2);
    // Read from module and expect a specific answer defined in PROGMEM (timeout in millisec)
    bool readResponseCheckAnswer_P(uint16_t timeout, const char* expectedAnswer, uint8_t crlfToWait = 2);
    // Read from module but forget what is received
    void readToForget(uint16_t timeout);
    
    // Find string in another string
    int16_t strIndex(const char* str, const char* findStr, uint16_t startIdx = 0);

    // Manage internal buffer
    void initInternalBuffer();

    // Initiate HTTP/S connection
    uint16_t initiateHTTP(const char* url);
    uint16_t terminateHTTP();

  private:
    // Serial line with SIM800L
    SoftwareSerial* serial;

    // Details about the circuit: pins
    uint8_t pinReset = 0;

    // Internal memory for the shared buffer
    // Used for all reception of message from the module
    char *internalBuffer;
    uint16_t internalBufferSize = 0;

    // Reception buffer
    char *recvBuffer;
    uint16_t recvBufferSize = 0;
    uint16_t dataSize = 0;

    // Enable debug mode
    bool enableDebug = false;
};

#endif // _SIM800L_H_
