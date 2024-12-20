import colorsys
import os
import time
import serial

import numpy as np
import torch
import torch.nn as nn
import cv2
from PIL import ImageDraw, ImageFont, Image

from nets.yolo import YoloBody
from utils.utils import (cvtColor, get_anchors, get_classes, preprocess_input,resize_image, show_config)
from utils.utils_bbox import DecodeBox, DecodeBoxNP

from serial_port import Serial_port

class YOLO(object):
    _defaults = {
        "model_path"        : 'model_data/best_epoch_weights.pth',
        "classes_path"      : 'model_data/cls_class.txt',
        "anchors_path"      : 'model_data/yolo_anchors.txt',
        "anchors_mask"      : [[3,4,5], [1,2,3]],
        "phi"               : 0,  
        "input_shape"       : [416, 416],
        "confidence"        : 0.5,
        "nms_iou"           : 0.3,
        "letterbox_image"   : False,
        "cuda"              : False,
    }

    @classmethod
    def get_defaults(cls, n):
        if n in cls._defaults:
            return cls._defaults[n]
        else:
            return "Unrecognized attribute name '" + n + "'"

    def __init__(self, **kwargs):
        self.__dict__.update(self._defaults)
        for name, value in kwargs.items():
            setattr(self, name, value)
            self._defaults[name] = value             
        #---------------------------------------------------#
        #   获得种类和先验框的数量
        #---------------------------------------------------#
        self.class_names, self.num_classes  = get_classes(self.classes_path)
        self.anchors, self.num_anchors      = get_anchors(self.anchors_path)
        self.bbox_util                      = DecodeBox(self.anchors, self.num_classes, (self.input_shape[0], self.input_shape[1]), self.anchors_mask)
        #---------------------------------------------------#
        #   画框设置不同的颜色
        #---------------------------------------------------#
        hsv_tuples = [(x / self.num_classes, 1., 1.) for x in range(self.num_classes)]
        self.colors = list(map(lambda x: colorsys.hsv_to_rgb(*x), hsv_tuples))
        self.colors = list(map(lambda x: (int(x[0] * 255), int(x[1] * 255), int(x[2] * 255)), self.colors))
        self.generate()
        show_config(**self._defaults)
        
    def generate(self, onnx=False):
        #---------------------------------------------------#
        #   建立yolo模型，载入yolo模型的权重
        #---------------------------------------------------#
        self.net = YoloBody(self.anchors_mask, self.num_classes, self.phi)
        device = torch.device('cpu')
        self.net = self.net.to(torch.device("cpu"))
        self.net.load_state_dict(torch.load(self.model_path, map_location=device))
        self.net.qconfig = torch.quantization.get_default_qconfig('qnnpack')
        torch.backends.quantized.engine = "qnnpack"
        quantized_model = torch.quantization.prepare(self.net,inplace=False)
        quantized_model = torch.quantization.convert(self.net,inplace=False)
        quantized_model.eval()
#        print('{} model, anchors, and classes loaded.'.format(self.model_path))
    #---------------------------------------------------#
    #   检测图片
    #---------------------------------------------------#
    def detect_image(self, image, crop = False, count = False):
        image_shape = np.array(np.shape(image)[0:2])
        #---------------------------------------------------------#
        #   在这里将图像转换成RGB图像，防止灰度图在预测时报错。
        #   代码仅仅支持RGB图像的预测，所有其它类型的图像都会转化成RGB
        #---------------------------------------------------------#
        image       = cvtColor(image)
        #---------------------------------------------------------#
        #   给图像增加灰条，实现不失真的resize
        #   也可以直接resize进行识别
        #---------------------------------------------------------#
        image_data  = resize_image(image, (self.input_shape[1],self.input_shape[0]), self.letterbox_image)
        #---------------------------------------------------------#
        #   添加上batch_size维度
        #---------------------------------------------------------#
        image_data  = np.expand_dims(np.transpose(preprocess_input(np.array(image_data, dtype='float32')), (2, 0, 1)), 0)

        with torch.no_grad():
            images = torch.from_numpy(image_data)
            if self.cuda:
                images = images.cuda()
            #---------------------------------------------------------#
            #   将图像输入网络当中进行预测！
            #---------------------------------------------------------#
            outputs = self.net(images)
            outputs = self.bbox_util.decode_box(outputs)
            #---------------------------------------------------------#
            #   将预测框进行堆叠，然后进行非极大抑制
            #---------------------------------------------------------#
            results = self.bbox_util.non_max_suppression(torch.cat(outputs, 1), self.num_classes, self.input_shape, 
                        image_shape, self.letterbox_image, conf_thres = self.confidence, nms_thres = self.nms_iou)                                        
            if results[0] is None: 
                return image

            top_label   = np.array(results[0][:, 6], dtype = 'int32')
            top_conf    = results[0][:, 4] * results[0][:, 5]
            top_boxes   = results[0][:, :4]
        #---------------------------------------------------------#
        #   设置字体与边框厚度
        #---------------------------------------------------------#
        font        = ImageFont.truetype(font='model_data/simhei.ttf', size=np.floor(3e-2 * image.size[1] + 0.5).astype('int32'))
        thickness   = int(max((image.size[0] + image.size[1]) // np.mean(self.input_shape), 1))

        #---------------------------------------------------------#
        #   图像绘制
        #---------------------------------------------------------#
        for i, c in list(enumerate(top_label)):
            predicted_class = self.class_names[int(c)]
            box             = top_boxes[i]
            score           = top_conf[i]
            
            top, left, bottom, right = box
            top     = max(0, np.floor(top).astype('int32'))
            left    = max(0, np.floor(left).astype('int32'))
            bottom  = min(image.size[1], np.floor(bottom).astype('int32'))
            right   = min(image.size[0], np.floor(right).astype('int32'))

            label = '{} {:.2f}'.format(predicted_class, score)
            draw = ImageDraw.Draw(image)
            label_size = draw.textsize(label, font)
            label = label.encode('utf-8')
            
            print(int(c),predicted_class)
            if int(c) == 0:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 1:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 2:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 3:
                data_to_send = "A"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")    
            elif int(c) == 4:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 5:
                data_to_send = "C"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 6:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 7:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 8:
                data_to_send = "B"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")
            elif int(c) == 9:
                data_to_send = "D"
                Serial_port(data_to_send)
                data_to_send = "E"
                Serial_port(data_to_send)
                print(data_to_send)
                print("--------------------------------------------------------")                                        
            if top - label_size[1] >= 0:
                text_origin = np.array([left, top - label_size[1]])
            else:
                text_origin = np.array([left, top + 1])

            for i in range(thickness):
                draw.rectangle([left + i, top + i, right - i, bottom - i], outline=self.colors[c])
            draw.rectangle([tuple(text_origin), tuple(text_origin + label_size)], fill=self.colors[c])
            draw.text(text_origin, str(label,'UTF-8'), fill=(0, 0, 0), font=font)
            del draw

        return image
        
    
