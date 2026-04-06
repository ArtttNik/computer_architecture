import ffmpeg

ffmpeg.input("clean.mp4").output(
    "noisy.mp4",
    vf="noise=alls=20:allf=t",
    pix_fmt="yuv420p",
    vcodec="libx264",
    crf=18
).run(overwrite_output=True)