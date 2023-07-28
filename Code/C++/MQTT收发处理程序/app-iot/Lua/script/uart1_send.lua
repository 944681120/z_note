-- 用全局变量 实现 静态变量
-- stc_a = stc_a or 0
-- stc_a = stc_a + 1
-- if stc_a >= 100 then
--     stc_a = 0
-- end

-- 利用闭合函数 (closure) 实现 static variable --这种方式只在本次调用有效
local function myStaic(min, max)
    local i = min;
    return function()
        i = i + 1;
        if i > max then
            i = min
        end
        return i;
    end
end

printf= function(s, ...)
    return io.write(s:format(...))
end

local priformat = "%s 0x%x\n";
printf(priformat, "Hello World!", 50)

local test = myStaic(33, 35)
myFunction("1A2B3C4D " .. test())
-- sleep(1) --暂时还没实现延时函数，调用会使得一下的都不会执行
-- myFunction("1A2B3C4D " .. test())
-- sleep(1)
-- myFunction("1A2B3C4D " .. test())
-- sleep(1)
-- myFunction("1A2B3C4D " .. test())
-- sleep(1)

