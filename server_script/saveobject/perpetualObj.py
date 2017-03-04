
class PerpetualObj(object):
	def __init__(self):
		pass

	def Save(self):
		dData = {}
		dData["sp"] = super(PerpetualObj,self).Save()
		return dData

	def Load(self,dData):
		super(PerpetualObj,self).Load(dData.get("sp",{}))

	def OnLoaded(self):
		pass