# Define the directory containing the library files
$directoryPath = "C:\local\bin\bullet3-3.25\build\lib\Debug"

# Get all files in the directory with the suffix '_Debug.lib'
$libFiles = Get-ChildItem -Path $directoryPath -Filter '*_Debug.lib'

# Loop through each file and rename it
foreach ($file in $libFiles) {
    # Generate the new name by replacing '_Debug.lib' with '.lib'
    $newName = $file.Name -replace '_Debug.lib$', '.lib'
    
    # Get the full path of the new name
    $newPath = Join-Path -Path $directoryPath -ChildPath $newName
    
    # Rename the file
    Rename-Item -Path $file.FullName -NewName $newPath
    
    # Output the old and new names for verification
    Write-Host "Renamed $($file.Name) to $newName"
}
