import cv2
import numpy as np
import math


cap = cv2.VideoCapture(0)

def empty(x):
    pass


cv2.namedWindow("Trackbars")
cv2.resizeWindow("Trackbars", 600, 240)


cv2.createTrackbar("HUE Min", "Trackbars", 0, 179, empty)
cv2.createTrackbar("HUE Max", "Trackbars", 179, 179, empty)

cv2.createTrackbar("SAT Min", "Trackbars", 0, 255, empty)
cv2.createTrackbar("SAT Max", "Trackbars", 255, 255, empty)

cv2.createTrackbar("VALUE Min", "Trackbars", 0, 255, empty)
cv2.createTrackbar("VALUE Max", "Trackbars", 255, 255, empty)

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to read frame")
        break


    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

   
    h_min = cv2.getTrackbarPos("HUE Min", "Trackbars")
    h_max = cv2.getTrackbarPos("HUE Max", "Trackbars")

    s_min = cv2.getTrackbarPos("SAT Min", "Trackbars")
    s_max = cv2.getTrackbarPos("SAT Max", "Trackbars")

    v_min = cv2.getTrackbarPos("VALUE Min", "Trackbars")
    v_max = cv2.getTrackbarPos("VALUE Max", "Trackbars")

   
    lower = np.array([h_min, s_min, v_min])
    upper = np.array([h_max, s_max, v_max])

   
    mask = cv2.inRange(hsv, lower, upper)

  
    result = cv2.bitwise_and(frame, frame, mask=mask)

   
    cv2.imshow("Trackbars", np.zeros((1, 600), np.uint8))

    
    cv2.imshow("Original", frame)
    cv2.imshow("Mask", mask)
    cv2.imshow("Result", result)


    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()