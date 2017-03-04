

class sqlable(object):
	def __init__(self):
		self.SelectSql = self.UpdateSql = self.InsertSql = None

	def SaveToDb(self):
		dData = self.Save()
		self.UpdateSql.ExcuteUpdate(dData)

	def LoadFromDb(self):
		self.SelectSql.ExcuteSelect(self.SelectDataCb)

	def InsertToDb(self):
		self.InsertSql.ExcuteInsert()

	def SelectDataCb(self,dData):
		self.Load(dData)