$LOCAL_DIR  = "D:\1x"
$REMOTE_DIR = "D:\2x"
$LOG_FILE   = "C:\logsync\robocopy.log"

# Tạo watcher giống inotify
$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = $LOCAL_DIR
$watcher.IncludeSubdirectories = $true
$watcher.EnableRaisingEvents = $true

$syncAction = {
    Start-Sleep -Milliseconds 500
    robocopy $using:LOCAL_DIR $using:REMOTE_DIR /MIR /Z /FFT /R:3 /W:2 /NP /LOG+:$using:LOG_FILE | Out-Null
}

# MỖI EVENT PHẢI CÓ ID RIÊNG
Register-ObjectEvent $watcher Changed -SourceIdentifier "FSChanged" -Action $syncAction
Register-ObjectEvent $watcher Created -SourceIdentifier "FSCreated" -Action $syncAction
Register-ObjectEvent $watcher Renamed -SourceIdentifier "FSRenamed" -Action $syncAction

while ($true) { Start-Sleep 60 }