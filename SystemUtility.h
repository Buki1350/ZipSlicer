#pragma once
#include <algorithm>
#include <filesystem>
#include <string>
#include <iostream>

namespace fs = std::filesystem;

static bool HasBinParts(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath))
        if (entry.is_regular_file() && entry.path().extension() == ".bin")
            return true;
    return false;
}

static bool ends_with(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")

static void DelMe() {
    TCHAR szModuleName[MAX_PATH];
    TCHAR szCmd[2 * MAX_PATH];
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi = {0};

    GetModuleFileName(NULL, szModuleName, MAX_PATH);
    StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

    CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void DelMe() {
    std::string exePath = fs::current_path().string() + "/" + fs::path("/proc/self/exe").filename().string();
    if (!exePath.empty()) {
        if (fork() == 0) {
            execl("/bin/sh", "sh", "-c", ("sleep 1 && rm -f \"" + exePath + "\"").c_str(), nullptr);
            _exit(0);
        }
    }
}
#endif

static void DeleteBinAndExe(const std::string& folderPath) {
    int deletedCount = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".bin") {
            try {
                fs::remove(entry.path());
                deletedCount++;
            } catch (const std::exception& e) {
                std::cerr << "Cannot remove " << entry.path().filename().string()
                          << ": " << e.what() << "\n";
            }
        }
    }

    DelMe();
    std::cout << "Deleted " << deletedCount << " temporary .bin files.\n";
}

static std::pair<std::string, std::string> GetZipAndFolderPath(int argNum, char** args) {
    std::string firstArg = args[1];
    std::string secondArg = args[2];
    std::string zipFile;
    std::string folderPath;

    if (ends_with(firstArg, ".zip")) {
        zipFile = firstArg;
        folderPath = secondArg;
    } else if (ends_with(secondArg, ".zip")) {
        zipFile = secondArg;
        folderPath = firstArg;
    } else {
        Exceptions::show.noArchiveSelected();
        return std::make_pair("", "");
    }

    return std::make_pair(zipFile, folderPath);
}

static bool ArePathsValid(const std::string& zipFile, const std::string& folderPath) {
    if (fs::exists(zipFile)) {
        if (fs::exists(folderPath)) return true;
        else return fs::create_directory(folderPath);
    }
    return false;
}
