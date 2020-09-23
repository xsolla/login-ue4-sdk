SET source=..\..\..\..\..\Plugins\store-ue4-sdk
SET target=..\..\..\..\..\..\ue4-store-sdk

rmdir %target% /S /Q
mkdir %target%

:: Folders
for %%a in ("Source" "Content" "Config" "Documentation" "Extras" "Resources") do (xcopy /I /S "%source%\%%~a" "%target%\%%~a")

:: Files
xcopy "%source%\*.uplugin" "%target%"

:: Remove useless map
del %target%\Content\Maps\Demo_Login.umap

@PAUSE