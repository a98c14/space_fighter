$app_name = "test";

Push-Location $PSScriptRoot/../
.\tools\build_win.ps1 ".\src\scratch\test.c" $app_name; 
if ($?) { 
    Push-Location .\dist\
    Write-Host "[BUILD] launching $app_name.exe";
    Invoke-Expression ".\$app_name.exe"
    Pop-Location
}