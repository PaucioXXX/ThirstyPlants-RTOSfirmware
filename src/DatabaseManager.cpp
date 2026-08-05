#include <DatabaseManager.h>
#include <Secrets.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

FirebaseConfig config;
FirebaseAuth auth;
FirebaseData fbdo;

void setupFirebase()
{
    config.api_key = API_KEY;
    config.database_url = FIREBASE_HOST;

    auth.user.email = "p@p.ro";
    auth.user.password = "qwerty";

    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("Authenticating");
    while (auth.token.uid == "") {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Authenticated! Logged into UID: ");
    Serial.print(auth.token.uid.c_str());
}

void uploadData(int sensorValue)
{
    char databasePath[128];

    snprintf(databasePath, sizeof(databasePath), DATABASE_ROOT_PATH, auth.token.uid.c_str());

    Firebase.RTDB.setInt(&fbdo, databasePath, sensorValue);
}