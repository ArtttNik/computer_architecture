import cv2
import numpy as np

blur_value = 1

def on_trackbar(val):
    global blur_value
    blur_value = val * 2 + 1

def sharpen(frame):
    kernel = np.array([[ 0, -1,  0],
                       [-1,  5, -1],
                       [ 0, -1,  0]])
    return cv2.filter2D(frame, -1, kernel)

capture = cv2.VideoCapture(0)

if not capture.isOpened():
    print("Камера не найдена")
    exit()

cv2.namedWindow("Filter")
cv2.createTrackbar("Blur (0=sharp)", "Filter", 0, 50, on_trackbar)

while True:
    grabbed, frame = capture.read()
    if not grabbed:
        break

    if blur_value == 1:
        result = cv2.medianBlur(frame, 5)  # радиус 5
    else:
        result = cv2.GaussianBlur(frame, (blur_value, blur_value), 0)

    combined = np.hstack([frame, result])
    cv2.imshow("Filter", combined)

    key = cv2.waitKey(1) & 0xFF

    if key == ord('q'):
        print("Exit pressed")
        break

capture.release()
cv2.destroyAllWindows()