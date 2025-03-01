#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "ACS712.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace acs712_external {

class ACS712Sensor : public sensor::Sensor, public PollingComponent {
   public:
    ACS712Sensor() : PollingComponent(15000) {}

    void setup() override;
    void update() override;

    // ✅ Declare setter functions correctly
    void set_current_sensor(sensor::Sensor *sensor) { this->current_sensor = sensor; }
    void set_power_sensor(sensor::Sensor *sensor) { this->power_sensor = sensor; }
    void set_pin(GPIOPin *pin) { pin_ = pin; }
    void set_csmpin(int pin) {
        // if (esphome::validate_pin()) {
            csmpin_ = pin;
        // } else {
        //    ESP_LOGE("acs712", "Invalid pin");
        //}
    }

   private:
    sensor::Sensor *current_sensor = nullptr;  // ✅ Initialize to nullptr
    sensor::Sensor *power_sensor = nullptr;
    ACS712 *ACS = nullptr;


   protected:
    GPIOPin *pin_;
    int csmpin_;


};

}  // namespace acs712_external
}  // namespace esphome
