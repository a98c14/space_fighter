$app_name = "main";
$run = $args[0];

Push-Location $PSScriptRoot/../
.\tools\build_win.ps1 ".\src\main.c" $app_name; 
if ($? -and $run) { 
    Push-Location .\dist\
    Write-Host "[BUILD] launching $app_name.exe";
    Invoke-Expression ".\$app_name.exe"
    Pop-Location
}