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

ig = [
"FF3700",
"FF3602",
"FF3505",
"FF3408",
"FF330B",
"FF320E",
"FF3111",
"FF3014",
"FF2F17",
"FF2E19",
"FF2D1C",
"FF2C1F",
"FF2B22",
"FF2A25",
"FF2928",
"FF292B",
"FF282E",
"FF2730",
"FF2633",
"FF2536",
"FF2439",
"FF233C",
"FF223F",
"FF2142",
"FF2045",
"FF1F48",
"FF1E4A",
"FF1D4D",
"FF1C50",
"FF1B53",
"FF1B56",
"FF1A59",
"FF195C",
"FF185F",
"FF1761",
"FF1664",
"FF1567",
"FF146A",
"FF136D",
"FF1270",
"FF1173",
"FF1076",
"FF0F79",
"FF0E7B",
"FF0D7E",
"FF0D81",
"FF0C84",
"FF0B87",
"FF0A8A",
"FF098D",
"FF0890",
"FF0792",
"FF0695",
"FF0598",
"FF049B",
"FF039E",
"FF02A1",
"FF01A4",
"FF00A7",
"FF00AA"]
'''
["D82E00",
"D52D00",
"D32C01",
"D12B02",
"CF2A03",
"CC2A04",
"CA2905",
"C82806",
"C62707",
"C32608",
"C12609",
"BF250A",
"BD240B",
"BA230C",
"B8230D",
"B6220E",
"B4210F",
"B12010",
"AF1F11",
"AD1F12",
"AB1E12",
"A91D13",
"A61C14",
"A41C15",
"A21B16",
"A01A17",
"9D1918",
"9B1819",
"99181A",
"97171B",
"94161C",
"92151D",
"90151E",
"8E141F",
"8B1320",
"891221",
"871122",
"851123",
"821024",
"800F25",
"7E0E25",
"7C0E26",
"7A0D27",
"770C28",
"750B29",
"730A2A",
"710A2B",
"6E092C",
"6C082D",
"6A072E",
"68072F",
"650630",
"630531",
"610432",
"5F0333",
"5C0334",
"5A0235",
"580136",
"560037",
"540038"]
'''
'''
["D82D00",
"D32600",
"CE1F00",
"CA1800",
"C51200",
"C10B00",
"BC0500",
"B80000",
"B30005",
"AF000A",
"AA000F",
"A50014",
"A10018",
"9C001C",
"980020",
"930023",
"8F0026",
"8A0029",
"86002C",
"81002F",
"7C0031",
"780033",
"730034",
"6F0035",
"6A0036",
"660037",
"610038",
"5D0038",
"580038",
"540038"]
'''
'''
["D82E00",
"D32C01",
"CE2A03",
"CA2905",
"C52707",
"C12609",
"BC240B",
"B8220D",
"B3210F",
"AF1F11",
"AA1E13",
"A51C15",
"A11A17",
"9C1919",
"98171B",
"93161C",
"8F141E",
"8A1320",
"861122",
"810F24",
"7C0E26",
"780C28",
"730B2A",
"6F092C",
"6A072E",
"660630",
"610432",
"5D0334",
"580136",
"540038"]
'''
# ig = ["9C1D85", "9F2081", "A2237E", "A5267A", "A82977", "AB2C73", "AE2F70", "B1326C", "B43569", "B73865", "BA3C62", "BD3F5F", "C0425B", "C34558", "C64854", "C94B51", "CC4E4D", "CF514A", "D25446", "D55743", "D85B40", "DB5E3C", "DE6139", "E16435", "E46732", "E76A2E", "EA6D2B", "ED7027", "F07324", "F37721"]

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



def generateInstagram(config, bright, wait):
	
	res = ""
	fade = True
	for h in reversed(ig):
		if fade:
			for x in range(2, 26, 2):
				b = f"{hex(x)[2:].upper()}0011"
				b = "1" + ("0" + b if len(b) == 5 else b) + " "
				print(b)
				res += b
				for l in range(1, numLights + 1):
					res += ("1" if l in config else "0") + h + " "
				res += "\n"
			fade = False
		else:
			res += "1" + bright + wait + " "
			for l in range(1, numLights + 1):
				res += ("1" if l in config else "0") + h + " "
			res += "\n"
	for h in ig:
		res += "1" + bright + wait + " "
		for l in range(1, numLights + 1):
			res += ("1" if l in config else "0") + h + " "
		res += "\n"
	
	for x in range(24, 1, -2):
		b = f"{hex(x)[2:].upper()}0011"
		b = "1" + ("0" + b if len(b) == 5 else b) + " "
		print(b)
		res += b
		for l in range(1, numLights + 1):
			res += ("1" if l in config else "0") + h + " "
		res += "\n"
	
	return res

if __name__ == '__main__':
	with open("script.txt", "w") as f:
		f.write(generateInstagram(possibleConfigs["G"], brightness[0], "000B"))
		'''
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
		'''