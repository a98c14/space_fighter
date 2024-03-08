$output_path = ".\dist";
$source_file_path = $args[0];
$app_name = $args[1];
$debug_mode = 1;

$compile_args = @(
    # "/O2",
    "/Zi",
    "/nologo",
    "/Fd:""$output_path\$app_name.pdb""",
    "/Fo:""$output_path""\",
    "/I"".\include""",
    "/I"".\opus\include""",
    "/I"".\opus\src""",
    "/DDEBUG=$debug_mode",
    "/DSTB_IMAGE_IMPLEMENTATION",
    ".\opus\lib\freetype.lib",
    ".\opus\lib\glfw3dll.lib",
    ".\opus\lib\glfw3.lib",
    ".\opus\lib\gl.obj",
    "opengl32.lib");

$link_args = @(
    "/debug:full",
    "/out:""$output_path\$app_name.exe""",
    "/pdb:""$output_path\$app_name.pdb""");

Push-Location $PSScriptRoot/../ | Out-Null
New-Item -ItemType Directory -Force -Path $output_path | Out-Null;
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
Write-Host "[BUILD] source file: $source_file_path";
cl $compile_args $source_file_path /link $link_args;
Write-Host "[BUILD] time: $(($stopwatch.ElapsedMilliseconds / 1000).ToString())s"
Pop-Location