# Blue and Red Object Distance Detector

A simple computer-vision project that uses a webcam to detect a blue object and a red object, then displays the distance between their centres in real time.

## Features

- Live webcam input
- Blue and red object detection using HSV colour masks
- Bounding boxes and centre points for the detected objects
- Real-time centre-to-centre distance measurement
- HSV trackbars for experimenting with and tuning colour ranges

## Requirements

- Python 3
- A webcam
- OpenCV
- NumPy

Install the Python packages:

```bash
pip install opencv-python numpy
```

## Run the project

From the project folder, run:

```bash
python distance.py
```

Place one blue object and one red object in front of the camera. The application opens a window named `Detection`, draws a box around each detected object, and shows their distance in pixels. Press `Esc` to close it.

## Files

| File | Description |
| --- | --- |
| `distance.py` | Detects blue and red objects and calculates the distance between their centre points. |
| `hsv_color_tuner.py` | Provides HSV trackbars to help find suitable colour ranges for detection. |

## How it works

1. The webcam frame is converted from BGR to HSV colour space.
2. HSV masks isolate blue and red regions. Red uses two ranges because it wraps around the HSV hue scale.
3. The largest contour for each colour is selected.
4. The centre of each object's bounding box is calculated.
5. The Euclidean distance between the two centres is displayed:

```text
distance = sqrt((x1 - x2)^2 + (y1 - y2)^2)
```

## Notes

- Lighting, shadows, and background colours can affect detection quality.
- Use `hsv-color_tuner.py` to tune HSV values if the target objects are not detected reliably.
- Converting pixel distance into a real-world unit requires camera calibration and a known reference scale.

## Contributors 

This project was created collaboratively by **Fatima Alfurais** and **Ahmed**, with supervision and support from the **RobotyLAB community**.
