import cv2

capture = cv2.VideoCapture(0)

if not capture.isOpened():
    print("Камера не найдена")
    exit()

fps = 20.0
width  = int(capture.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(capture.get(cv2.CAP_PROP_FRAME_HEIGHT))

fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer = cv2.VideoWriter('output.avi', fourcc, fps, (width, height))

cv2.namedWindow("Camera", cv2.WINDOW_AUTOSIZE)

print("Запись... нажми Q для выхода")

while True:
    grabbed, frame = capture.read()
    if not grabbed:
        break

    writer.write(frame)
    cv2.imshow("Camera", frame)

    key = cv2.waitKey(1)
    if key == ord('q') or key == ord('Q') or key == 27:
        break

capture.release()
writer.release()
cv2.destroyAllWindows()
print("Записано в output.avi")