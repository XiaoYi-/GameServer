import engin

def fun1(*t):
	str = engin.UnpacketStr()
	print "--------fun1----------",str
	engin.PreparePacket(10)
	engin.PacketInt(4)
	engin.SendPacket()
	print "-----fun1 end------"
	return 1001

def fun2(*t):
	iResult = 1002#engin.fact(10)
	print "-------fun2------"
	return iResult

gFun = {
	1:fun1,
	2:fun2,
}

def ExeFunc(*t):
	print "-------ExeFunc--------",t,t[0]
	iType = t[0]
	func = gFun.get(iType)
	if not func:
		return 10
	return func(t[0])

if __name__ == "__main__":
	sData = "{1:10}"
	dData = json.loads(sData)

	dData = {}
	dData["kk"] = 100



#import event