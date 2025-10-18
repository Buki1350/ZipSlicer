#include "Exceptions.h"
#include "Slicer.h"
#include "SystemUtility.h"
#include "Utility.h"
#include "Rebuilder.h"
#include <iostream>
#include <string>
#include <strsafe.h>

#define REBUILD_ARCHIVE_NAME  "rebuilt_archive.zip"

enum ModeBasedOnArgNum {
    AUTO_REBUILD = 1,
    REBUILD = 3,
    SLICE = 4,
    SLICE_WITH_UNIT = 5,
};

int main(int argNum, char** args) {

    switch (argNum) {
        case AUTO_REBUILD: {
            std::string currentDir = fs::current_path().string();

            if (!HasBinParts(currentDir)) { Exceptions::show.noBinariesFound(); return 1; }

            std::string outPath = currentDir + "\\" + REBUILD_ARCHIVE_NAME;
            Rebuild(currentDir, outPath);
            DeleteBinAndExe(currentDir);
            return 0;
        }
        case REBUILD: {
            auto [zipFile, folderPath] = GetZipAndFolderPath(argNum, args);

            if (!HasBinParts(folderPath)) { Exceptions::show.noBinariesFound(); return 1; }

            Rebuild(folderPath, zipFile);
            DeleteBinAndExe(folderPath);
            break;
        }
        case SLICE: {
            auto [zipFile, folderPath] = GetZipAndFolderPath(argNum, args);

            bool pathsValid = ArePathsValid(zipFile, folderPath);
            if (!pathsValid) { Exceptions::show.invalidPaths(); return 1; }
            
            long long partSize = std::stoll(args[3]);
            partSize = ConvertToBytes(partSize, "-b");

            Slice(zipFile, folderPath, partSize);
            break;
        }
        case SLICE_WITH_UNIT: {
            auto [zipFile, folderPath] = GetZipAndFolderPath(argNum, args);

            bool pathsValid = ArePathsValid(zipFile, folderPath);
            if (!pathsValid) { Exceptions::show.invalidPaths(); return 1; }
            
            long long partSize = std::stoll(args[3]);
            std::string unit = args[4];
            partSize = ConvertToBytes(partSize, unit);
        
            Slice(zipFile, folderPath, partSize);
            break;
        }
        default:
            Exceptions::show.usage();
            break;
    }

    return 0;
}

