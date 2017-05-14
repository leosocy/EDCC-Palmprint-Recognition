#!/usr/bin/python
# -*- coding: UTF-8 -*-

def getTuningResult( oldTuningResultFileName ):
	newResult = []
	oneRow = []
	count = 0
	with open( oldTuningResultFileName, 'r' ) as oldFile:
		while True:
			line = oldFile.readline()
			if not line:
				break
			count += 1
			info = line.split( "\t" )
			if count % 4 == 1:
				oneRow.append( info[0].split(":")[1] )
				oneRow.append( info[1].split(":")[1] )
			elif count % 4 == 2:
				oneRow.append( (info[0].split(":(")[1]).split(",")[0] )
				oneRow.append( info[1].split(":")[1] )
				oneRow.append( info[2].split(":")[1] )
				oneRow.append( (info[3].split(":(")[1]).split(",")[0] )
			elif count % 4 == 3:
				oneRow.append( info[0].split(":")[1] )
				oneRow.append( info[1].split(":")[1] )
				oneRow.append( info[2].split(":")[1] )
				oneRow.append( info[3].split(":")[1] )
				oneRow.append( info[4].split(":")[1] )
				oneRow.append( info[5].split(":")[1] )
				oneRow.append( info[6].split(":")[1] )
				oneRow.append( info[7].split(":")[1] )
			else:
				newResult.append( oneRow )
				oneRow = []
	newFile = open( "./feature_info/NewTuning.txt", "w" )
	for row in newResult:
		newFile.write( "%d %d %d %d " % ( int(row[2]), int(row[3]), int(row[4]), int(row[5]) ) )
 		newFile.write( "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n" % ( float(row[0]), float(row[1]), float(row[6]), float(row[7]), float(row[8]), float(row[9]), float(row[10]), float(row[11]), float(row[12]), ( float(row[9]) - float(row[10]) ) / ( float(row[11]) + float(row[12]) ) ) )
	newFile.close()

if __name__ == "__main__":
	getTuningResult( "./feature_info/TuningResult.txt" )

