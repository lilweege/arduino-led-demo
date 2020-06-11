# this script is janky af

from colorsys import hsv_to_rgb
from math import cos, radians
import re

numLights = 24

brightness = ["1A", "33", "80", "FF"]

# don't forget subtract 1
possibleConfigs = { "A" : [3, 9, 15, 21],  																			# 4: middles
					"B" : [6, 12, 18, 24], 																			# 4: corners
					"AB": [3, 6, 9, 12, 15, 18, 21, 24],															# 8: mid and corner
					"C" : [2, 4, 8, 10, 14, 16, 20, 22], 															# 8: two middle
					"D" : [2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24], 											# 12: evens
					"E" : [1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23], 												# 12: odds
					"F" : [1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23],					# 20: no corners
					"G" : [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24]	# 24: all
					}



def generateRainbowCycle(config, bright, wait):
	res = ""
	for c in range(360):
		res += "1" + bright + wait + " "
		for i in range(numLights):
			h = ((i * 360 / numLights + c) % 360) / 360
			t = hsv_to_rgb(h, 1, 1)
			m = map(lambda x: hex(int(x * 255))[2:], t)
			l = [("0" if len(str(e)) == 1 else "") + str(e) for e in m]
			s = re.sub("\(|\)|'| |,", "", str(tuple(l)))
			res += ("1" if i + 1 in config else "0") + s + " "
		res += "\n"
	return res


def generateSolidColors(config, bright, wait):
	res = ""
	for s in ("FF0000", "00FF00", "0000FF", "FFFFFF"):
		res += "1" + bright + wait + " "
		for i in range(numLights):
			res += ("1" if i + 1 in config else "0") + s + " "
		res += "\n"
	return res

def generateChasing(config, bright, wait):
	res = ""
	n = len(config)
	while len(res) < 40 * ((numLights + 1) * 8):
		for p in range(numLights):
			line = ""
			cnt = 0
			empty = True
			line += "1" + bright + wait + " "
			for i in range(numLights):
				if i + 1 in config:
					line += "1"
					if config[cnt] - 1 == p:
						line += "FFFFFF"
						empty = False
					elif config[cnt - 1] - 1 == p or config[(cnt + 1) % n] - 1 == p:
						line += "808080"
						empty = False
					else:
						line += "000000"
					cnt += 1
				else:
					line += "0000000"
				line += " "
			if not empty:
				res += line + "\n"
	return res

def generatePulsing(config, bright, wait):
	res = ""
	for a in range(360):
		res += "1" + bright + wait + " "
		b = (-cos(radians(a)) + 1) / 2
		e = hex(int(b * 255))[2:]
		s = (("0" if len(str(e)) == 1 else "") + str(e)) * 3
		for i in range(numLights):
			res += ("1" if i + 1 in config else "0") + s + " "
		res += "\n"
	return res

if __name__ == '__main__':
	with open("script.txt", "w") as f:
		for b in brightness:
			f.write(generateSolidColors(possibleConfigs["A"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["B"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["AB"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["C"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["D"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["E"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["F"], b, "01F4"))
			f.write(generateSolidColors(possibleConfigs["G"], b, "01F4"))
			
			f.write(generateRainbowCycle(possibleConfigs["A"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["B"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["AB"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["C"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["D"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["E"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["F"], b, "0005"))
			f.write(generateRainbowCycle(possibleConfigs["G"], b, "0005"))
			
			f.write(generateChasing(possibleConfigs["A"], b, "0019"))
			f.write(generateChasing(possibleConfigs["B"], b, "0019"))
			f.write(generateChasing(possibleConfigs["AB"], b, "0019"))
			f.write(generateChasing(possibleConfigs["C"], b, "0019"))
			f.write(generateChasing(possibleConfigs["D"], b, "0019"))
			f.write(generateChasing(possibleConfigs["E"], b, "0019"))
			f.write(generateChasing(possibleConfigs["F"], b, "0019"))
			f.write(generateChasing(possibleConfigs["G"], b, "0019"))
			
			f.write(generatePulsing(possibleConfigs["A"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["B"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["AB"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["C"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["D"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["E"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["F"], b, "0005"))
			f.write(generatePulsing(possibleConfigs["G"], b, "0005"))
			