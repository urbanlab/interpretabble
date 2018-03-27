import sys
import pygame
import os
from pygame.locals import *



if __name__ == '__main__':
    pygame.init()
    screen = pygame.display.set_mode((800, 600), 0, 32)
    screen.fill((255, 255, 255))
    clock = pygame.time.Clock()
    z = 0
    i = 0
    brush = pygame.image.load("brush.png")
    brush = pygame.transform.scale(brush, (32, 32))
    pygame.display.update()

    while 1:
        clock.tick(60)
        x, y = pygame.mouse.get_pos()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            elif event.type == MOUSEBUTTONDOWN:
                z = 1
            elif event.type == MOUSEBUTTONUP:
                z = 0
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_w:
                    screen.fill((255, 255, 255))
                    pygame.display.update()
                elif event.key == pygame.K_s:
                    pygame.image.save(screen, "data{0}.jpg".format(i))
                    os.system("python3 resize.py data{0}.jpg".format(i))
                    os.system("python3 main.py some0.pgm")
                    i += 1
                elif event.key == pygame.K_c:
                    pygame.image.save(screen, "data{0}.jpg".format(i))
                    i += 1

            if z == 1:
                screen.blit(brush, (x - 16, y - 16))
                pygame.display.update()
