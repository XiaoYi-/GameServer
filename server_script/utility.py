import defineType
import json

def ParseFunc(oFunc):
	if type(oFunc) == defineType.METHOD_TYPE:
		return oFunc.im_func,oFunc.im_self
	elif type(oFunc) == defineType.FUNC_TYPE:
		return oFunc,None

def ParseFuncName(oFunc):
	c = C()
	if type(oFunc) == type(f):
		return oFunc,None
	elif type(oFunc) == type(c.f):
		return oFunc.im_func.func_name,oFunc.im_self

class Cfunctor(object):
	def __init__(self,oFunc,*tArgc):
		self.oFunc,self.wr = ParseFunc(oFunc)
		self.tArgc = tArgc

	def __call__(self,*tArgc):
		if self.wr:
			self.oFunc(self.wr,*self.tArgc)
		else:
			self.oFunc(*self.tArgc)


class CEvent(object):
	def __init__(self):
		self.lFunc = []

	def __call__(self,*tArgc):
		for t in self.lFunc:
			handle = t[0]
			wr = t[1]
			if wr:
				oFun = getattr(wr,handle)
				oFun(*tArgc)
			else:
				handle(*tArgc)

	def __sub__(self,fun):
		func,wr = ParseFuncName(fun)
		t = (func,wr)
		self.lFunc.remove(t)
		print "__sub__"
		return self
		

	def __iadd__(self,fun):
		func,wr = ParseFunc(fun)
		t = (func,wr)
		self.lFunc.append(t)
		print "__add__"
		return self

def F(iNum):
	print iNum 

class A(object):
	def __init__(self):
		self.iNum = 100
	def F(self,iNum):
		print self.iNum + iNum

if __name__ == "__main__":
	oFunc = Cfunctor(F,10)
	oFunc()
	a = A()
	oFunc1 = Cfunctor(a.F,10)
	oFunc1()

	str = json.dumps(['foo', {'bar': ('baz', None, 1.0, 2)}])
	l = json.loads(str)