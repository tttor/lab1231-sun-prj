#Target class list file
classListPath=''
#Image Source per class dir
srcDir=''
#Target training folder dir
tgtDir=''
#Target test.txt dir:
splitDir=''

quotaPerClass=100
blockSize=10


with open(classListPath) as f:
    classList = f.read().splitlines()

classDirList = [ f for f in listdir(srcDir) ]

classDirList.sort()




srcImageList = [join(imgdir for imgdir in [ for in listdirjoin(srcDir,classDirList)]

for f in classDirList:
	srcImageList = [imgpath for imgpath in join(srcDir,f) ]
