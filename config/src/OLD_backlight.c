#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(backlight);

#define BACKLIGHT_NODE DT_NODELABEL(backlight_pwm)

static const struct device *pwm_dev = DEVICE_DT_GET(DT_PWMS_CTLR(BACKLIGHT_NODE));
static const uint32_t pwm_channel = DT_PWMS_CHANNEL(BACKLIGHT_NODE);
static const uint32_t pwm_period = DT_PWMS_PERIOD(BACKLIGHT_NODE);

int ret = pwm_set(pwm_dev, pwm_channel, PWM_USEC(1000), PWM_USEC(500), PWM_POLARITY_NORMAL);

void backlight_set(uint8_t brightness_percent) {
    if (!device_is_ready(pwm_dev)) {
        LOG_ERR("PWM device not ready");
        return;
    }

    uint32_t pulse = (brightness_percent * pwm_period) / 100;
    int ret = pwm_set(pwm_dev, pwm_channel, pwm_period, pulse, 0);
    if (ret) {
        LOG_ERR("Failed to set PWM: %d", ret);
    }
}
