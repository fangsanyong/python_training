from collections import OrderedDict, namedtuple

import numpy as np
import torch
import torch.nn as nn
import tensorrt as trt


def nptype(trt_type):
    """
    Returns the numpy-equivalent of a TensorRT :class:`DataType` .

    :arg trt_type: The TensorRT data type to convert.

    :returns: The equivalent numpy type.
    """
    import numpy as np

    mapping = {
        trt.DataType.FLOAT: np.float32,
        trt.DataType.HALF: np.float16,
        trt.DataType.INT8: np.int8,
        trt.DataType.INT32: np.int32,
        trt.DataType.BOOL: np.bool_,
    }
    if trt_type in mapping:
        return mapping[trt_type]
    raise TypeError("Could not resolve TensorRT datatype to an equivalent numpy datatype.")

class DetectMultiBackend(nn.Module):
    # YOLOv5 MultiBackend class for python inference on various backends
    def __init__(self, weights='yolov5s.pt', device=torch.device('cuda')):
        super().__init__()
        stride, names = 32, ['person','car']  # assign defaults
        trt.init_libnvinfer_plugins(None, '')
        Binding = namedtuple('Binding', ('name', 'dtype', 'shape', 'data', 'ptr'))
        logger = trt.Logger(trt.Logger.INFO)
        with open(weights, 'rb') as f, trt.Runtime(logger) as runtime:
            model = runtime.deserialize_cuda_engine(f.read())
        context = model.create_execution_context()
        bindings = OrderedDict()
        fp16 = False  # default updated below
        dynamic_input = False
        for index in range(model.num_bindings):
            name = model.get_binding_name(index)
            dtype = nptype(model.get_binding_dtype(index))
            if model.binding_is_input(index):
                if -1 in tuple(model.get_binding_shape(index)):  # dynamic
                    dynamic_input = True
                    context.set_binding_shape(index, tuple(model.get_profile_shape(0, index)[2]))
                if dtype == np.float16:
                    fp16 = True
            shape = tuple(context.get_binding_shape(index))
            data = torch.from_numpy(np.empty(shape, dtype=np.dtype(dtype))).to(device)
            bindings[name] = Binding(name, dtype, shape, data, int(data.data_ptr()))
        binding_addrs = OrderedDict((n, d.ptr) for n, d in bindings.items())
        batch_size = bindings['images'].shape[0]  # if dynamic, this is instead max batch size

        self.__dict__.update(locals())  # assign all variables to self

    def forward(self, im):

        if im.shape != self.bindings['images'].shape and self.dynamic_input:
            self.context.set_binding_shape(self.model.get_binding_index('images'), im.shape)  # reshape if dynamic
            self.bindings['images'] = self.bindings['images']._replace(shape=im.shape)
        assert im.shape == self.bindings['images'].shape, (
            f"image shape {im.shape} exceeds model max shape {self.bindings['images'].shape}" if self.dynamic_input
            else f"image shape {im.shape} does not match model shape {self.bindings['images'].shape}")
        self.binding_addrs['images'] = int(im.data_ptr())
        self.context.execute_v2(list(self.binding_addrs.values()))
        nums = self.bindings['num'].data[0]
        boxes = self.bindings['boxes'].data[0]
        scores = self.bindings['scores'].data[0]
        classes = self.bindings['classes'].data[0]
        boxes = boxes[0:nums]
        scores = scores[0:nums]
        classes = classes[0:nums]
        y = [torch.cat([boxes, torch.unsqueeze(scores, -1), torch.unsqueeze(classes, -1)], -1)]
        return y

    def warmup(self, imgsz=(1, 3, 640, 640)):
        im = torch.zeros(*imgsz, dtype=torch.float32, device=self.device)  # input
        self.forward(im)  # warmup
