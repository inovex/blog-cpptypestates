#include <cstdio>

#include "typedgpio.hpp"

using namespace typedgpio;

namespace {

class GpioMock : public GpioPeriphery {
public:
    ~GpioMock() override {}

    void enable_input() override {
        puts("enabled as input");
    }

    void enable_output() override {
        puts("enabled as output");
    }

    void disable() override {
        puts("disabled");
    }

    void pullLow() override {
        puts("pull low");
    }

    void pullHigh() override {
        puts("pull high");
    }

    void set_bit(bool value) override {
        if (value) {
            puts("set high");
        } else {
            puts("set low");
        }
    }

    bool get_bit() override {
        puts("read");
        return false;
    }
};

GpioConfig<Disabled, DontCare, DontCare> get_pin() {
    static auto sInstance = GpioMock{};
    return GpioConfig<Disabled, DontCare, DontCare>{&sInstance};
}

}  // namespace

int main() {
	auto pin = get_pin();

    // Can't do this, pin isn't enabled!
    // pin.into_input_pull_down();

    // Now turn the pin from unconfigured to a high-z input
    auto input_pin = pin.into_enabled_input();

    // Read from the pin
    auto pin_state = input_pin.bit_is_set();

    // Can't do this, input pins don't have this interface!
    // input_pin.set_bit(true);

    /*
     * Example 2: High-Z input to Pulled Low input
     */
    auto pulled_low = input_pin.into_input_pull_down();
    pin_state = pulled_low.bit_is_set();

    /*
     * Example 3: Pulled Low input to Output, set high
     */
    auto output_pin = pulled_low.into_enabled_output();
    output_pin.set_bit(true);

    // Can't do this, output pins don't have this interface!
    // output_pin.into_input_pull_down();
}
