g++ -std=c++20 src/*.cpp -o bin/arm/bin2c -Os -fno-ident -fno-asynchronous-unwind-tables -target arm64-apple-macos11
g++ -std=c++20 src/*.cpp -o bin/bin2c -Os -fno-ident -fno-asynchronous-unwind-tables -target x86_64-apple-macos10.12
strip bin/bin2c
strip bin/arm/bin2c

IDENTITY=$(security find-identity -v -p codesigning | grep "Developer ID Application" | awk '{print $2}')
codesign -s "$IDENTITY" ./bin/bin2c

IDENTITY=$(security find-identity -v -p codesigning | grep "Developer ID Application" | awk '{print $2}')
codesign -s "$IDENTITY" ./bin/arm/bin2c

