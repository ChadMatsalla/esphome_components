#include "acs712_component.h"
#include <driver/gpio.h>

namespace esphome {
namespace acs712 {
    static const char *const TAG = "acs712";
void ACS712Sensor::setup() {
    // ESP_LOGD("acs712", "Setting up ACS712 sensor on GPIO %d", this->csmpin_);
    // ACS712 *ACS = new ACS712(this->csmpin_, 5, 1023, 100);
    if (1==2) {
        this->ACS->autoMidPoint();
        ESP_LOGD("acs712", "MidPoint: %d", this->ACS->getMidPoint());
        this->ACS->setNoisemV(43);
        ESP_LOGD("acs712", "Noise mV: %d", this->ACS->getNoisemV());
        ESP_LOGD("acs712", "GPIO is %d", this->csmpin_);
    }
    ESP_LOGD(TAG,"Chad was here");
}
void ACS712Sensor::loop() {
    if (!this->is_sampling_)
        return;
}

void ACS712Sensor::update() {
    is_sampling_ = true;

    this->set_timeout("read", this->sample_duration_, [this]() {
      this->is_sampling_ = false;
      if (this->num_samples_ == 0)
        return;
  
      float mean = this->sample_sum_ / float(this->num_samples_);
      ESP_LOGD(TAG, "ADC read voltage: %.3f V (mean from %" PRId32 " samples)", mean, this->num_samples_);
  
      // PM2.5 calculation
      // ref: https://www.howmuchsnow.com/arduino/airquality/
      int16_t pm_2_5_value = 170 * mean;
      this->publish_state(pm_2_5_value);
    });
  
    // reset readings
    this->num_samples_ = 0;
    this->sample_sum_ = 0.0f;
}

}  // namespace acs712
}  // namespace esphome
