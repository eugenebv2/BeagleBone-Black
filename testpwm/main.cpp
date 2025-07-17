#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <unistd.h>

class BBBPWMDevice {
private:
    std::string pwmPath;
    int dutyCycle;
    int period;

public:
//    BBBPWMDevice(const std::string& pwmPin) {
//        pwmPath = "/sys/class/pwm/" + pwmPin + "/";
//        dutyCycle = 0;
//        period = 0;
//    }
//
    BBBPWMDevice(const std::string& pwmPin, const std::string& pwmChannel) {
        pwmPath = "/dev/bone/pwm/" + pwmPin + "/" + pwmChannel + "/";
        dutyCycle = 0;
        period = 0;
    }

    void setPeriod(int period_ns) {
        period = period_ns;
        std::ofstream periodFile(pwmPath + "period");
        if (!periodFile) {
            throw std::runtime_error("Failed to set period");
        }
        periodFile << period_ns;
        periodFile.close();
    }

    void setDutyCycle(int duty_ns) {
        dutyCycle = duty_ns;
        std::ofstream dutyFile(pwmPath + "duty_cycle");
        if (!dutyFile) {
            throw std::runtime_error("Failed to set duty cycle");
        }
        dutyFile << duty_ns;
        dutyFile.close();
    }

    void enablePWM(bool enable) {
        std::ofstream enableFile(pwmPath + "enable");
        if (!enableFile) {
            throw std::runtime_error("Failed to enable/disable PWM");
        }
        enableFile << (enable ? "1" : "0");
        enableFile.close();
    }

    int getDutyCycle() const {
        return dutyCycle;
    }

    int getPeriod() const {
        return period;
    }
};

int main() {
	long pwmPeriod = 1000000;
	float step = 0.2;
	float min = 0.2;
	float max = 1.0;
	float brightness = min;
    try {
        //BBBPWMDevice pwm("pwmchip3/pwm1");
        BBBPWMDevice pwm("1", "b");
        pwm.setPeriod(pwmPeriod);  // 20ms period
        pwm.setDutyCycle(pwmPeriod);  // 1.5ms duty cycle
        pwm.enablePWM(true);

        std::cout << "PWM enabled with period: " << pwm.getPeriod()
                  << " ns and duty cycle: " << pwm.getDutyCycle() << " ns" << std::endl;
	sleep(2);
	pwm.setDutyCycle(0);

        std::cout << "PWM enabled with period: " << pwm.getPeriod()
                  << " ns and duty cycle: " << pwm.getDutyCycle() << " ns" << std::endl;

	sleep(2);
	pwm.setDutyCycle(pwmPeriod);

        std::cout << "PWM enabled with period: " << pwm.getPeriod()
                  << " ns and duty cycle: " << pwm.getDutyCycle() << " ns" << std::endl;
	sleep(1);

	while(1) {
		pwm.setDutyCycle(pwmPeriod*brightness);
		brightness += step;
		if(brightness >= max || brightness <= min)
			step = -1 * step;
		//std::cout << brightness << " : " << step << std::endl;
		sleep(1);
	}
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

