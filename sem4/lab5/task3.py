import cv2
import numpy as np

# ─── Загрузка шаблона ───────────────────────────────────────────────────────
template = cv2.imread('template.png')
if template is None:
    print("Файл template.png не найден")
    exit()

tmpl_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

# ─── Захват камеры ───────────────────────────────────────────────────────────
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Камера не найдена")
    exit()

grabbed, frame = cap.read()
fh, fw = frame.shape[:2]
th, tw = tmpl_gray.shape[:2]

print(f"Кадр камеры: {fw}x{fh}")
print(f"Шаблон до масштабирования: {tw}x{th}")

# Если шаблон больше 1/3 кадра — уменьшаем
max_tw = fw // 3
max_th = fh // 3
if tw > max_tw or th > max_th:
    scale = min(max_tw / tw, max_th / th)
    tmpl_gray = cv2.resize(tmpl_gray, (int(tw * scale), int(th * scale)))
    template  = cv2.resize(template,  (int(tw * scale), int(th * scale)))
    th, tw = tmpl_gray.shape[:2]
    print(f"Шаблон после масштабирования: {tw}x{th}")

print(f"Шаблон итого: {tw}x{th}")

WINDOW = "Template Search"
cv2.namedWindow(WINDOW, cv2.WINDOW_AUTOSIZE)
threshold_val = [30]  # начинаем с низкого порога

def on_threshold(val):
    threshold_val[0] = max(val, 1)

cv2.createTrackbar("Threshold %", WINDOW, 30, 100, on_threshold)


def build_heatmap(result, frame_shape):
    norm = cv2.normalize(result, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    heatmap = cv2.resize(norm, (frame_shape[1], frame_shape[0]))
    heatmap = cv2.applyColorMap(heatmap, cv2.COLORMAP_JET)
    return heatmap


def nms(boxes, overlap_thresh=0.3):
    if not boxes:
        return []
    boxes = np.array(boxes, dtype=float)
    picked = []
    x1, y1, x2, y2 = boxes[:,0], boxes[:,1], boxes[:,2], boxes[:,3]
    area = (x2 - x1 + 1) * (y2 - y1 + 1)
    idxs = np.argsort(y2)
    while len(idxs) > 0:
        last = idxs[-1]
        picked.append(last)
        suppress = [len(idxs) - 1]
        for pos in range(len(idxs) - 1):
            i = idxs[pos]
            ix1 = max(x1[i], x1[last])
            iy1 = max(y1[i], y1[last])
            ix2 = min(x2[i], x2[last])
            iy2 = min(y2[i], y2[last])
            iw = max(0, ix2 - ix1 + 1)
            ih = max(0, iy2 - iy1 + 1)
            overlap = (iw * ih) / area[i]
            if overlap > overlap_thresh:
                suppress.append(pos)
        idxs = np.delete(idxs, suppress)
    return [tuple(map(int, boxes[i])) for i in picked]


def draw_matches(frame, result, threshold):
    thresh = threshold / 100.0
    locations = np.where(result >= thresh)
    boxes = []
    for pt in zip(*locations[::-1]):
        boxes.append((pt[0], pt[1], pt[0] + tw, pt[1] + th))
    boxes = nms(boxes, overlap_thresh=0.3)
    for (x1, y1, x2, y2) in boxes:
        confidence = float(result[y1, x1])
        green = int(255 * confidence)
        blue  = int(255 * (1 - confidence))
        cv2.rectangle(frame, (x1, y1), (x2, y2), (blue, green, 0), 2)
        cv2.putText(frame, f"{confidence:.2f}", (x1, y1 - 6),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (blue, green, 0), 1)
    return len(boxes)


print("Q или Esc — выход, S — сохранить кадр")

while True:
    grabbed, frame = cap.read()
    if not grabbed:
        break

    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Проверка что шаблон меньше кадра
    if tw >= fw or th >= fh:
        cv2.putText(frame, "TEMPLATE TOO LARGE", (50, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imshow(WINDOW, frame)
        key = cv2.waitKey(1)
        if key == ord('q') or key == ord('Q') or key == 27:
            break
        continue

    result = cv2.matchTemplate(frame_gray, tmpl_gray, cv2.TM_CCOEFF_NORMED)
    _, max_val, _, max_loc = cv2.minMaxLoc(result)

    heatmap = build_heatmap(result, frame.shape)
    display = frame.copy()
    count = draw_matches(display, result, threshold_val[0])

    # Всегда рисуем лучшее совпадение отдельно (красным) даже ниже порога
    x, y = max_loc
    cv2.rectangle(display, (x, y), (x + tw, y + th), (0, 0, 255), 1)
    cv2.putText(display, f"best:{max_val:.2f}", (x, y - 6),
                cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 255), 1)

    # Миниатюра шаблона в углу
    th_s = min(th, 80)
    tw_s = int(tw * th_s / th)
    tmpl_thumb = cv2.resize(template, (tw_s, th_s))
    display[10:10+th_s, 10:10+tw_s] = tmpl_thumb
    cv2.rectangle(display, (10, 10), (10+tw_s, 10+th_s), (255, 255, 0), 1)
    cv2.putText(display, "template", (10, 10+th_s+14),
                cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255, 255, 0), 1)

    status = f"Found: {count}  Best: {max_val:.2f}  Thr: {threshold_val[0]}%"
    cv2.putText(display, status, (10, frame.shape[0] - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.55, (0, 255, 255), 1)

    blended = cv2.addWeighted(display, 0.7, heatmap, 0.3, 0)
    cv2.imshow(WINDOW, blended)

    key = cv2.waitKey(1)
    if key == ord('q') or key == ord('Q') or key == 27:
        break
    elif key == ord('s') or key == ord('S'):
        cv2.imwrite('found_frame.png', blended)
        print("Сохранено: found_frame.png")

cap.release()
cv2.destroyAllWindows()