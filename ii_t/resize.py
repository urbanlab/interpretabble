from matplotlib import pyplot as plt
import pygame, random
from PIL import Image
import tensorflow as tf
import numpy as np
import os
import sys


class Reload_Model(object):

    def __init__(self):
        tf.reset_default_graph()
        self.saver = tf.train.Saver()
        with tf.Session() as self.sess:
            self.sess.run(tf.global_variables_initializer())
            self.saver.restore(self.sess, os.path.join(os.getcwd() + "model/model.ckpt"))
            print("Model load")


class Test_Model(object):

    def __init__(self):
        self.img = Image.open(sys.argv[1])
        self.basewidth = 56
        self.ah = 0
        self.resize_image()

    def resize_image(self):
        wpercent = (self.basewidth/float(self.img.size[0]))
        #hsize = int((float(self.img.size[1]) * float(wpercent)))
        img = self.img.resize((self.basewidth, 56), Image.ANTIALIAS)
        img.save('some{0}.jpg'.format(self.ah))
        os.system("convert some{0}.jpg some{1}.pgm".format(self.ah, self.ah))
        self.ah += 1


if __name__ == '__main__':
    model = Test_Model()
    #eval = Reload_Model()
    sys.exit(42)
