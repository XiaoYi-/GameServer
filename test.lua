function functionobj(fun,arg)
	local ofun = {__mode="kv"}
	ofun.arg = arg
	ofun.fun = fun
	return ofun
end

function upvalue()
	local i = 0
	return function() i = i + 1;return i; end
end

event = {}

event.new = function(self)
	local div = {lfun = {__mode = "kv"}}
	setmetatable(div,self)
	self.__index = self
	return div
end

event.call = function(self)
	for k,ofun in ipairs(self.lfun) do 
		--ofun.fun(ofun.arg)
		ofun()
	end
end

event.add = function(self,fun)
	table.insert(self.lfun,fun)
end

A = {}
A.new = function(self)
	local  div = {x=0}
	setmetatable(div,self)
	self.__index = self
	return div
end
A.call = function(self)
	--local obj = self
	return function()
		--local obj = self
		print("a "..self.x)
	end
end

B = {}
B.new = function(self)
	local  div = {c=0}
	setmetatable(div,self)
	self.__index = self
	return div
end
B.call = function(self)
	--local obj = self
	return function()
		--local obj = self
		print("b "..self.c)
	end
end

a1 = A:new()
a2 = A:new()

b1 = B:new()
b2 = B:new()

cf = event:new()
-- cf:add(functionobj(a1.call,a1))
-- cf:add(functionobj(b1.call,b1))

-- cf:add(functionobj(a2.call,a2))
-- cf:add(functionobj(b2.call,b2))

cf:add(a1:call())
cf:add(a2:call())

cf:add(b1:call())
cf:add(b2:call())

local lfun = cf.lfun

b1.c = 100
b2.c = 200

a1.x = 1.0
a2.x = 2.1

--cf:call()

function pairs_(t)
	return next,t,nil
end

t = {1,2,3}

for k,element in pairs_(t) do
	--print(k,element)
end


-- for var_1, ..., var_n in explist do block end

local for_  

function for_( ... )
	local _f,_t,_c = ...
	while true do
		local _k,_val = _f(_t,_c)
		_c = _k
		if _c == nil then break end
		print(_k,_val)
	end
end

for k,v in next,t,nil do
	--print("next ",k,v)
end

--more status iter table as _c


--not status
function iter(t,i)
	local i = i + 1
	if i<= #t then return i,t[i] end
end

function ipairs_(t)
	return iter,t,0
end

for k,v in iter,t,0 do --pairs_(t)
	--print(k,v)
end

-- real iter fun as arg 
local function read_iter(t,f) 
	local n = #t
	for i=1,n do
		f(i,t[i])
	end 
end

--read_iter(t,function ( ... ) print(...) end)


w = {true,false}

for k,v in ipairs(w) do 
	--assert(v)
	--print(k,v)
end

list = nil 
list = { next_=list,value_ = 10 }

Queue = {}
Queue.new = function(self)
	local o = { head_=0,tail_=-1 }
	setmetatable(o,self)
	self.__index = self
	return o
end

Queue.pop = function (self)
	if(self.head_ > self.tail_) then return nil end
	self.head_ = self.head_ + 1
	local val = self[self.head_ - 1]
	self[self.head_] = nil
	return val
end

Queue.push = function(self,val)
	self.tail_ = self.tail_+1
	self[self.tail_] = val
end


function rl(t)
	local tb = {}
	--tb.__index = t
	local mt = { __newindex = function(t,k,v) print("read only") end , __index = t}
	setmetatable(tb,mt)
	return tb
end

-- rltb = rl({x=10})
-- rltb.x = 200
-- rltb.y = 300
-- print(rltb.x)



function ReadOnlyTable (t)
  local proxy = {}
  local mt = {
   __index = t,
   __newindex = function (t,k,v)
    error("attempt to update a read-only talbe",2)
   end
  }
  setmetatable(proxy,mt)
  return proxy
end

w = {x=10,t={y=100}}
w.new = function(self)
	local tb = {}
	setmetatable(tb,self)
	self.__index = self
	return tb
end

tb1 = w:new()
tb2 = w:new()

print(rawget(tb1,"t"))
tb1.t.y = 100
print(rawget(tb1,"t"))

print("tb2",rawget(tb2,"x"))
print(tb2.x)