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
    brush = pygame.image.load("label/" + sys.argv[1] + "-01.png")
    brush = pygame.transform.scale(brush, (128, 128))
    screen.blit(brush, (350, 250))
    pygame.draw.circle(screen, (40, 40, 40), (415, 315), 100, 10)
    pygame.display.update()

    while 1:
        clock.tick(60)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            pygame.display.update()