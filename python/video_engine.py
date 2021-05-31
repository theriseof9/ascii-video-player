import cv2
import render_strategy as re

class VideoEngine:
    def __init__(self, strategy="default"):
        strategy_object = re.STRATEGIES[strategy]
        self.render_strategy = strategy_object
        self.read_buffer = None
        self.with_audio = False

    def set_strategy(self, strategy):
        strategy_object = re.STRATEGIES[strategy]
        self.render_strategy = strategy_object

    def load_video_from_file(self, filename):
        cap = cv2.VideoCapture(filename)
        self.read_buffer = cap

    def play(self, file=None):
        self.render_strategy.render(self.read_buffer, 
            output=file, 
            with_audio=self.with_audio)
