param(
    [Parameter(Mandatory)][string]$OutputDirectory,
    [Parameter(Mandatory)][ValidateRange(0, 10)][int]$Trial,
    [string]$Port = 'COM3'
)

$ErrorActionPreference = 'Stop'
$name = 'trial-{0:D2}' -f $Trial
New-Item -ItemType Directory -Force $OutputDirectory | Out-Null
$outputPath = (Resolve-Path -LiteralPath $OutputDirectory).Path
$rawPath = Join-Path $outputPath "$name.serial.txt"
if (Test-Path -LiteralPath $rawPath) {
    throw "Capture already exists for $name"
}
$serial = [IO.Ports.SerialPort]::new($Port, 115200, 'None', 8, 'One')
$serial.DtrEnable = $false
$serial.RtsEnable = $false
$serial.ReadTimeout = 350
$capture = [IO.MemoryStream]::new()
$response = ''
try {
    $serial.Open()
    Start-Sleep -Milliseconds 600
    $serial.DiscardInBuffer()
    $started = Get-Date -Format o
    $serial.Write('t')
    $deadline = [DateTime]::UtcNow.AddSeconds(12)
    $buffer = [byte[]]::new(2048)
    while ([DateTime]::UtcNow -lt $deadline) {
        try {
            $count = $serial.Read($buffer, 0, $buffer.Length)
        } catch [TimeoutException] {
            continue
        }
        $capture.Write($buffer, 0, $count)
        $response = [Text.Encoding]::ASCII.GetString($capture.ToArray())
        if ($response -match '(?m)^# END\r?\n') {
            break
        }
    }
} finally {
    $serial.Dispose()
    if ($capture.Length -gt 0) {
        [IO.File]::WriteAllBytes($rawPath, $capture.ToArray())
    }
    $capture.Dispose()
}
$block = [regex]::Match($response, '(?ms)^sample,t_ms,raw,baseline_mv\r?\n.*?(?=^# END\r?$)')
if (!$block.Success) {
    throw "Incomplete serial response for $name"
}
$rows = @($block.Value | ConvertFrom-Csv)
if ($rows.Count -ne 150) {
    throw "Expected 150 samples, got $($rows.Count)"
}
for ($i = 0; $i -lt $rows.Count; $i++) {
    $row = $rows[$i]
    if ($row.sample -ne "$i" -or $row.raw -notmatch '^\d+$' -or [int]$row.raw -gt 4095 -or
        $row.baseline_mv -notmatch '^\d+$' -or $row.t_ms -notmatch '^\d+$') {
        throw "Invalid ADC row $i"
    }
}
[IO.File]::WriteAllText((Join-Path $outputPath "$name.csv"), $block.Value.Replace("`r`n", "`n"))
$codes = $rows | Measure-Object raw -Average -Minimum -Maximum -StandardDeviation
$voltage = $rows | Measure-Object baseline_mv -Average
[pscustomobject]@{
    trial = $Trial
    started = $started
    n = $rows.Count
    raw_mean = $codes.Average
    raw_sd = $codes.StandardDeviation
    raw_min = $codes.Minimum
    raw_max = $codes.Maximum
    adc_estimated_v = $voltage.Average / 1000
} | ConvertTo-Json | Tee-Object -FilePath (Join-Path $outputPath "$name.json")
