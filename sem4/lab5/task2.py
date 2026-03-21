import cv2
import numpy as np

blur_value = 1

def on_trackbar(val):
    global blur_value
    # теперь максимум 50 -> ядро до 101
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

cv2.namedWindow("Filter", cv2.WINDOW_AUTOSIZE)

# максимум увеличен до 50
cv2.createTrackbar("Blur (0=sharp)", "Filter", 0, 50, on_trackbar)

while True:
    grabbed, frame = capture.read()
    if not grabbed:
        break

    if blur_value == 1:
        result = sharpen(frame)
    else:
        result = cv2.GaussianBlur(frame, (blur_value, blur_value), 0)

    combined = np.hstack([frame, result])
    cv2.imshow("Filter", combined)

    # фикс Q на Windows
    key = cv2.waitKey(1)
    if key == ord('q') or key == ord('Q') or key == 27:
        break

capture.release()
cv2.destroyAllWindows()