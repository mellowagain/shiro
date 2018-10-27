git clone "https://github.com/Microsoft/vcpkg"
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
$newPath = Get-Location
Write-Host "##vso[task.setvariable variable=PATH;]${env:PATH};$newPath";
