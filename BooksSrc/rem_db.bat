FOR /r %%F IN (*.obk) DO (
"C:\Program Files\7-Zip\7z.exe" d -tzip %%F *DB
)