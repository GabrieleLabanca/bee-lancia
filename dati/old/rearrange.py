import re

infile = open("DATA.TXT","r")
datastring = infile.read()
line = "" 
counter = 0
for c in datastring:

    #if(re.match(r'\.',c)):
    counter=counter+1
    line = line + c
    if (counter%17==0):
      print line
      line = "" 
    #  print '\n'

