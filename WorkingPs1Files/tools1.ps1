param (
    [string]$Action
)

switch ($Action.ToLower()) {  # Makes comparison case-insensitive
    "clean" {
        Write-Host "Running clean..."
        Remove-Item -Recurse -Force .\build\
    }
    "configure" {
        Write-Host "Running configure with changes..."
        cmake -G Ninja -B build -S . 
    }
    "build" {
        Write-Host "Running build with changes..."
        ninja -C build
    }
    "run" {
        Write-Host "Running current build"
        .\build\Graphiti_API_Library_0_1.exe
    }
    "cleanConfigureBuild" {
        Write-Host "Running clean..."
        Remove-Item -Recurse -Force .\build\
        Write-Host "Running configure..."
        cmake -G Ninja -B build -S . 
        Write-Host "Running build..."
        ninja -C build
    }
    "all" {
        Write-Host "Running clean..."
        Remove-Item -Recurse -Force .\build\
        Write-Host "Running configure..."
        cmake -G Ninja -B build -S . 
        Write-Host "Running build..."
        ninja -C build
        Write-Host "Running current build"
        .\build\Graphiti_API_Library_0_1.exe
    }
    default {
        Write-Host "Error: Unknown action '$Action'. Valid options: 'clean', 
        'configure', 'build', 'cleanConfigureBuild', 'run', 'all'"
        exit 1
    }
}