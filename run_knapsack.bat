@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

rem — директория с входными файлами
set "data_dir=%~dp0data"
if not exist "%data_dir%\" (
    echo ERROR: data directory "%data_dir%" not found.
    pause
    exit /b 1
)

rem — подготовка результирующего CSV
set "out_file=%~dp0results.csv"
if exist "%out_file%" del "%out_file%"
echo filename,total_value,total_weight,duration_ms,selected_items>"%out_file%"

rem — цикл по всем файлам (любой тип) в data_dir
for %%F in ("%data_dir%\*") do (
    if not exist "%%~fF\" (
        echo Processing: %%~nxF

        rem — запускаем программу один раз
        "%~dp0greedy_knapsack.exe" < "%%~fF" > "%~dp0temp_knap_out.txt"

        rem — сброс переменных
        set "val=" & set "wt=" & set "dur=" & set "items="

        rem — парсим total_value
        for /f "tokens=2 delims= " %%a in ('
            findstr /b /c:"total_value " "%~dp0temp_knap_out.txt"
        ') do set "val=%%a"

        rem — парсим total_weight
        for /f "tokens=2 delims= " %%a in ('
            findstr /b /c:"total_weight " "%~dp0temp_knap_out.txt"
        ') do set "wt=%%a"

        rem — парсим duration_ms
        for /f "tokens=2 delims= " %%a in ('
            findstr /b /c:"duration_ms " "%~dp0temp_knap_out.txt"
        ') do set "dur=%%a"

        rem — парсим selected_items
        for /f "tokens=1,* delims= " %%a in ('
            findstr /b /c:"selected_items " "%~dp0temp_knap_out.txt"
        ') do set "items=%%b"

        rem — запись в CSV или лог ошибок
        if defined val (
            echo %%~nxF,!val!,!wt!,!dur!,!items!>> "%out_file%"
        ) else (
            echo WARNING: invalid output for %%~nxF >> "%~dp0errors.log"
        )
    )
)

del "%~dp0temp_knap_out.txt"
echo Done. Results in "%out_file%"
pause
