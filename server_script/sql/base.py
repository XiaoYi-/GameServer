class Base(object):
	
	def __init__(self):
		self.dData = {}
	
	def Set(self,key,value):
		self.dData[key] = value
	
	def Query(self,key):
		return self.dData.get(key,0)

	def Save(self):
		dData = {}
		dData["sp"] = super(Base,self).Save()
		dData["data"] = self.dData
		return dData

	def Load(self,dData):
		super(Base,self).Load(dData["sp"])
		self.dData = dData["dData"]
		self.kk = dData["kk"]

class Div(Base):
	def __init__(self):
		super(Div,self).__init__()
		self.id = 100

	def Save(self):
		dData = {}
		dData["sp"] = super(Div,self).Save()
		dData["dData"] = self.dData
		dData["id"] = self.id
		return dData
	def Load(self,dData):
		super(Div,self).Load(dData["sp"])
		self.dData = dData["dData"]
		self.id = dData["id"]

if __name__ == "__main__":
	div = Div()
	div.Set("kk",10)
	dData = div.dData
	div.newkk = 200
	print dData