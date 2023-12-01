from pathlib import Path


if __name__ == '__main__':
    root_path = Path(r"/home/coco/datasets/ped-car-temp")
    pathlist = root_path.rglob("*images")
    for path in pathlist:
        print(f"{path},")