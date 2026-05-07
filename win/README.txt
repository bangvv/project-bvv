sc create LogSyncService binPath= "powershell.exe -WindowStyle Hidden -ExecutionPolicy Bypass -File D:\project-bvv\win\logsync.ps1" start= auto

sc start LogSyncService

sc stop LogSyncService

sc stop LogSyncService
sc delete LogSyncService