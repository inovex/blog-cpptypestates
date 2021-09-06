#include <type_traits>

namespace typedgpio {

// This stuff isn't really part of the example, as it wasn't specified in the original
// Rust code.
class GpioPeriphery {
public:
    virtual ~GpioPeriphery() {}

    virtual void enable_input() = 0;
    virtual void enable_output() = 0;
    virtual void disable() = 0;

    virtual void pullLow() = 0;
    virtual void pullHigh() = 0;

    virtual void set_bit(bool) = 0;
    virtual bool get_bit() = 0;
};

struct Disabled{};
struct Enabled{};
struct Output{};
struct Input{};
struct PulledLow{};
struct PulledHigh{};
struct HighZ{};
struct DontCare{};

template<typename State, typename Direction, typename Mode>
class GpioConfig {
public:
    using state_type = State;
    using direction_type = Direction;
    using mode_type = Mode;

    explicit GpioConfig(GpioPeriphery * periph) : mPeriph{periph} {}

    auto into_disabled() const -> GpioConfig<Disabled, DontCare, DontCare> {
        mPeriph->disable();
        return GpioConfig<Disabled, DontCare, DontCare>{mPeriph};
    }

    auto into_enabled_input() const -> GpioConfig<Enabled, Input, HighZ> {
        mPeriph->enable_input();
        return GpioConfig<Enabled, Input, HighZ>{mPeriph};
    }

    auto into_enabled_output() const -> GpioConfig<Enabled, Output, DontCare> {
        mPeriph->enable_output();
        return GpioConfig<Enabled, Output, DontCare>{mPeriph};
    }

    template<typename D = direction_type>
    void set_bit(bool high, typename std::enable_if<std::is_same_v<D, Output>, bool>::type = true) const {
        mPeriph->set_bit(high);
    }

    template<typename D = direction_type>
    bool bit_is_set(std::enable_if_t<std::is_same_v<D, Input>, bool> = true) const {
        return mPeriph->get_bit();
    }

    template<typename D = direction_type>
    auto into_input_pull_down(std::enable_if_t<std::is_same_v<D, Input>, bool> = true) const -> GpioConfig<Enabled, Input, PulledLow> {
        mPeriph->pullLow();
        return GpioConfig<Enabled, Input, PulledLow>{mPeriph};
    }

    template<typename D = direction_type>
    auto input_pull_up(std::enable_if_t<std::is_same_v<D, Input>, bool> = true) const -> GpioConfig<Enabled, Input, PulledHigh> {
        mPeriph->pullHigh();
        return GpioConfig<Enabled, Input, PulledHigh>{mPeriph};
    }

private:
    GpioPeriphery * mPeriph;
};

}  // namespace typedgpio
