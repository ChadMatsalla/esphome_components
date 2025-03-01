#include "acs712_component.h"
#include <driver/gpio.h>

namespace esphome {
namespace acs712_external {

void ACS712Sensor::setup() {
    ESP_LOGD("acs712", "Setting up ACS712 sensor on GPIO %d", this->csmpin_);
    // ACS712 *ACS = new ACS712(this->csmpin_, 5, 1023, 100);

    this->ACS->autoMidPoint();
    ESP_LOGD("acs712", "MidPoint: %d", this->ACS->getMidPoint());
    this->ACS->setNoisemV(43);
    ESP_LOGD("acs712", "Noise mV: %d", this->ACS->getNoisemV());
    ESP_LOGD("acs712", "GPIO is %d", this->csmpin_);
}

void ACS712Sensor::update() {
    float average = 0;
    int count = 5;
    for (int i = 0; i < count; i++) {
        average += this->ACS->mA_AC();
    }
    float amps = (average / count / 10000.0) - 0.13;
    if (amps < 0.03) {
        amps = 0.0;
    }

    if (this->current_sensor != nullptr) {  // ✅ Check if sensor is initialized
        this->current_sensor->publish_state(amps);
    }
    if (this->power_sensor != nullptr) {  // ✅ Check if sensor is initialized
        this->power_sensor->publish_state(amps * 230);
    }
}

}  // namespace acs712_external
}  // namespace esphome
