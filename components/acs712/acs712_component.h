#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "ACS712.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace acs712 {

class ACS712Sensor : public sensor::Sensor, public PollingComponent {
   public:
    ACS712Sensor() : PollingComponent(15000) {}

    void setup() override;
    void update() override;
    void loop() override;

    // ✅ Declare setter functions correctly
    void set_current_sensor(sensor::Sensor *sensor) { this->current_sensor = sensor; }
    void set_power_sensor(sensor::Sensor *sensor) { this->power_sensor = sensor; }
    void set_adc_source(voltage_sampler::VoltageSampler *source) { source_ = source; }

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
    voltage_sampler::VoltageSampler *source_ = nullptr;
    GPIOPin *pin_ = nullptr;
    int csmpin_ = 99;
    bool is_sampling_ = false;
    // https://www.allegromicro.com/-/media/files/datasheets/acs712-datasheet.ashx
    uint32_t sample_duration_ = 100;
    uint32_t sample_wait_before_ = 280;
    float sample_sum_ = 0.0f;
    uint32_t num_samples_ = 0;


};

}  // namespace acs712
}  // namespace esphome
