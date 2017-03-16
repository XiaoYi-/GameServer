import json

def fun():
	w = {1:10,2:20,"kk":{}}
	print w[1],w[2]
	sData = json.dumps(w)
	dData = json.loads(sData)
	print dData,dData[1],dData["kk"]

if __name__ == "__main__":
	fun()
