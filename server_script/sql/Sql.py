import json

class Sql(object):
	self.sSql = ""
	self.tParsType=""
	self.tPrimaryKey = 0

	def ExcuteSelect(self,cb,*tArgc):
		global gSelectCb
		global iSelecCbIndex

		if gSelectCb.get(iSelecCbIndex+1):
			print "--error--"
			return 
		iSelecCbIndex = iSelecCbIndex + 1
		iSelecCbIndex = iSelecCbIndex % 10000000
		gSelectCb[iSelecCbIndex] = cb

		#sendPacket to db server

	def ExcuteUpdate(self,dData):
		sData = json.dumps(dData)

	def ExcuteInsert(self):
		pass


def ExcuteCb():
	global gSelectCb
	sData = "{1:10}"
	dData = json.loads(sData)
	iIndex = 1
	cb = gSelectCb.get(iIndex)
	cb(dData)


gSelectCb = {}
iSelecCbIndex = 0
