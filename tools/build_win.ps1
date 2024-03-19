# --- Parse Commands --------------------------------------
$commands = @{};
foreach ($arg in $args) {
    $parts = $arg -split "="
    $commands[$parts[0]] = if ($parts.Length -gt 1) { $parts[1] } else { 1 }
}

if ($null -eq $commands["output"]) {
    $commands["output"] = "$PSScriptRoot/../dist"
}
else {
    $commands["output"] = "$PSScriptRoot/../$($commands["output"])"
}

if ($null -eq $commands["version"]) {
    $commands["version"] = $(git describe --tags)
}

# --- Set Compile Flags -----------------------------------
if (-not $commands["release"]) { $commands["debug"] = 1; }
$compile_args = @();
if ($commands["release"]) {
    Write-Host "[BUILD] release mode" 
    $compile_args += "/O2";
    $compile_args += "/DBUILD_DEBUG=0"
}

if ($commands["debug"]) {
    Write-Host "[BUILD] debug mode"; 
    $compile_args += "/Z7";
    $compile_args += "/Od";
    $compile_args += "/DBUILD_DEBUG=1"
}


# --- Common Compile Flags ---------------------------------
$compile_args += "/nologo"
$compile_args += "/I..\opus\include"
$compile_args += "/I..\opus\src"
$compile_args += "/DSTB_IMAGE_IMPLEMENTATION"
$compile_args += "/DVERSION_NUMBER=\""$($commands["version"])\"""

# --- Libs -------------------------------------------------
$compile_args += "..\opus\lib\soloud_x64_d.lib";
$compile_args += "..\opus\lib\freetype.lib";
$compile_args += "..\opus\lib\glfw3dll.lib";
$compile_args += "..\opus\lib\glfw3.lib";
$compile_args += "..\opus\lib\gl.obj";
$compile_args += "opengl32.lib";


# --- Set Link Flags --------------------------------------
$link_args = @();
$link_args += "/out:main.exe"

# --- Set Build Target ------------------------------------
$source_file_path = ""
if ($commands["game"]) { $source_file_path = "../src/main.c" }


New-Item -ItemType Directory -Force -Path $($commands["output"]) | Out-Null;
Push-Location $($commands["output"]) | Out-Null
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
Write-Host "[BUILD] target: $source_file_path";
cl $compile_args $source_file_path /link $link_args;
Write-Host "[BUILD] time: $(($stopwatch.ElapsedMilliseconds / 1000).ToString())s"
if ($commands["run"]) { Invoke-Expression "$($commands["output"])\main.exe" }
Pop-Location;

