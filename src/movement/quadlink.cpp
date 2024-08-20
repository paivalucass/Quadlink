#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <thread>
#include <chrono>

using namespace mavsdk;
using namespace std;

int main() {
    // Creating asn instance of Mavsdk and configuring it to connect to the GroundStation component
    Mavsdk::Configuration config(mavsdk::Mavsdk::ComponentType::GroundStation);
    Mavsdk mavsdk(config);

    // Connecting via TCP 
    string connection_url = "udpin://127.0.0.1:14562"; // IP and port of the GroundStation
    ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);

    if (connection_result != ConnectionResult::Success) {
        cerr << "Falha na conexão" << endl;
        return 1;
    }

    // Waiting for the connection
    cout << "Esperando pelo sistema..." << endl;

    while (mavsdk.systems().size() == 0) {
        this_thread::sleep_for(chrono::seconds(1));
    }

    auto system = mavsdk.systems().at(0);

    if (!system->is_connected()) {
        cerr << "Nenhum drone conectado!" << endl;
        return 1;
    }

    cout << "Drone conectado!" << endl;

    // Creating instances of Action and Telemetry
    auto action = std::make_shared<Action>(system);
    auto telemetry = std::make_shared<Telemetry>(system);

    // Example: Gathering data from Telemetry
    // Lambda function
    telemetry->subscribe_battery([](Telemetry::Battery battery) {
        cout << "Bateria: " << battery.remaining_percent << "%" << endl;
    });

    // Time sleep but in C++
    this_thread::sleep_for(chrono::seconds(5));

    return 0;
}