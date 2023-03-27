-- Returns array with parsed contents of /proc/meminfo
-- Returns nil on failure
function get_meminfo()
	local r={}
	local f=io.open("/proc/meminfo","r")
	if not f then return nil; end
	local s=f:read("*a")
	for k,v in string.gmatch(s,"(%w+): *(%d+)") do
		r[k]=tonumber(v)
	end 
	f:close()
	return r
end

-- Returns 1 when RAM is bad, 0 when RAM is ok
-- Returns nil when memtest was not run
function memory_bad()
	local m=get_meminfo()
	if not m then return nil; end
	if not m.HardwareCorrupted then return nil; end
	if not m.EarlyMemtestBad   then return nil; end
	if m.HardwareCorrupted > 0 then return 1; end
	if m.EarlyMemtestBad   > 0 then return 1; end
	return 0;
end

m=get_meminfo()
print(m.MemTotal, m.MemFree, m.HardwareCorrupted, m.EarlyMemtestBad)
print(memory_bad());
