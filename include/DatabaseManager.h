#pragma once

#include <Arduino.h>
#include <Secrets.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

class DatabaseManager
{
private:
    FirebaseConfig _config;
    FirebaseAuth _auth;
    FirebaseData _fbdo;
    FirebaseJson _json;
    char _databasePath[128];
    char _devicePath[160];
    bool _isAuthenticated = false;

    static void s_databaseTask(void *arg)
    {
        DatabaseManager *_manager = static_cast<DatabaseManager *>(arg);

        _manager->setupFirebase();
        
        while(true)
        {
            if(_manager -> _isAuthenticated)
            {
                _manager -> pingRTDB();
            }

            vTaskDelay(pdMS_TO_TICKS(10000));
        }

    }

    void setupFirebase()
    {
        _config.api_key = API_KEY;
        _config.database_url = FIREBASE_HOST;

        _auth.user.email = DEMO_FIREBASE_EMAIL;
        _auth.user.password = DEMO_FIREBASE_PASSWORD;

        _config.token_status_callback = tokenStatusCallback;
        Firebase.begin(&_config, &_auth);
        Firebase.reconnectWiFi(true);

        Serial.println("Authenticating"); // debug
        uint8_t retries = 0;
        while (_auth.token.uid == "" && retries < 40)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
            retries++;
            Serial.print(".");
        }
        Serial.print("Authenticated! Logged into UID: "); // debug
        Serial.print(_auth.token.uid.c_str());             // debug

        if (_auth.token.uid != "")
        {
            Serial.println("\n Authenticated successfully!"); // debug
            Serial.print(" Device UID: "); // debug
            Serial.println(_auth.token.uid.c_str()); // debug

            snprintf(_devicePath, sizeof(_devicePath), "devices/%s", _auth.token.uid.c_str());
            _isAuthenticated = true;
        }
        snprintf(_databasePath, sizeof(_databasePath), DATABASE_ROOT_PATH, _auth.token.uid.c_str());
    }

public:
    void pingRTDB()
    {
        if (Firebase.RTDB.setBool(&_fbdo, "users/testuser/devices/deviceID/online", true))
        {
            Serial.println("Succes"); // debug
        }
        else
        {
            Serial.println(_fbdo.errorReason()); // debug
        }
    }

    void uploadHeartbeat()
    {
        
    }

    void uploadSensorData(int sensorValue)
    {
        // Firebase.RTDB.setInt(&fbdo, databasePath, sensorValue);
    }

    void beginTask()
    {
        xTaskCreate(
            s_databaseTask,
            "Database Task",
            8192,
            this,
            1,
            NULL);
    }
};
