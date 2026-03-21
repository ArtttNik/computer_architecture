import ffmpeg
import os

def denoise_video(input_file, output_file, strength):
    """
    Устранение цифрового шума с видео.
    strength: 1-10, чем больше — тем агрессивнее шумоподавление
    """
    # hqdn3d — фильтр шумоподавления:
    # luma_spatial, chroma_spatial, luma_tmp, chroma_tmp
    # все параметры масштабируем от strength
    luma_s   = strength * 0.5      # пространственный шум яркости
    chroma_s = strength * 0.4      # пространственный шум цвета
    luma_t   = strength * 0.3      # временной шум яркости
    chroma_t = strength * 0.2      # временной шум цвета

    inp = ffmpeg.input(input_file)
    # video = inp.video.filter(
    #     'hqdn3d',
    #     luma_spatial=luma_s,
    #     chroma_spatial=chroma_s,
    #     luma_tmp=luma_t,
    #     chroma_tmp=chroma_t
    # )
    video = inp.video.filter('nlmeans', s=strength * 2)

    # Если есть аудио — сохраняем без изменений
    try:
        audio = inp.audio
        out = ffmpeg.output(video, audio, output_file,
                            vcodec='libx264',
                            pix_fmt='yuv420p',
                            acodec='copy')
    except Exception:
        out = ffmpeg.output(video, output_file,
                            vcodec='libx264',
                            pix_fmt='yuv420p')

    ffmpeg.run(out, overwrite_output=True)
    print(f"Готово: {output_file}")


def main():
    input_file = input("Введите путь к видеофайлу: ").strip()

    if not os.path.exists(input_file):
        print("Файл не найден")
        return

    print("\nСтепень шумоподавления:")
    print("  1-3  — слабое (едва заметное, сохраняет детали)")
    print("  4-6  — среднее (оптимально для большинства видео)")
    print("  7-10 — сильное (агрессивное, может смягчить детали)")

    try:
        strength = int(input("Введите степень (1-10): ").strip())
        strength = max(1, min(10, strength))
    except ValueError:
        print("Некорректное значение, используется 5")
        strength = 5

    base, ext = os.path.splitext(input_file)
    output_file = f"{base}_denoised_s{strength}.mp4"

    print(f"\nОбработка с силой шумоподавления {strength}/10...")
    print(f"Файл: {input_file} → {output_file}")

    denoise_video(input_file, output_file, strength)
    size_in  = os.path.getsize(input_file) / (1024 * 1024)
    size_out = os.path.getsize(output_file) / (1024 * 1024)
    print(f"Размер входного:  {size_in:.2f} МБ")
    print(f"Размер выходного: {size_out:.2f} МБ")


if __name__ == '__main__':
    main()