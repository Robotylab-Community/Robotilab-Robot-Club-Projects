import cv2
import numpy as np
import math


cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)
    
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([100, 150, 50])
    upper_blue = np.array([140, 255, 255])
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)


    lower_red1 = np.array([0,   150, 100])
    upper_red1 = np.array([5,   255, 255])
    mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)

    lower_red2 = np.array([175, 150, 100])
    upper_red2 = np.array([180, 255, 255])
    mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)

    mask_red = mask_red1 + mask_red2


    contours_blue, _ = cv2.findContours(mask_blue, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours_red, _ = cv2.findContours(mask_red, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    center_blue = None
    center_red = None

   
    if contours_blue:
        c = max(contours_blue, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(c)
        center_blue = (x + w//2, y + h//2)

        cv2.rectangle(frame, (x,y), (x+w,y+h), (255,0,0), 2)
        cv2.circle(frame, center_blue, 5, (255,0,0), -1)

    
    if contours_red:
        c = max(contours_red, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(c)
        center_red = (x + w//2, y + h//2)

        cv2.rectangle(frame, (x,y), (x+w,y+h), (0,0,255), 2)
        cv2.circle(frame, center_red, 5, (0,0,255), -1)

    
    if center_blue and center_red:
        distance = math.sqrt(
            (center_blue[0] - center_red[0])**2 +
            (center_blue[1] - center_red[1])**2
        )

        cv2.line(frame, center_blue, center_red, (0,255,0), 2)

        cv2.putText(frame, f"Distance: {int(distance)} px",
                    (50,50), cv2.FONT_HERSHEY_SIMPLEX,
                    1, (0,255,0), 2)


    cv2.imshow("Detection", frame)

    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
