import libxml2
import os.path
from sys import argv

doc = libxml2.parseFile('EWARM/mksRobinNano.ewp')
ctxt = doc.xpathNewContext()

def fix(item):
	return item.content.replace("$PROJ_DIR$\\..\\", "").replace("$PROJ_DIR$/../", "").replace("\\","/")


if ((len(argv)==1) or (argv[1]=="src")):
	res = ctxt.xpathEval("//file[not(excluded[configuration='mksRobinNano'])]/name/text()")
	for e in res:
		e = fix(e)
		if os.path.isfile(e):
			print e
elif (argv[1]=="define"):
	res = ctxt.xpathEval("//settings/data/option[name='CCDefines']/state/text()")
	for e in res:
		print e
elif (argv[1]=="include"):
	res = ctxt.xpathEval("//settings/data/option[name='CCIncludePath2']/state/text()")
	for e in res:
		print fix(e)
elif (argv[1]=="lib"):
	res = ctxt.xpathEval("//settings/data/option[name='IlinkAdditionalLibs']/state/text()")
	for e in res:
		print fix(e)

