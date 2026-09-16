#include "DatabaseManager.h"
#include "Secrets.h"
#include <Preferences.h>


void DatabaseManager::s_databaseTask(void *arg)
    {
        DatabaseManager *_manager = static_cast<DatabaseManager *>(arg);

        _manager->setupFirebase();
        
        while(true)
        {
            if(_manager -> _isAuthenticated && Firebase.ready())
            {
                if (millis() - _manager -> _lastUploadTimeMS >= UPLOAD_INTERVAL_MS)
                {
                    _manager -> pingRTDB();
                    _manager -> uploadSensorData(DUMMY_SENSOR_VALUE);
                    _manager -> _lastUploadTimeMS = millis();
                } 
                
                _manager -> listenForDatabaseCommands();
            }

            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }

void DatabaseManager::setupFirebase()
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
        Serial.print(_auth.token.uid.c_str());            // debug

        if (_auth.token.uid != "" && Firebase.ready())
        {

            Serial.println("\n Authenticated successfully!"); // debug
            Serial.print(" Device UID: ");                    // debug
            Serial.println(_auth.token.uid.c_str());          // debug

            _isAuthenticated = true;

            snprintf(_databaseUserPath, sizeof(_databaseUserPath), "users/%s", _auth.token.uid.c_str()); 
            snprintf(_databaseDevicePath, sizeof(_databaseDevicePath), "%s/%s", _databaseUserPath, g_macAddress);
            snprintf(_databasePingPath, sizeof(_databasePingPath), "%s/%s", _databaseDevicePath, "online");
            snprintf(_databaseSoilMoisturePath, sizeof(_databaseSoilMoisturePath), "%s/%s", _databaseDevicePath, "soilMoisture");
            snprintf(_databaseCommandsPath, sizeof(_databaseCommandsPath),"%s/%s", _databaseDevicePath, "commands");
        
            Firebase.RTDB.beginStream(&_streamFbdo, _databaseCommandsPath);
        }
        
    }

    void DatabaseManager::listenForDatabaseCommands()
    {
        if(!Firebase.RTDB.readStream(&_streamFbdo))
        {
            return;
        }

        if(_streamFbdo.streamTimeout())
        {
            return;
        }

        if(_streamFbdo.streamAvailable())
        {
            if(_streamFbdo.dataType() == "json")
            {
                FirebaseJson &json = _streamFbdo.jsonObject();
                FirebaseJsonData jsonResult;

                json.get(jsonResult, "reset");

                if(jsonResult.success && jsonResult.boolValue == true)
                {
                    executeFactoryReset();
                }
            }
            else if (_streamFbdo.dataType() == "boolean")
            {
                if (_streamFbdo.dataPath() == "/reset" && _streamFbdo.boolData() == true)
                {
                    executeFactoryReset();
                }
            }
        }
    }

    void DatabaseManager::executeFactoryReset()
    {
        Preferences prefs;
        prefs.begin("wifi", false);
        prefs.clear();
        prefs.end();

        delay(500);

        ESP.restart();
    }

    void DatabaseManager::pingRTDB()
    {
        if (Firebase.RTDB.setBool(&_fbdo, _databasePingPath, true))
        {
            Serial.println("Succes"); // debug
        }
        else
        {
            Serial.println(_fbdo.errorReason()); // debug
        }
    }


    void DatabaseManager::uploadSensorData(int sensorValue)
    {
        Firebase.RTDB.setInt(&_fbdo, _databaseSoilMoisturePath, sensorValue);
    }

    void DatabaseManager::beginTask()
    {
        if(_databaseTaskHandle != nullptr)
            return;

        xTaskCreate(
            s_databaseTask,
            "Database Task",
            8192,
            this,
            1,
            &_databaseTaskHandle);
    }