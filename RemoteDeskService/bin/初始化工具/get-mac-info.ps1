$items = Get-WmiObject -class Win32_NetworkAdapter  -namespace "root\cimv2" | where {$_.MACAddress -ne $null}
$mac = $null;
Write-Host INDEX"   "ADDRESS"                "NAME;
Write-Host ------- ---------------------- --------------------------;
foreach($item in $items)
{
	Write-Host $item.Index"    "$item.MACAddress"    "$item.Name;
}
Write-Host   
Write-Host 完成
Write-Host   
Write-Host   