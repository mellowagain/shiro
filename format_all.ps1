Get-ChildItem -Recurse .\src\ | ForEach-Object {
    if (($_.Extension -eq ".cc") -or ($_.Extension -eq ".hh")) {
        Write-Host $_.FullName
        if ($_.FullName -notmatch "thirdparty") {
            clang-format.exe -i -style=file $_.FullName
        }
    }
}
