#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static void Rebuild(const std::string& archivePath, const std::string& destinationFile) {
    std::vector<std::string> parts;

    // ... parts collecting
    for (const auto& entry : fs::directory_iterator(archivePath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bin") {
            parts.push_back(entry.path().string());
        }
    }

    if (parts.empty()) {
        std::cerr << "[!] No parts found in: " << archivePath << "\n";
        return;
    }

    // ... sort by id
    std::sort(parts.begin(), parts.end(), [](const std::string& a, const std::string& b) {
        std::ifstream fa(a, std::ios::binary);
        std::ifstream fb(b, std::ios::binary);
        int idA = -1, idB = -1;
        fa.read(reinterpret_cast<char*>(&idA), sizeof(int));
        fb.read(reinterpret_cast<char*>(&idB), sizeof(int));
        return idA < idB;
    });

    std::ofstream out(destinationFile, std::ios::binary);
    if (!out) {
        std::cerr << "[!] Cannot create output file.\n";
        return;
    }

    // ... reconstruction and save
    std::vector<char> buffer(1024 * 1024);
    for (const auto& part : parts) {
        std::ifstream in(part, std::ios::binary);
        int id;
        in.read(reinterpret_cast<char*>(&id), sizeof(int));
        while (in) {
            in.read(buffer.data(), buffer.size());
            std::streamsize bytesRead = in.gcount();
            if (bytesRead > 0) out.write(buffer.data(), bytesRead);
        }
        in.close();
    }

    out.close();
}