#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

static void Slice(const std::string& filePath, const std::string& archivePath, int partSize_inBytes) {
    std::ifstream in(filePath, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "[!] Cannot open input file.\n";
        return;
    }

    std::streamsize totalSize = in.tellg();
    in.seekg(0, std::ios::beg);

    int partId = 0;
    std::vector<char> buffer(partSize_inBytes);

    // ... slicing
    while (totalSize > 0) {
        std::streamsize toRead = std::min<std::streamsize>(partSize_inBytes, totalSize);

        // ... reading bytes to toRead
        in.read(buffer.data(), toRead);

        std::string partFile = (fs::path(archivePath) / ("part_" + std::to_string(partId) + ".bin")).string();
        std::ofstream out(partFile, std::ios::binary);
        if (!out) {
            std::cerr << "[!] Cannot create part file.\n";
            return;
        }

        out.write(reinterpret_cast<const char*>(&partId), sizeof(int));
        out.write(buffer.data(), toRead);
        out.close();

        totalSize -= toRead;
        partId++;
    }

    in.close();
    std::cout << "[>] Slicing completed into " << partId << " parts.\n";
}
