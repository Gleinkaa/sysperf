# start.ps1 — Windows entry point. sysperf is a LINUX-ONLY desktop app: it reads
# /proc, /sys, NVML and DRM fdinfo and cannot build or run natively on Windows.
# This wrapper runs the (cross-platform) preflight for sanity, then routes the real
# build+run into WSL if an Ubuntu distro is available.

$ErrorActionPreference = "Stop"
Set-Location -Path (Split-Path -Parent $PSScriptRoot)

Write-Host "sysperf is a Linux-only desktop app; Windows is source storage only." -ForegroundColor Yellow

$bash = Get-Command bash -ErrorAction SilentlyContinue
if ($bash) {
    Write-Host "--- running cross-platform preflight via bash ---"
    & bash scripts/preflight.sh
}

$wsl = Get-Command wsl -ErrorAction SilentlyContinue
if ($wsl) {
    $distros = (& wsl -l -q) 2>$null
    if ($LASTEXITCODE -eq 0 -and $distros) {
        Write-Host "--- building + running inside WSL ---"
        # Translate the current Drive path to a /mnt path for WSL.
        $winPath = (Get-Location).Path
        $drive = $winPath.Substring(0,1).ToLower()
        $rest  = $winPath.Substring(2) -replace '\\','/'
        $wslPath = "/mnt/$drive$rest"
        & wsl bash -lc "cd '$wslPath' && ./scripts/start.sh"
        exit $LASTEXITCODE
    }
}

Write-Error "No usable WSL/Linux environment found. Build and run on Ubuntu: ./scripts/start.sh"
exit 1
