#pragma once

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    int getBitrate() const { return bitrate; }
    void setBitrate(int mbps) { bitrate = mbps; }

    int getBufferIndex() const { return bufferIndex; }
    void setBufferIndex(int index) { bufferIndex = index; }

    int getOCProfile() const { return ocProfile; }
    void setOCProfile(int profile) { ocProfile = profile; }

    int getResolutionIndex() const { return resIndex; }
    void setResolutionIndex(int index) { resIndex = index; }

    bool getStatsOverlay() const { return statsOverlay; }
    void setStatsOverlay(bool enable) { statsOverlay = enable; }

private:
    Config() : bitrate(25), bufferIndex(0), ocProfile(1), resIndex(0), statsOverlay(true) {}
    
    int bitrate;        // Битрейт в Мбит/с (5 - 50)
    int bufferIndex;    // 0 = Ultra Low (0ms), 1 = Balanced (10ms), 2 = Safe (30ms)
    int ocProfile;      // 0 = Off, 1 = Boost (1224MHz), 2 = MAX OC (1785MHz)
    int resIndex;       // 0 = 720p (Native Handheld), 1 = 1080p (TV Docked)
    bool statsOverlay;  // Отображение оверлея FPS / Ping
};
