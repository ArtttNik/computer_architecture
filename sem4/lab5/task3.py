import cv2
import numpy as np
import mediapipe as mp
from mediapipe.tasks import python as mp_python
from mediapipe.tasks.python import vision as mp_vision
import urllib.request
import os
import sys

MODEL_PATH = "hand_landmarker.task"
MODEL_URL  = "https://storage.googleapis.com/mediapipe-models/hand_landmarker/hand_landmarker/float16/1/hand_landmarker.task"

if not os.path.exists(MODEL_PATH):
    print("Скачиваю модель hand_landmarker.task (~9 МБ)...")
    urllib.request.urlretrieve(MODEL_URL, MODEL_PATH)
    print("Готово.")

FINGER_TIPS  = [4, 8, 12, 16, 20]
FINGER_MIDS  = [3, 7, 11, 15, 19]
FINGER_BASES = [2, 6, 10, 14, 18]
WRIST        = 0

def landmarks_to_vector(lms):
    pts = np.array([[lm.x, lm.y] for lm in lms], dtype=np.float32)
    wrist = pts[WRIST]
    scale = np.linalg.norm(pts[9] - wrist) + 1e-6
    pts   = (pts - wrist) / scale

    features = []
    for tip in FINGER_TIPS:
        v = pts[tip]
        n = np.linalg.norm(v) + 1e-6
        features.extend((v / n).tolist())
    for base, mid, tip in zip(FINGER_BASES, FINGER_MIDS, FINGER_TIPS):
        v1 = pts[mid] - pts[base]
        v2 = pts[tip] - pts[mid]
        n1 = np.linalg.norm(v1) + 1e-6
        n2 = np.linalg.norm(v2) + 1e-6
        features.append(float(np.clip(np.dot(v1/n1, v2/n2), -1, 1)))
    for i in range(len(FINGER_TIPS)):
        for j in range(i+1, len(FINGER_TIPS)):
            features.append(float(np.linalg.norm(pts[FINGER_TIPS[i]] - pts[FINGER_TIPS[j]])))

    vec = np.array(features, dtype=np.float32)
    return vec / (np.linalg.norm(vec) + 1e-6)

def make_detector(running_mode, num_hands=1, det_conf=0.4):
    base = mp_python.BaseOptions(model_asset_path=MODEL_PATH)
    opts = mp_vision.HandLandmarkerOptions(
        base_options=base,
        num_hands=num_hands,
        min_hand_detection_confidence=det_conf,
        min_hand_presence_confidence=det_conf,
        min_tracking_confidence=det_conf,
        running_mode=running_mode,
    )
    return mp_vision.HandLandmarker.create_from_options(opts)

def bgr_to_mp(bgr):
    return mp.Image(image_format=mp.ImageFormat.SRGB,
                    data=cv2.cvtColor(bgr, cv2.COLOR_BGR2RGB))

template_img = cv2.imread("template.png")
if template_img is None:
    print("Файл template1.png не найден")
    sys.exit(1)

det_static   = make_detector(mp_vision.RunningMode.IMAGE, num_hands=1, det_conf=0.3)
res_tmpl     = det_static.detect(bgr_to_mp(template_img))
if not res_tmpl.hand_landmarks:
    res_tmpl = det_static.detect(bgr_to_mp(cv2.flip(template_img, 1)))
det_static.close()

if not res_tmpl.hand_landmarks:
    print("ОШИБКА: рука в template1.png не найдена.")
    print("Рука должна быть чётко видна на светлом фоне.")
    sys.exit(1)

template_vec = landmarks_to_vector(res_tmpl.hand_landmarks[0])
print(f"Шаблон загружен OK, точек: {len(res_tmpl.hand_landmarks[0])}")

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Камера не найдена"); sys.exit(1)

grabbed, frame = cap.read()
fh, fw = frame.shape[:2]
print(f"Камера: {fw}x{fh}")
print("Q/Esc — выход   S — сохранить кадр")

WINDOW = "Gesture Search"
cv2.namedWindow(WINDOW, cv2.WINDOW_AUTOSIZE)
threshold_val = [70]
cv2.createTrackbar("Threshold %", WINDOW, 70, 99,
                   lambda v: threshold_val.__setitem__(0, max(v, 1)))

THUMB_H = 100
THUMB_W = int(template_img.shape[1] * THUMB_H / template_img.shape[0])
tmpl_thumb = cv2.resize(template_img, (THUMB_W, THUMB_H))

HAND_CONNECTIONS = [
    (0,1),(1,2),(2,3),(3,4),
    (0,5),(5,6),(6,7),(7,8),
    (5,9),(9,10),(10,11),(11,12),
    (9,13),(13,14),(14,15),(15,16),
    (13,17),(17,18),(18,19),(19,20),(0,17)
]

det_video    = make_detector(mp_vision.RunningMode.VIDEO, num_hands=2, det_conf=0.5)
timestamp_ms = 0

while True:
    grabbed, frame = cap.read()
    if not grabbed:
        break

    frame   = cv2.flip(frame, 1)
    display = frame.copy()
    timestamp_ms += 33

    result = det_video.detect_for_video(bgr_to_mp(frame), timestamp_ms)

    best_sim = 0.0
    best_lms = None

    if result.hand_landmarks:
        for hand_lms in result.hand_landmarks:
            pts_px = [(int(lm.x * fw), int(lm.y * fh)) for lm in hand_lms]
            for a, b in HAND_CONNECTIONS:
                cv2.line(display, pts_px[a], pts_px[b], (0, 200, 255), 2)
            for pt in pts_px:
                cv2.circle(display, pt, 4, (255, 255, 255), -1)

            sim = float(np.dot(template_vec, landmarks_to_vector(hand_lms)))
            if sim > best_sim:
                best_sim = sim
                best_lms = hand_lms

    pct   = int(max(0.0, best_sim) * 100)
    found = pct >= threshold_val[0]

    if best_lms and found:
        xs = [lm.x * fw for lm in best_lms]
        ys = [lm.y * fh for lm in best_lms]
        cv2.rectangle(display,
                      (max(0, int(min(xs))-15), max(0, int(min(ys))-15)),
                      (min(fw, int(max(xs))+15), min(fh, int(max(ys))+15)),
                      (0, 255, 0), 3)

    bx, by, bw, bh = 10, fh-50, 250, 20
    cv2.rectangle(display, (bx, by), (bx+bw, by+bh), (40,40,40), -1)
    fill = int(bw * max(0.0, best_sim))
    if fill > 0:
        cv2.rectangle(display, (bx, by), (bx+fill, by+bh),
                      (0,220,0) if found else (0,120,220), -1)
    tx = bx + int(bw * threshold_val[0] / 100)
    cv2.line(display, (tx, by-3), (tx, by+bh+3), (255,255,0), 2)
    cv2.rectangle(display, (bx, by), (bx+bw, by+bh), (200,200,200), 1)
    cv2.putText(display, f"{pct}%", (bx+bw+8, by+15),
                cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255,255,255), 2)
    cv2.putText(display,
                f"FOUND! ({pct}%)" if found else f"Searching... ({pct}%)",
                (10, fh-10), cv2.FONT_HERSHEY_SIMPLEX, 0.65,
                (0,220,0) if found else (80,80,220), 2)

    display[10:10+THUMB_H, 10:10+THUMB_W] = tmpl_thumb
    cv2.rectangle(display, (10,10), (10+THUMB_W, 10+THUMB_H),
                  (0,220,0) if found else (100,100,100), 2)
    cv2.putText(display, "template", (10, 10+THUMB_H+14),
                cv2.FONT_HERSHEY_SIMPLEX, 0.4, (200,200,200), 1)

    cv2.imshow(WINDOW, display)
    key = cv2.waitKey(1) & 0xFF
    if key in (ord('q'), ord('Q'), 27):
        break
    elif key in (ord('s'), ord('S')):
        cv2.imwrite("found_frame.png", display)
        print("Сохранено: found_frame.png")

det_video.close()
cap.release()
cv2.destroyAllWindows()