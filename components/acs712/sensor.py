# from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, voltage_sampler
from esphome.const import (
  UNIT_AMPERE,
  UNIT_WATT,
  ICON_FLASH,
  ICON_CURRENT_AC,
  CONF_PIN,
  CONF_ID,
  CONF_SENSOR,
)

DEPENDENCIES = []
AUTO_LOAD = ["voltage_sampler"]

acs712_ns = cg.esphome_ns.namespace("acs712")
ACS712Sensor = acs712_ns.class_("ACS712Sensor",
    sensor.Sensor,
    cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        ACS712Sensor,
        unit_of_measurement=UNIT_AMPERE,
        icon=ICON_CURRENT_AC,
        accuracy_decimals=2,
    ).extend({
        # cv.Required(CONF_PIN): cv.All(pins.internal_gpio_input_pin_schema),
        cv.Required(CONF_SENSOR): cv.use_id(voltage_sampler.VoltageSampler),
        cv.Required("midpoint"): cv.float_range(0.0, 3.3),
        cv.Required("max_amps"): cv.int_range(5, 30),
        cv.Required("max_volts"): cv.float_range(1, 3.3),
        cv.Optional("current"): sensor.sensor_schema(),
        cv.Optional("power"): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_FLASH,
            accuracy_decimals=2,
        ),
    }).extend(cv.polling_component_schema("60s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    # print(f"id is {var}")
    # pin = await cg.gpio_pin_expression(config[CONF_PIN])
    # print(f"Pin is {config[CONF_PIN]}")
    # pin_number = config[CONF_PIN]['number']
    # print(f"Pin number is {pin_number}")
    # cg.add(var.set_csmpin(pin_number))

    adc_sensor = await cg.get_variable(config[CONF_SENSOR])
    cg.add(var.set_adc_source(adc_sensor))
    cg.add(var.set_midpoint(config.get("midpoint", 1.65)))
    cg.add(var.set_max_amps(config.get("max_amps", 5)))
    cg.add(var.set_max_volts(config.get("max_volts", 3.3)))
    # cg.add(var.set_pin(pin))
    cg.add_library("RobTillaart/ACS712", "0.3.10")
    if "current" in config:
        sens = await sensor.new_sensor(config["current"])
        cg.add(var.set_current_sensor(sens))  # ✅ Use set_current_sensor()

    if "power" in config:
        sens = await sensor.new_sensor(config["power"])
        cg.add(var.set_power_sensor(sens))  # ✅ Use set_power_sensor()