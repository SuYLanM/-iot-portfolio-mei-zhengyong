param(
    [Parameter(Mandatory)][string]$OutputDirectory,
    [Parameter(Mandatory)][ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Run,
    [string]$Port = 'COM3',
    [ValidateSet(32, 150, 500, 1000)][int]$Samples = 150
)
$ErrorActionPreference = 'Stop'
New-Item -ItemType Directory -Force $OutputDirectory | Out-Null
$rawPath = Join-Path $OutputDirectory "$Run.serial.txt"
$csvPath = Join-Path $OutputDirectory "$Run.csv"
if ((Test-Path $rawPath) -or (Test-Path $csvPath)) {
    throw "Run already exists: $Run"
}
$serial = [IO.Ports.SerialPort]::new($Port, 115200, 'None', 8, 'One')
$serial.DtrEnable = $false
$serial.RtsEnable = $false
$serial.ReadTimeout = 500
$bytes = [IO.MemoryStream]::new()
try {
    $serial.Open()
    Start-Sleep -Milliseconds 1400
    $serial.DiscardInBuffer()
    $command = @{32 = 'l'; 150 = 't'; 500 = 'L'; 1000 = 'W'}[$Samples]
    $serial.Write($command)
    $deadline = [DateTime]::UtcNow.AddSeconds($Samples * 0.02 + 10)
    $buffer = [byte[]]::new(4096)
    while ([DateTime]::UtcNow -lt $deadline) {
        try {
            $count = $serial.Read($buffer, 0, $buffer.Length)
        } catch [TimeoutException] {
            continue
        }
        $bytes.Write($buffer, 0, $count)
        if ([Text.Encoding]::ASCII.GetString($bytes.ToArray()) -match '# END\r?\n') {
            break
        }
    }
} finally {
    $serial.Dispose()
    [IO.File]::WriteAllBytes($ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($rawPath), $bytes.ToArray())
    $bytes.Dispose()
}
$serialText = Get-Content -Raw $rawPath
$match = [regex]::Match($serialText, '(?m)^sample,t_ms,raw,baseline_mv,mean5,median5,ema,ldr_mv\r?\n([\s\S]*?)^# END\r?\n')
if (!$match.Success) { throw "Incomplete capture. Original bytes saved to $rawPath" }
$csv = $match.Value.Substring(0, $match.Value.LastIndexOf('# END'))
$rows = @($csv | ConvertFrom-Csv)
if ($rows.Count -ne $Samples) { throw "Expected $Samples samples; got $($rows.Count)" }
for ($i = 0; $i -lt $rows.Count; $i++) {
    if ($rows[$i].sample -ne "$i" -or $rows[$i].raw -notmatch '^\d+$' -or
        [int]$rows[$i].raw -gt 4095 -or $rows[$i].ldr_mv -notmatch '^\d+$') {
        throw "Invalid row $i"
    }
}
[IO.File]::WriteAllText($ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($csvPath), $csv.Replace("`r`n", "`n"))
$rows | Measure-Object -Property raw -Minimum -Maximum -Average -StandardDeviation
$rows | Measure-Object -Property ldr_mv -Minimum -Maximum -Average
