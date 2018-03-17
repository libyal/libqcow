# Script that synchronizes dokan
#
# Version: 20180317

Param (
	[switch]$UseHead = $false
)

$Git = "git"

# Patched version of dokan 0.6.0
$GitUrl = "https://github.com/joachimmetz/dokan.git"
$Destination = "..\dokan"

# PowerShell will raise NativeCommandError if git writes to stdout or stderr
# therefore 2>&1 is added and the output is stored in a variable.
$Output = Invoke-Expression -Command "${Git} clone ${GitUrl} ${Destination} 2>&1"

Push-Location ${Destination}

Try
{
	$Output = Invoke-Expression -Command "${Git} fetch --quiet --all --tags --prune 2>&1"

	$LatestTag = Invoke-Expression -Command "${Git} describe --tags --abbrev=0 2>&1"

	If (${LatestTag} -and -not ${UseHead})
	{
		Write-Host "Synchronizing: dokan from ${GitUrl} tag ${LatestTag}"

		$Output = Invoke-Expression -Command "${Git} checkout --quiet tags/${LatestTag} 2>&1"
	}
	Else
	{
		Write-Host "Synchronizing: dokan from ${GitUrl} HEAD"
	}
	$Configuration = ${Env:Configuration}

	If (-not ${Env:Configuration})
	{
		$Configuration = "Release"
	}
	$Platform = ${Env:Platform}

	If (-not ${Env:Platform})
	{
		$Platform = "Win32"
	}
	$MSBuildOptions = "/target:Build /property:Configuration=${Configuration},Platform=${Platform}"

	Invoke-Expression -Command "C:\\Windows\Microsoft.NET\Framework\v3.5\MSBuild.exe ${MSBuildOptions} msvscpp\dokan\dokan.vcproj"
}
Finally
{
	Pop-Location
}

