// Drone.h
#pragma once

#include <string>
#include <memory>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

namespace quadlink {

    class QuadCopter {
    public:
        // Construtor que inicializa o drone com uma configuração específica
        QuadCopter(const mavsdk::Mavsdk::Configuration& config);

        // Método para conectar o drone via URL (pode ser UDP, TCP, etc.)
        bool connect(const std::string& connection_url);

        // Método para iniciar o voo
        void takeoff(double height);

        // Método para pousar o drone
        void land();

        // Método para obter o nível de bateria atual
        void get_battery_status();

    private:
        // Instâncias do MAVSDK, Action e Telemetry
        mavsdk::Mavsdk mavsdk;
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };

}

