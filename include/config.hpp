#pragma once

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    int getBitrate() const { return bitrate; }
    void setBitrate(int mbps) { bitrate = mbps; }

    int getBufferDelayMs() const { return bufferDelayMs; }
    void setBufferDelayMs(int ms) { bufferDelayMs = ms; }

    int getOCProfile() const { return ocProfile; }
    void setOCProfile(int profile) { ocProfile = profile; }

private:
    Config() : bitrate(25), bufferDelayMs(0), ocProfile(1) {}
    
    int bitrate;        // Мбит/с
    int bufferDelayMs;  // Буферизация кадров (мс)
    int ocProfile;      // 0 = Stock, 1 = Boost, 2 = Max OC
};
