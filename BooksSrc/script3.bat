REM for windows only. call it from cmd.exe
REM this script use recent version of 7zip
REM create .obk files from .txt
FOR /r %%F IN (*.txt) DO (
"C:\Program Files\7-Zip\7z.exe" a -tzip %%~pnF.obk %%F
"C:\Program Files\7-Zip\7z.exe" rn -tzip %%~pnF.obk %%~nxF BookText 
)
REM delete .txt files
DEL /S *.txt