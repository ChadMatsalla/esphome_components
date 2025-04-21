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
    ESP_LOGD(TAG,"Midpoint: %f", this->midpoint_);
    ESP_LOGD(TAG,"Max Amps: %d", this->maxamps_);
    ESP_LOGD(TAG,"Max Volts: %f", this->maxvolts_);

}
void ACS712Sensor::loop() {
    if (!this->is_sampling_)
        return;
    // ESP_LOGD(TAG,"Reading voltage...");
    float read_voltage = this->source_->sample();
    // ESP_LOGD(TAG,"Done reading voltage...");
    if (std::isnan(read_voltage))
        return;
    
    // ESP_LOGD(TAG, "ACS712Sensor loop read_voltage: %.3f V", read_voltage);
    // read_voltage = read_voltage;
    //  * this->voltage_multiplier_ - this->voltage_offset_;
    this->num_samples_++;
    this->sample_sum_ += read_voltage;  
}

void ACS712Sensor::update() {
    // ESP_LOGD(TAG, "ACS712Sensor update");
    is_sampling_ = true;
    this->set_timeout("read", this->sample_duration_, [this]() {
      this->is_sampling_ = false;
      if (this->num_samples_ == 0)
        return;
      float mean = this->sample_sum_ / float(this->num_samples_);
      ESP_LOGD(TAG, "ADC read voltage: %.3f V (mean from %" PRId32 " samples)", mean, this->num_samples_);
      int16_t pm_2_5_value = 170 * mean;
      this->publish_state(pm_2_5_value);
    });
    // reset readings
    this->num_samples_ = 0;
    this->sample_sum_ = 0.0f;
}

}  // namespace acs712
}  // namespace esphome
