import yaml
config = yaml.safe_load(open("config.yml"))

import pygame
import sys
import math
import time

import screeninfo
W = screeninfo.get_monitors()[0].width
H = screeninfo.get_monitors()[0].height
DEFAULT_IMAGE_SIZE = (W, H)

rW = W/1920
rH = H/1080

pygame.init()


# DEBUG

pygame.mouse.set_visible(config['debug']['cursore_mouse'])
screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
# screen = pygame.display.set_mode((640, 480))
color = (255,0,0)
TOUCH_ZONE = config['debug']['zone_touch']

# MENU

menu = pygame.image.load("img/menu.png")
menu = pygame.transform.scale(menu, DEFAULT_IMAGE_SIZE)

menuSelected = []
for i in range(4):
	spath = "img/menu" + str(i+1) + ".png"
	stemp = pygame.image.load(spath)
	stemp = pygame.transform.scale(stemp, (rW*435, rH*685))
	menuSelected.append(stemp)


wMissioni = 350*rW
hMissioni = 600*rH
posMissioni = [(195,280),(587,280),(979,280),(1373,280)]
for i in range(4):
	xT, yT = posMissioni[i]
	posMissioni[i] = (xT*rW, yT*rH)

# INTRO

end_intro = [".jpg",".jpg",".png",".png"]
intro = []
for i in range(4):
	spath = "img/mission" + str(i+1) + end_intro[i]
	stemp = pygame.image.load(spath)
	stemp = pygame.transform.scale(stemp, DEFAULT_IMAGE_SIZE)
	intro.append(stemp)

wBack = 290*rW
hBack = 100*rH
posBack = (260*rW, 790*rH)

wIntro = 850*rW
hIntro = 100*rH
posIntro = (830*rW, 790*rH)

# STRUMENTI

bg = pygame.image.load("img/tools.png")
bg = pygame.transform.scale(bg, DEFAULT_IMAGE_SIZE)

avvio = pygame.image.load("img/avvio.png")
avvio = pygame.transform.scale(avvio, (rW*555, rH*125))

s = []
for i in range(1,16):
	spath = "img/s" + str(i) + ".png"
	stemp = pygame.image.load(spath)
	stemp = pygame.transform.scale(stemp, (357*rW,371*rH))
	s.append(stemp)

wPR = 190*rW
hPR = 190*rH

wD = 80*rW
hD = 90*rH

# pos = []
# for i in range(15):
# 	xi = i%5
# 	yi = int(math.floor(i/5))

# 	xP = round(281 + xi*(W/7.67))
# 	yP = round(127 + yi*(H/4.32))

# 	pos.append((round(xP),round(yP)))

pos = [	(281,127),(531,127),(781,127),(1032,127),(1282,127),
		(281,375),(531,375),(781,375),(1032,375),(1282,375),
		(281,624),(531,624),(781,624),(1032,624),(1282,624) ]

for i in range(15):
	xT, yT = pos[i]
	pos[i] = (xT*rW, yT*rH)

wCheck = 500*rW
hCheck = 90*rH
posCheck = (700*rW, 940*rH)

wBack2 = 100*rW
hBack2 = 100*rH

# WIN / LOOSE

win = pygame.image.load("img/win.jpg")
win = pygame.transform.scale(win, DEFAULT_IMAGE_SIZE)

error = pygame.image.load("img/error.jpg")
error = pygame.transform.scale(error, DEFAULT_IMAGE_SIZE)

# VIDEO

import cv2

video1_discesa = cv2.VideoCapture("mov/video1_discesa.mp4")
video2_discesa = cv2.VideoCapture("mov/video2_discesa.mp4")
video3_discesa = cv2.VideoCapture("mov/video3_discesa.mp4")
video4_discesa = cv2.VideoCapture("mov/video4_discesa.mp4")

video1_salita = cv2.VideoCapture("mov/video1_salita.mp4")
video2_salita = cv2.VideoCapture("mov/video2_salita.mp4")
video3_salita = cv2.VideoCapture("mov/video3_salita.mp4")

video_discesa = [video1_discesa, video2_discesa, video3_discesa, video4_discesa]
video_salita = [video1_salita, video2_salita, video3_salita]


clock = pygame.time.Clock()
PAUSE = False

# FINALE

background = pygame.image.load("img/background.jpg")
background = pygame.transform.scale(background, DEFAULT_IMAGE_SIZE)

obiettivi = []
for i in range(4):
	st = "img/obiettivi_" + str(i+1) + ".png"
	ot = pygame.image.load(st)
	ot = pygame.transform.scale(ot, DEFAULT_IMAGE_SIZE)
	obiettivi.append(ot)

finali = []
finali_moveX = []
for i in range(4):
	st = "img/finale" + str(i+1) + ".jpg"
	ot = pygame.image.load(st)
	if i == 0:
		ot = pygame.transform.rotate(ot, 90)
	sx, sy = ot.get_rect().size
	ot = pygame.transform.scale(ot, (H/sy*sx, H))
	finali_moveX.append((W - H/sy*sx)/2)
	finali.append(ot)

finale4_sottomarina = pygame.image.load("img/finale4_sottomarina.png")
sx, sy = finale4_sottomarina.get_rect().size
finale4_sottomarina = pygame.transform.scale(finale4_sottomarina, (H/sy*sx, H))
finale4_sottomarina_moveX = (W - H/sy*sx)/2

import serial

try:
    ser = serial.Serial('/dev/ttyACM0',9600)
except:
    SERIAL_EN = False
else:
	SERIAL_EN = True

startMotor = 0

# GRAFICI
grafici = [[],[],[]]
for i in range(3):
	sa = "img/g" + str(i+1) + "a.jpg"
	sb = "img/g" + str(i+1) + "b.jpg"
	sc = "img/g" + str(i+1) + "c.jpg"

	at = pygame.image.load(sa)
	bt = pygame.image.load(sb)
	ct = pygame.image.load(sc)

	at = pygame.transform.scale(at, DEFAULT_IMAGE_SIZE)
	bt = pygame.transform.scale(bt, DEFAULT_IMAGE_SIZE)
	ct = pygame.transform.scale(ct, DEFAULT_IMAGE_SIZE)

	grafici[i].append(at)
	grafici[i].append(bt)
	grafici[i].append(ct)


# SUONI
from ffpyplayer.player import MediaPlayer

pygame.mixer.init()
blip_sound = pygame.mixer.Sound("snd/blip.wav")
blop_sound = pygame.mixer.Sound("snd/blop.wav")

audio1_discesa = ("mov/video1_discesa.mp4")
audio2_discesa = ("mov/video2_discesa.mp4")
audio3_discesa = ("mov/video3_discesa.mp4")
audio4_discesa = ("mov/video4_discesa.mp4")

audio1_salita = ("mov/video1_salita.mp4")
audio2_salita = ("mov/video2_salita.mp4")
audio3_salita = ("mov/video3_salita.mp4")

audio_discesa = [audio1_discesa, audio2_discesa, audio3_discesa, audio4_discesa]
audio_salita = [audio1_salita, audio2_salita, audio3_salita]

# GLOBAL VARIABLE
tempoReset = 0
resetMotor = False

sFlag = [False for i in range(15)]
change = False

mX = 0
mY = 0

statoGioco = 0
statoMotore = 0
update = True
fps = 30

dtPausa = 0

tools_missioni = [(5,10,4,7,2,8,6),(5,10,4,1,6,8,9),(5,10,4,1,3,9,14,8),(15,10,4,3,9,1,8,11,12,13,14)]

def controllo():

	for i in range(15):
		if sFlag[i]:
			if (i+1) not in tools_missioni[missione]:
				return False
		else:
			if (i+1) in tools_missioni[missione]:
				return False

	return True

while True:

	# INITIAL TIMER
	if (pygame.time.get_ticks() - tempoReset) > config['slide']['pausa_finale4']*1000 and resetMotor:
		resetMotor = False
		if SERIAL_EN:
			ser.write("of\r".encode())
			ser.flush()

	# DURANTE
	if not update:
		if statoGioco == 3: # VIDEO DISCESA

			if not PAUSE:

				audio.set_pause(False)

				success, video_image = video_discesa[missione].read()
				if success:
					video_surf = pygame.image.frombuffer(
						video_image.tobytes(), video_image.shape[1::-1], "BGR")
					vid = pygame.transform.scale(video_surf, NEW_SIZE)

					if MOVE_VIDEO > 0:
						screen.blit(vid, (MOVE_VIDEO, 0))
					else:
						screen.blit(vid, (0, 0))

					adesso = time.time()

					dtInizioReale = (adesso - START_TIME + dtPausa) #s
					dtInizioCorretto = video_discesa[missione].get(cv2.CAP_PROP_POS_MSEC)/1000 #s
					DT = dtInizioCorretto - dtInizioReale
					if DT > 0:
						time.sleep(DT 	)
						pygame.display.update()
						start_time = time.time()

				else:
					if SERIAL_EN:
						ser.write("of\r".encode())
						ser.flush()
					statoGioco = 4
					update = True

		elif statoGioco == 4: # OBIETTIVI
			if (pygame.time.get_ticks() - tempoObiettivi) > config['slide']['pausa_obiettivi']*1000:
				statoGioco = 5
				update = True

		elif statoGioco == 5: # RISALITA
			if missione == 3:
				if (pygame.time.get_ticks() - tempoFinale4) > config['slide']['pausa_finale4']*1000:
					statoGioco = 0
					update = True
			else:
				if not PAUSE:

					audio.set_pause(False)

					success, video_image = video_salita[missione].read()

					if success:
						video_surf = pygame.image.frombuffer(
							video_image.tobytes(), video_image.shape[1::-1], "BGR")

						vid = pygame.transform.scale(video_surf, NEW_SIZE)

						if MOVE_VIDEO > 0:
							screen.blit(vid, (MOVE_VIDEO, 0))
						else:
							screen.blit(vid, (0, 0))

						adesso = time.time()

						dtInizioReale = (adesso - START_TIME + dtPausa) #s
						dtInizioCorretto = video_salita[missione].get(cv2.CAP_PROP_POS_MSEC)/1000 #s

						DT = dtInizioCorretto - dtInizioReale
						if DT > 0:
							time.sleep(DT)
							pygame.display.update()
							start_time = time.time()
					else:
						if SERIAL_EN:
							ser.write("of\r".encode())
							ser.flush()
						statoGioco = 70
						update = True

		elif statoGioco == 6: # CHECK FINE FOTO FINALI
			if (pygame.time.get_ticks() - tempoFinale) > config['slide']['pausa_foto']*1000:
				# MANDA SEGNALE GIU
				resetMotor = False
				if SERIAL_EN:
					ser.write("lw\r".encode())
					ser.flush()

				# MANDA VIDEO
				success, video_image = video_discesa[missione].read()
				fps = video_discesa[missione].get(cv2.CAP_PROP_FPS)
				wVideo = video_discesa[missione].get(3)  # float `width`
				hVideo = video_discesa[missione].get(4)  # float `height`

				if wVideo < W:
					NEW_SIZE = (H/hVideo*wVideo,H)
					screen.fill((0,0,0))
					pygame.display.update()
					MOVE_VIDEO = (W - H/hVideo*wVideo)/2
				else:
					NEW_SIZE = DEFAULT_IMAGE_SIZE
					MOVE_VIDEO = 0

				audio = MediaPlayer(audio_discesa[missione])
				audio.set_pause(True)
				start_time = time.time()
				START_TIME = start_time
				dtPausa = 0
				statoGioco = 3

	# PRIMA VOLTA
	if update:
		update = False

		if statoGioco == 0:

			if SERIAL_EN:
				ser.write("up\r".encode())
				ser.flush()
				tempoReset = pygame.time.get_ticks()
				resetMotor = True

			sFlag = [False for i in range(15)]
			PAUSE = False

			video1_discesa.set(cv2.CAP_PROP_POS_FRAMES, 0)
			video2_discesa.set(cv2.CAP_PROP_POS_FRAMES, 0)
			video3_discesa.set(cv2.CAP_PROP_POS_FRAMES, 0)
			video4_discesa.set(cv2.CAP_PROP_POS_FRAMES, 0)

			video1_salita.set(cv2.CAP_PROP_POS_FRAMES, 0)
			video2_salita.set(cv2.CAP_PROP_POS_FRAMES, 0)
			video3_salita.set(cv2.CAP_PROP_POS_FRAMES, 0)

			statoMotore = 0

			screen.blit(menu, (0,0))

			if TOUCH_ZONE:
				for i in range(4):
					pygame.draw.rect(screen, color, pygame.Rect(posMissioni[i][0], posMissioni[i][1], wMissioni, hMissioni), 1)

			pygame.display.update()

		elif statoGioco == 1:

			screen.blit(intro[missione], (0,0))
			if TOUCH_ZONE:
				pygame.draw.rect(screen, color, pygame.Rect(posIntro[0], posIntro[1], wIntro, hIntro), 1)
				pygame.draw.rect(screen, color, pygame.Rect(posBack[0], posBack[1], wBack, hBack), 1)
			
			pygame.display.update()

		elif statoGioco == 2:

			screen.blit(bg, (0,0))
			for i in range(15):
				if sFlag[i]:
					screen.blit(s[i], pos[i])

			if TOUCH_ZONE:
				for i in range(15):
					pygame.draw.rect(screen, color, pygame.Rect(pos[i][0]+wD, pos[i][1]+hD, wPR, hPR), 1)

				pygame.draw.rect(screen, color, pygame.Rect(posCheck[0], posCheck[1], wCheck, hCheck), 1)
			
			pygame.display.update()

		elif statoGioco == 4:
			screen.blit(obiettivi[missione], (0,0))
			pygame.display.update()
			tempoObiettivi = pygame.time.get_ticks()

		elif statoGioco == 5:
			
			if missione == 3:
				screen.fill((0,0,0))
				screen.blit(finale4_sottomarina , (finale4_sottomarina_moveX,0))
				pygame.display.update()
				tempoFinale4 = pygame.time.get_ticks()

			else:
				# MANDA SEGNALE SU
				if SERIAL_EN:
					ser.write("up\r".encode())
					ser.flush()
				# MANDA VIDEO
				success, video_image = video_salita[missione].read()
				fps = video_salita[missione].get(cv2.CAP_PROP_FPS)

				wVideo = video_salita[missione].get(3)  # float `width`
				hVideo = video_salita[missione].get(4)  # float `height`

				if wVideo < W:
					NEW_SIZE = (H/hVideo*wVideo,H)
					screen.fill((0,0,0))
					pygame.display.update()
					MOVE_VIDEO = (W - H/hVideo*wVideo)/2
				else:
					NEW_SIZE = DEFAULT_IMAGE_SIZE
					MOVE_VIDEO = 0

				audio = MediaPlayer(audio_salita[missione])
				audio.set_pause(True)
				start_time = time.time()
				START_TIME = start_time
				dtPausa = 0

		elif statoGioco == 6:
			screen.fill((0,0,0))
			screen.blit(finali[missione], (finali_moveX[missione],0))
			pygame.display.update()
			tempoFinale = pygame.time.get_ticks()

		elif statoGioco == 100:

			screen.blit(win, (0,0))
			pygame.display.update()
			time.sleep(config['slide']['pausa_win'])
			statoGioco = 6
			update = True

		elif statoGioco == 666:

			screen.blit(error, (0,0))
			pygame.display.update()
			time.sleep(config['slide']['pausa_errore'])
			statoGioco = 2
			update = True

		elif statoGioco == 70:

			screen.blit(grafici[missione][0], (0,0))
			pygame.display.update()

		elif statoGioco == 71:

			screen.blit(grafici[missione][1], (0,0))
			pygame.display.update()

		elif statoGioco == 72:

			screen.blit(grafici[missione][2], (0,0))
			pygame.display.update()


	# MOUSE CLICK
	for event in pygame.event.get():

		if event.type == pygame.QUIT:
			# Quit the game
			pygame.quit()
			if SERIAL_EN:
				ser.write("of\r".encode())
				ser.flush()
			sys.exit()

		if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
			# Quit the game
			pygame.quit()
			if SERIAL_EN:
				ser.write("of\r".encode())
				ser.flush()
			sys.exit()

		if event.type == pygame.MOUSEBUTTONDOWN:


			mX, mY = pygame.mouse.get_pos()

			if statoGioco == 0:
				for i in range(4):
					xP, yP = posMissioni[i]
					w = wMissioni
					h = hMissioni
					if mX > xP and mX < xP+wMissioni:
						if mY > yP and mY < yP+hMissioni:
							pygame.mixer.Sound.play(blip_sound)
							screen.blit(menuSelected[i], (xP-41*rW,yP-37*rH))
							pygame.display.update()
							time.sleep(config['slide']['tempo_pulsanti_arancioni'])
							missione = i
							statoGioco = 1
							update = True

			elif statoGioco == 1:
				xP, yP = posIntro
				w = wIntro
				h = hIntro
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						pygame.mixer.Sound.play(blip_sound)
						statoGioco = 2
						update = True

				xP, yP = posBack
				w = wBack
				h = hBack
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						pygame.mixer.Sound.play(blip_sound)
						statoGioco = 0
						update = True

			elif statoGioco == 2:

				xP, yP = (0, 0)
				w = wBack2
				h = hBack2
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						statoGioco = 0
						update = True
						break

				for i in range(15):

					xPR, yPR = pos[i]

					xPR = xPR + wD
					yPR = yPR + hD

					if mX > xPR and mX < xPR+wPR:
						if mY > yPR and mY < yPR+hPR:
							pygame.mixer.Sound.play(blip_sound)
							sFlag[i] = not sFlag[i]
							change = True

				xP, yP = posCheck
				w = wCheck
				h = hCheck
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:

						
						screen.blit(avvio, (xP-19*rW,yP-15*rH))
						
						if controllo():
							pygame.mixer.Sound.play(blip_sound)
							statoGioco = 100
							update = True
						else:
							pygame.mixer.Sound.play(blop_sound)
							statoGioco = 666
							update = True

						pygame.display.update()
						time.sleep(config['slide']['tempo_pulsanti_arancioni'])

				if change:
					change = False

					screen.blit(bg, (0,0))

					for i in range(15):

						if sFlag[i]:
							screen.blit(s[i], pos[i])

					pygame.display.update()

			elif statoGioco == 3:
				xP, yP = (0, 0)
				w = wBack2
				h = hBack2
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						statoGioco = 0
						update = True
						break
				if config['video']['click_pausa']:
					PAUSE = not PAUSE

					if PAUSE:
						audio.set_pause(True)
						dTemp = time.time()
					else:
						dtPausa = dtPausa + (dTemp - time.time())

					if PAUSE:
						if SERIAL_EN:
							ser.write("of\r".encode())
							ser.flush()
					else:
						if SERIAL_EN:
							ser.write("lw\r".encode())
							ser.flush()

			elif statoGioco == 6:
				xP, yP = (0, 0)
				w = wBack2
				h = hBack2
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						statoGioco = 0
						update = True
						break

			elif statoGioco == 5:
				xP, yP = (0, 0)
				w = wBack2
				h = hBack2
				if mX > xP and mX < xP+w:
					if mY > yP and mY < yP+h:
						statoGioco = 0
						update = True
						break

				if config['video']['click_pausa'] and missione < 3:
					PAUSE = not PAUSE

					if PAUSE:
						audio.set_pause(True)
						dTemp = time.time()
					else:
						dtPausa = dtPausa + (dTemp - time.time())

					if PAUSE:
						if SERIAL_EN:
							ser.write("of\r".encode())
							ser.flush()
					else:
						if SERIAL_EN:
							ser.write("up\r".encode())
							ser.flush()

			elif statoGioco >= 70 and statoGioco < 80:

				xP = W/2

				if mX > xP+100:
					statoGioco = statoGioco + 1
					if statoGioco > 72:
						statoGioco = 0
					update = True
					break

				elif mX < xP-100:
					statoGioco = statoGioco - 1
					if statoGioco < 70:
						statoGioco = 70
					update = True
					break

