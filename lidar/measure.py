import pygame # pygame 라이브러리 사용
import Serial
import random

pygame.init() # pygame 초기화
pygame.font.init()

infostuffs = pygame.display.Info()
monitorx, monitory = infostuffs.current_w, infostuffs.current_h
board = pygame.display.set_mode((monitorx, monitory)) # 크기를 기반으로 게임판 설정
pygame.display.set_caption("measure program") # 게임 이름 설정
ser = serial.Serial('/dev/ttyACM0', 9600)
musics = ['siren.mp3']
sel = 0

done = True # 무한 반복을 위한 done 변수를 True로 설정
while done : # done이 True일 경우 (무한 반복)
    for event in pygame.event.get() : # 이벤트 인식
        if event.type == pygame.QUIT : # 종료 버튼을 누른 경우
            done = False # done을 False로 설정
        elif event.type == pygame.KEYDOWN :
            if event.key == pygame.K_e:
                done = False

    try:
        n = int(ser.readline())
    except:
        n = 0
    font = pygame.font.SysFont('elephant', 500)
    prt = str(n) + " / 18"
    if n <= 10:
        col = (0, 0, 255)
    elif 10 < n < 18:
        col = (255, 150, 0)
    else:
        col = (255, 0, 0)
        if n>18:
            if pygame.mixer.music.get_busy():
                pass
            else:
                pygame.mixer.music.load(musics[0])
                pygame.mixer.music.play()
    if n<=18 and pygame.mixer.music.get_busy():
        pygame.mixer.music.stop

    text = font.render(prt, True, col)
    textr = text.get_rect()
    textr.center = (monitorx/2, monitory/2)
   

    board.fill((255,255,255)) # 배경을 흰색으로 채운다
    board.blit(text, textr)

    pygame.display.flip() # 게임판을 그린다
    pygame.time.delay(10) # 딜레이 설정 (0.01초)

exit()
