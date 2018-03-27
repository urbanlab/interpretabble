from matplotlib import pyplot as plt
import pygame
import random
from PIL import Image
import tensorflow as tf
import numpy as np
import os
import sys

class Data(object):

    def __init__(self):
        self.category = ["ear", "nose", "bed", "house", "ambulance", "food"]
        self.num_size = 1000
        self.len_cat = len(self.category)
        self.data_dir = "other/"
        self.train_data = []
        self.features = []
        self.is_trainable = 0
        self.n_node_1 = 500
        self.n_node_2 = 500
        self.n_node_3 = 500
        self.n_output = 3
        self.learning_rate = 0.00003

    """def add_data(self):
        self.features = []

        for i in range(30):
            o = 0
            os.system("python3 resize.py images/data{0}.jpg".format(i))
            n = Image.frombytes('L', (56, 56), Image.open("some0.pgm").tobytes())
            images = np.array(n)
            images_parsed = np.zeros(3136)
            for i in range(len(images)):
                for j in range(len(images[i])):
                    images_parsed[o] = 255 - images[i][j] if 255 - images[i][j] > 90 else 0
                    o += 1
            self.features.append(images_parsed)
            #np.reshape(images, (1, 78400))
        print(len(self.features), self.features[0])"""

    def add_data(self):
        images = []
        directories = [f for f in os.listdir(self.data_dir)
        if os.path.isdir(os.path.join(self.data_dir, f))]

        print(directories)
        for d in directories:
            c = 0
            fruit_directory = os.path.join(self.data_dir, d)
            file_names = [
            os.path.join(fruit_directory, f)
            for f in os.listdir(fruit_directory)
                if f.endswith(".jpg")]
            for f in file_names:
                o = 0
                if c / 100 == 1:
                    break
                os.system("python3 resize.py " + f)
                #images_parsed = np.zeros(3136)
                n = Image.frombytes('L', (56, 56), Image.open("some0.pgm").tobytes())
                images = np.array(n)
                """for i in range(len(images)):
                    for j in range(len(images[i])):
                        images_parsed[o] = images[i][j]
                        o += 1"""
                images_parsed = np.reshape(images, (3136))
                self.features.append(images_parsed)
                c += 1
            print(c)
        return images

    """def add_data(self):

        category_filenames = []

        for name in self.category:
            filename = os.path.join(self.data_dir, name)
            category_filenames.append(filename)

        print(category_filenames)
        self.train_data = []

        for i_category, category in enumerate(self.category):
            data = np.load(category_filenames[i_category])
            n_data = len(data)
            n_categories = len(self.category)  # number of classes
            print("[%d/%d] Reading category index %d: '%s' (%d images)" %
                (i_category + 1, n_categories, i_category, category, n_data))

            for j, data_j in enumerate(data):
                if j < self.num_size:
                    # img = np.array(data_j).reshape((img_size, img_size))
                    img = np.array(data_j)
                    self.train_data.append((img, i_category))
                else:
                    break
        self.features = []
        self.output_test = []

        e = 0
        for k in range(3):
            for c in range(1000):
                self.features.append(np.array(self.train_data[c + e][0]))
                self.output_test.append(float(k))
            e += 1000"""

    def add_target(self):
        target = np.zeros((600, 6))

        for i in range(600):
            if i / 100 < 1:
                target[i][0] = 1
            elif i / 100 >= 1 and i / 100 < 2:
                target[i][1] = 1
            elif i / 100 >= 2 and i / 100 < 3:
                target[i][2] = 1
            elif i / 100 >= 3 and i / 100 < 4:
                target[i][3] = 1
            elif i / 100 >= 4 and i / 100 < 5:
                target[i][4] = 1
            elif i / 100 >= 5 and i / 100 < 6:
                target[i][5] = 1
        return target

    def brain(self):
        x_h = tf.placeholder(tf.float32, [None, 3136])
        image = tf.reshape(x_h, [-1, 56, 56, 1])
        y_h = tf.placeholder(tf.float32, [None, 6])

        target = self.add_target()

        conv1 = tf.layers.conv2d(image, 64, [5, 5], padding="same", activation=tf.nn.relu)
        pool1 = tf.layers.max_pooling2d(conv1, [2, 2], strides=2)
        conv2 = tf.layers.conv2d(pool1, 128, [5, 5], padding="same", activation=tf.nn.relu)
        pool2 = tf.layers.max_pooling2d(conv2, [2, 2], strides=2)
        pool2_flat = tf.reshape(pool2, [-1, 14 * 14 * 128])
        dense = tf.layers.dense(pool2_flat, 2084, activation=tf.nn.sigmoid)
        dropout = tf.layers.dropout(dense, rate=0.4)
        logits = tf.layers.dense(dropout, 6)
        y = tf.nn.softmax(logits)
        entropy = tf.losses.softmax_cross_entropy(onehot_labels=y_h, logits=logits, label_smoothing=1e-5)
        correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_h, 1))
        accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
        p_train = tf.train.AdamOptimizer(self.learning_rate).minimize(entropy)
        p_init = tf.initialize_all_variables()
        saver = tf.train.Saver()

        print("la\n")
        with tf.Session() as sess:
            sess.run(p_init)
            if self.is_trainable:
                try:
                    for i in range(200):
                        if not i:
                            Image.fromarray(np.reshape(self.features[1], (56, 56)), 'L').show()
                            Image.fromarray(np.reshape(self.features[10], (56, 56)), 'L').show()
                            Image.fromarray(np.reshape(self.features[20], (56, 56)), 'L').show()
                        sess.run(p_train, feed_dict={x_h: self.features, y_h: target})
                        loss = sess.run(entropy, feed_dict={x_h: self.features, y_h: target})
                        t_accuracy, y_pred = sess.run([accuracy, y], feed_dict={x_h: self.features, y_h: target})
                        print("Iteration : {0}, Loss : {1}, Train accuracy: {2}".format(i, loss, t_accuracy))
                #except ValueError:
                 #   pass
                finally:
                    builder = tf.saved_model.builder.SavedModelBuilder(os.path.join(os.getcwd(), "model/model"))
                    tensor_info_x = tf.saved_model.utils.build_tensor_info(x_h)
                    tensor_info_y = tf.saved_model.utils.build_tensor_info(y)
                    prediction_signature = (
                        tf.saved_model.signature_def_utils.build_signature_def(
                            inputs={'x_input': tensor_info_x},
                            outputs={'y_output': tensor_info_y},
                            method_name=tf.saved_model.signature_constants.PREDICT_METHOD_NAME))
                    builder.add_meta_graph_and_variables(
                        sess, [tf.saved_model.tag_constants.SERVING],
                        signature_def_map={
                            tf.saved_model.signature_constants.DEFAULT_SERVING_SIGNATURE_DEF_KEY:
                                prediction_signature 
                        },
                    )
                    builder.save()
                    #save_path = saver.save(sess, os.path.join(os.getcwd(), "model/model"))
                #print("Model saved in : {0}".format(save_path))
            else:
                signature_key = tf.saved_model.signature_constants.DEFAULT_SERVING_SIGNATURE_DEF_KEY
                input_key = 'x_input'
                output_key = 'y_output'
                meta_graph_def = tf.saved_model.loader.load(
                    sess,
                    [tf.saved_model.tag_constants.SERVING],
                    os.path.join(os.getcwd(), "model/model"))
                signature = meta_graph_def.signature_def
                x_tensor_name = signature[signature_key].inputs[input_key].name
                y_tensor_name = signature[signature_key].outputs[output_key].name

                x = sess.graph.get_tensor_by_name(x_tensor_name)
                y = sess.graph.get_tensor_by_name(y_tensor_name)
                n = Image.frombytes('L', (56, 56), Image.open(sys.argv[1]).tobytes())
                images = np.array(n)
                """for i in range(len(images)):
                    for j in range(len(images[i])):
                        images[i][j] = 255 - images[i][j] if 255 - images[i][j] > 90 else 0"""
                features = np.reshape(images, (1, 3136))
                print(features)
                Image.fromarray(np.reshape(features, (56, 56)), 'L').show()
                predictions = sess.run(y, feed_dict={x: features})
                i = np.argmax(predictions)
                print(self.category[i])
                os.system("python3 recon.py " + self.category[i])
                sys.exit(84)
                



if __name__ == '__main__':
    data = Data()
    if len(sys.argv) == 1:
        data.is_trainable = 1
        data.add_data()
    else:
        data.is_trainable = 0
    data.brain()
