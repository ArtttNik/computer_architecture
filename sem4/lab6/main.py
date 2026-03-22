import ffmpeg
import os
import shutil

def main():
    input_file = input("Введите путь к видео: ").strip()
    if not os.path.exists(input_file):
        print("Файл не найден")
        return

    if shutil.which("ffmpeg") is None:
        print("ffmpeg не найден в PATH")
        return

    try:
        strength = int(input("Сила шумоподавления (1-10): "))
        strength = max(1, min(10, strength))
    except ValueError:
        print("Некорректное число")
        return

    base, ext = os.path.splitext(input_file)
    output_file = f"{base}_denoised.mp4"

    try:
        probe = ffmpeg.probe(input_file)
        has_audio = any(s["codec_type"] == "audio" for s in probe["streams"])
    except ffmpeg.Error as e:
        print(f"Ошибка анализа: {e.stderr.decode()}")
        return

    s = strength * 3
    p = 5
    r = 11

    stream = ffmpeg.input(input_file)
    video = stream.video.filter("nlmeans", s=s, p=p, r=r)
    #video = stream.video.filter("hqdn3d", luma_spatial=strength*2, chroma_spatial=strength, luma_temporal=3, chroma_temporal=2)
    if has_audio:
        out = ffmpeg.output(
            video, stream.audio, output_file,
            vcodec="libx264", crf=18, pix_fmt="yuv420p", acodec="copy"
        )
    else:
        out = ffmpeg.output(
            video, output_file,
            vcodec="libx264", crf=18, pix_fmt="yuv420p"
        )

    try:
        ffmpeg.run(out, overwrite_output=True)
        print(f"Готово → {output_file}")
    except ffmpeg.Error as e:
        print(f"Ошибка ffmpeg: {e.stderr.decode()}")

if __name__ == "__main__":
    main()