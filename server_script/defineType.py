class C():
	def F(self):
		pass

def F():
	pass

c = C()
METHOD_TYPE = type(c.F)
FUNC_TYPE = type(F)