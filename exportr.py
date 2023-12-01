import argparse
import sys
import onnx
import time
import cv2
import yaml
import onnxruntime
import onnxsim
 
sys.path.append("./")  # to run '$ python *.py' files in subdirectories
 
import torch
import torch.nn as nn
import numpy as np
import torch.nn.functional as F
 
import models
from models.experimental import attempt_load
from utils.activations import Hardswish, SiLU
from utils.general import set_logging, check_img_size
from utils.torch_utils import select_device
 
 
def set_parser():
    parser = argparse.ArgumentParser()
    #parser.add_argument( "--weights", type=str, default="0630.pt", help="weights path" )  # from yolov5/models/
    parser.add_argument( "--weights", type=str, default="/home/coco/yolov5-6.1/runs/train/exp16/weights/best.pt", help="weights path" )  # from yolov5/models/
    #parser.add_argument("--img-size", nargs="+", type=int, default=[224, 320], help="image size h w")  # height, width
    parser.add_argument("--img-size", nargs="+", type=int, default=[640, 640], help="image size h w")  # height, width
    parser.add_argument( "--config-file", type=str, default="models/yolov5s-hw.yaml",help="model config path", )
    parser.add_argument( "--image", type=str, default=r"1.jpg", help="input image same size as model input size")
    parser.add_argument("--batch-size", type=int, default=1, help="batch size")
    parser.add_argument("--dynamic", action="store_true", help="dynamic ONNX axes")
    parser.add_argument("--grid", action="store_true", help="export Detect() layer grid" )
    parser.add_argument( "--device", default="cpu", help="cuda device, i.e. 0 or 0,1,2,3 or cpu")
    opt = parser.parse_args()
    return opt
 
 
class MyExport(nn.Module):
    def __init__(self, model, opt, params):
        super(MyExport, self).__init__()
        self.model = model
        self.num_class = params["nc"]
        self.anchor_num = len(params["anchors"])
        self.input_w, self.input_h = opt.img_size[1], opt.img_size[0]
        self.anchors = torch.tensor(data=params["anchors"], dtype=torch.float32).view(3, 3, 2)
 
        # obtain YOLO decoding parameters
        self.feature_map_h, self.feature_map_w = list(), list()
        self.grid_y, self.grid_x, self.anchor_w, self.anchor_h = (
            list(),
            list(),
            list(),
            list(),
        )
        for anchors, stride in zip(self.anchors, [8, 16, 32]):
            feature_map_h = int(self.input_h / stride)
            feature_map_w = int(self.input_w / stride)
            grid_y, grid_x, anchor_w_h = self.create_grid(
                anchors, stride, feature_map_h, feature_map_w
            )
            grid_x = grid_x.repeat(self.anchor_num, 1, 1).view(-1, 1).to("cpu")
            grid_y = grid_y.repeat(self.anchor_num, 1, 1).view(-1, 1).to("cpu")
            anchor_w_h = (anchor_w_h.repeat(1, 1, feature_map_h, feature_map_w, 1).view(1, -1, 2).to("cpu"))
            anchor_w, anchor_h = torch.split(anchor_w_h, 1, 2)
            self.feature_map_w.append(float(feature_map_w))
            self.feature_map_h.append(float(feature_map_h))
            self.grid_x.append(grid_x)
            self.grid_y.append(grid_y)
            self.anchor_w.append(anchor_w.view(-1, 1))
            self.anchor_h.append(anchor_h.view(-1, 1))
 
    def forward(self, x):
        # x = self.focus_conv(x)
        features_list = self.model(x)
        all_preds = list()
        for i, features in enumerate(features_list):
            predictions = self.convert_channel(features)
            n = predictions.size(1)
            predictions = predictions.sigmoid()
            preds_x, preds_y, preds_w, preds_h, conf, cn = torch.split(predictions, [1, 1, 1, 1, 1, self.num_class], 2)
            preds_x = (preds_x * 2.0 - 0.5 + self.grid_x[i]) / self.feature_map_w[i]
            preds_y = (preds_y * 2.0 - 0.5 + self.grid_y[i]) / self.feature_map_h[i]
            preds_w = (torch.exp(torch.log(preds_w * 2.0) * 2.0)* self.anchor_w[i] / self.feature_map_w[i])
            preds_h = (torch.exp(torch.log(preds_h * 2.0) * 2.0)* self.anchor_h[i]/ self.feature_map_h[i])
            # cmax = torch.max(cn, dim=2).values
            preds = torch.cat((preds_x, preds_y, preds_w, preds_h, conf, cn),dim=2,)
            # preds = torch.cat((preds_x, preds_y, preds_w, preds_h, conf, c1, cn), dim=2)
            all_preds.append(preds)
        if len(all_preds) == 1:
            return preds.view(1, 1, n, 5 + self.num_class)
        else:
            output = torch.cat(all_preds, dim=1)
            output = output.view(1, 1, output.shape[1], output.shape[2])
            return output
 
    def convert_channel(self, features):
        b, c, h, w = features.size()
        n = h * w
        print(c, h, w)
        print(self.anchor_num, 5 + self.num_class, n)
        print(c*h*w,(self.anchor_num*(5 + self.num_class)*n))
        predictions = features.view(1, self.anchor_num, 5 + self.num_class, n)
        predictions = predictions.permute(0, 1, 3, 2).contiguous()
        predictions = predictions.view(1, self.anchor_num * n, 5 + self.num_class)
        return predictions
 
    def create_grid(self, anchors, stride, feature_map_h, feature_map_w):
        grid_y, grid_x = torch.meshgrid(
            [torch.arange(feature_map_h), torch.arange(feature_map_w)]
        )
        anchor_w_h = anchors / stride
        anchor_w_h = anchor_w_h.view(1, anchors.size(0), 1, 1, anchors.size(1))
        grid_y = grid_y.to(torch.float32)
        grid_x = grid_x.to(torch.float32)
        anchor_w_h = anchor_w_h.to(torch.float32)
        return grid_y, grid_x, anchor_w_h
 
 
def main():
    opt = set_parser()
    opt.img_size *= 2 if len(opt.img_size) == 1 else 1  # expand
    print(opt)
    set_logging()
    t = time.time()
 
    # Load PyTorch model
    device = select_device(opt.device)
    model = attempt_load(opt.weights, map_location=device)  # load FP32 model
 
    # Load config
    with open(opt.config_file) as f:
        params = yaml.safe_load(f)
 
    # Checks
    gs = int(max(model.stride))  # grid size (max stride)
    opt.img_size = [
        check_img_size(x, gs) for x in opt.img_size
    ]  # verify img_size are gs-multiples
 
    # Input
    input = cv2.imread(opt.image)
    img = input[:, :, ::-1].copy()
    img = cv2.resize(img, dsize=(opt.img_size[1], opt.img_size[0]))
    img = img[:, :, ::-1].transpose(2, 0, 1)
    img = np.ascontiguousarray(img)
    img = torch.from_numpy(img)
    img = img.float()  # uint8 to fp16/32
    img /= 255.0  # 0 - 255 to 0.0 - 1.0
    if img.ndimension() == 3:
        img = img.unsqueeze(0)
 
    # Update model
    for k, m in model.named_modules():
        m._non_persistent_buffers_set = set()  # pytorch 1.6.0 compatibility
        if isinstance(m, models.common.Conv):  # assign export-friendly activations
            if isinstance(m.act, nn.Hardswish):
                m.act = Hardswish()
            elif isinstance(m.act, nn.SiLU):
                m.act = SiLU()
        elif isinstance(m, models.common.Focus):
            m.export = True
        # elif isinstance(m, models.yolo.Detect):
        # m.forward = m.forward_export  # assign forward (optional)
    model.model[-1].export = True#not opt.grid  # set Detect() layer grid export
 
    export_model = MyExport(model, opt, params)
    # print(export_model)
    y = export_model(img)  # dry run
    print(y)
    # ONNX export
    print("\nStarting ONNX export with onnx %s..." % onnx.__version__)
    f = opt.weights.replace(".pt", ".onnx")  # filename
    torch.onnx.export(
        export_model,
        img,
        f,
        verbose=False,
        opset_version=11,
        input_names=["input"],
        output_names=["output"],
        dynamic_axes={
            "input": {0: "batch", 2: "height", 3: "width"},  # size(1,3,640,640)
            "output": {0: "batch", 2: "y", 3: "x"},
        }
        if opt.dynamic
        else None,
    )
 
    # Checks
    onnx_model = onnx.load(f)  # load onnx model
    onnx.checker.check_model(onnx_model)  # check onnx model
    print("ONNX export success, saved as %s" % f)
 
 
    try:
        print(f' simplifying with onnx-simplifier {onnxsim.__version__}...')
        model_onnx, check = onnxsim.simplify(
            model_onnx,
            input_shapes={'images': list(img.shape)})
        assert check, 'assert check failed'
        onnx.save(onnx_model, f)
    except Exception as e:
        print(f' simplifier failure: {e}')
 
    model = onnxruntime.InferenceSession(f, None, providers=["CPUExecutionProvider"])
    ort_inputs = {model.get_inputs()[0].name: img.numpy()}
    ort_outs = model.run(None, ort_inputs)[0].squeeze()
    print(ort_outs)
 
    # Finish
    print("\nExport complete (%.2fs). Visualize with https://github.com/lutzroeder/netron."% (time.time() - t))
 
 
if __name__ == "__main__":
    main()