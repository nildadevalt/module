param(
    [Parameter(Mandatory=$true)][string]$ScriptPath,
    [int]$WaitSeconds = 15,
    [string]$PlaceId = '79035306837882'
)
$ErrorActionPreference = 'Stop'
function Read-SharedLog([string]$Path) {
    $file = [IO.FileStream]::new($Path, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::ReadWrite -bor [IO.FileShare]::Delete)
    $reader = [IO.StreamReader]::new($file)
    try { $reader.ReadToEnd() } finally { $reader.Dispose() }
}
$players = @(Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue)
foreach ($player in $players) {
    Stop-Process -Id $player.Id -Force
    if (-not $player.WaitForExit(10000)) { throw 'Roblox did not exit' }
}
if (Get-Process -Name RobloxPlayerBeta -ErrorAction SilentlyContinue) { throw 'Roblox is still running' }
$moduleLog = 'C:\Users\LASSE\AppData\Local\Roblox\Versions\version-4ea8e85481e34f46\Logs.log'
$start = Get-Date
$originalLength = if (Test-Path -LiteralPath $moduleLog) { (Read-SharedLog $moduleLog).Length } else { 0 }
Start-Process "roblox://placeId=$PlaceId"
$deadline = (Get-Date).AddSeconds(35)
$ready = $false
while ((Get-Date) -lt $deadline) {
    Start-Sleep -Milliseconds 250
    $all = Read-SharedLog $moduleLog
    $session = $all.Substring([Math]::Min($originalLength, $all.Length))
    if ($session.Contains('[TaskScheduler] Execution initialized:')) { $ready = $true; break }
}
if (-not $ready) { Write-Output $session; throw 'Execution did not initialize' }
$scriptBytes = [Text.Encoding]::UTF8.GetBytes([IO.File]::ReadAllText((Resolve-Path -LiteralPath $ScriptPath)))
$lengthBytes = [BitConverter]::GetBytes([Net.IPAddress]::HostToNetworkOrder([int]$scriptBytes.Length))
$client = [Net.Sockets.TcpClient]::new()
try {
    $client.Connect('127.0.0.1',6969)
    $stream = $client.GetStream()
    $stream.Write($lengthBytes,0,$lengthBytes.Length)
    $stream.Write($scriptBytes,0,$scriptBytes.Length)
    $stream.Flush()
} finally { $client.Dispose() }
Write-Output "Submitted probe ($($scriptBytes.Length) bytes)"
Start-Sleep -Seconds $WaitSeconds
$all = Read-SharedLog $moduleLog
$session = $all.Substring([Math]::Min($originalLength, $all.Length))
$session | Set-Content -LiteralPath '.maintenance\latest-probe-module.log' -Encoding UTF8
$session -split "`n" | Select-Object -Last 80 | Write-Output
$log = rg --files 'C:\Users\LASSE\AppData\Local\Roblox\logs' -g '*Player*last.log' |
    Where-Object { $_ -notlike '*CrashHandler*' -and $_ -notlike '*\crashes\*' } |
    ForEach-Object { Get-Item -LiteralPath $_ } |
    Where-Object { $_.LastWriteTime -ge $start.AddSeconds(-2) } |
    Sort-Object LastWriteTime -Descending | Select-Object -First 1
if ($log) {
    Write-Output "Player log: $($log.FullName)"
    $results = @(Select-String -LiteralPath $log.FullName -Pattern 'MYR [0-9]+\|')
    if ($results.Count) { Write-Output "Myriad completed=$($results.Count) passed=$(@($results | Where-Object { $_.Line -match 'MYR 200\|' }).Count)" }
    Select-String -LiteralPath $log.FullName -Pattern 'NERO_|MYR-PLACE|MYR-START|MYR-DONE|MYR (?!200\|)[0-9]+\|' |
        ForEach-Object { $_.Line }
}
