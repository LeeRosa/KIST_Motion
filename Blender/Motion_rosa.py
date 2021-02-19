import bge
import math
from math import *
import mathutils
import time

import sys
#sys.path.append("/usr/lib/python3/dist-packages")
import serial
import glob

#port=''.join(glob.glob("/dev/ttyACM*"))
#port=''.join(glob.glob("/dev/ttyUSB*"))
#port=''.join(glob.glob("/dev/rfcomm"))  
ser = serial.Serial("COM8",115200)
print("connected to: " + ser.portstr)

#Connect the suit first and after a ~second launch the script


# Get the whole bge scene
scene = bge.logic.getCurrentScene()
# Helper vars for convenience
source = scene.objects
# Get the whole Armature
main_arm = source.get('Game_engine')
ob = bge.logic.getCurrentController().owner


def updateAngles():
	ser.write('a'.encode('UTF-8'))
	s=ser.readline()[:-3].decode('UTF-8') #delete ";\r\n"
	angles = [x.split(',') for x in s.split(';')]
	for i in range(len(angles)):
		angles[i] = [float(x) for x in angles[i]]
    
	upperarm_r = mathutils.Quaternion(angles[0])
	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
	upperarm_r_out = correction*upperarm_r

	upperarm_l = mathutils.Quaternion(angles[1])
	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
	upperarm_l_out = correction*upperarm_l

	neck_01 = mathutils.Quaternion(angles[2])
	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
	neck_01_out = correction*neck_01

#	thigh_r = mathutils.Quaternion(angles[0])
#	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
#	thigh_r_out = correction*thigh_r

#	thigh_l = mathutils.Quaternion(angles[1])
#	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
#	thigh_l_out = correction*thigh_l

#	spine_01 = mathutils.Quaternion(angles[2])
#	correction = mathutils.Quaternion((1.0, 0.0, 0.0), math.radians(90.0))
#	spine_01_out = correction*spine_01

	ob.channels['upperarm_r'].rotation_quaternion = upperarm_r_out
	ob.channels['upperarm_l'].rotation_quaternion = upperarm_l_out
	ob.channels['neck_01'].rotation_quaternion = neck_01_out
#	ob.channels['thigh_r'].rotation_quaternion = thigh_r_out
#	ob.channels['thigh_l'].rotation_quaternion = thigh_l_out
#	ob.channels['spine_01'].rotation_quaternion = spine_01_out
	ob.update()
	#time.sleep(0.001)
