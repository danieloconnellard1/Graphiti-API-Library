param (
    [string]$Action = "buildrun"
)

function clean {
    Write-Host "Running clean..."
    Remove-Item -Recurse -Force .\build\
}

function configure {
    param (
        [switch]$Debug,
        [switch]$NoExtension
    )

    Write-Host "Running configure..."

    $version = @()

    if ($Debug) {
        $version += "-DCMAKE_BUILD_TYPE=Debug"
        Write-Host "-Debug enabled"
    }

    if ($NoExtension) {
        $version += "-DUSE_EXTENSION=OFF"
        Write-Host "-Extension disabled"
    }

    cmake -G Ninja -B build -S . @version
}

function build {
    Write-Host "Running build with changes..."
    ninja -C build
}

function run_ExtensionTest {
    Write-Host "Running current build"
    .\build\ExtensionTest.exe
}

function run_Testing {
    Write-Host "Running current build"
    .\build\Testing.exe
}

switch ($Action.ToLower()) {  # Makes comparison case-insensitive
    "clean" { clean }
    "configure" { configure }
    "configure_debug"{ configure -Debug }
    "build" { build }
    "run" { run_Testing }
    "full_build" {
        clean
        configure
        build
    }
    "standard" {
        clean
        configure
        build
        run
    }
    "standard_debug" {
        clean
        configure -Debug
        build
        run
    }
    "standard_noextension" {
        clean
        configure -NoExtension
        build
        run
    }
    "buildrun" {
        build
        run_Testing
    }
    "all" {
        clean
        configure -Debug
        build
        run_Testing
    }
    default {
        Write-Host "Error: Unknown action '$Action'. Valid options: 'clean', 
        'configure', 'configure_debug', 'build', 'run', 'cleanConfigureBuild', 
        'standard', 'standard_debug', 'standard_extension', 'all'"
        exit 1
    }
}