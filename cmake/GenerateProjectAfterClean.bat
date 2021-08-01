cd ..

if exist bin (
    del /f /s /q bin
)

if exist project (
    del /f /s /q project
) else (
    mkdir project
)

cd project 
cmake ..
PAUSE