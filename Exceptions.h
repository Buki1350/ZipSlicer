#pragma once
#include <iostream>

class Exceptions {
    class ExceptionsInfo {
    public:
        static void usage() {
            std::cout << "\n=== [ USAGE ] ===\n"
                         "Slice:   FileSlicer.exe [archive path] [destination directory] [size] [unit optional]\n"
                         "or       FileSlicer.exe [destination directory] [archive path] [size] [unit optional]\n"
                         "\n"
                         "         Units: -b (bytes, default), -kb, -mb, -gb\n"
                         "         Example: FileSlicer -s my.zip parts 10 -mb\n"
                         "-------------------\n"
                         "Rebuild: FileSlicer [directory with parts] [destination archive path]\n"
                         "         or just run FileSlicer.exe in a folder with .bin files to auto rebuild\n\n";
        }
        
        static void noBinariesFound() {
            std::cout << "\n[!] No .bin parts found in the specified directory.\nUse \"ZipSlicer.exe ?\" for usage information.\n";
        }

        static void noArchiveSelected() {
            std::cout << "\n[!] No archive selected.\nUse \"ZipSlicer.exe ?\" for usage information.\n";
        }

        static void invalidPaths() {
            std::cout << "\n[!] One or both specified paths are invalid.\nUse \"ZipSlicer.exe ?\" for usage information.\n";
        }
    };

public:
    static ExceptionsInfo show;
};

inline Exceptions::ExceptionsInfo Exceptions::show;
