from abc import ABC, abstractmethod
class RenderStrategy(ABC):
    @abstractmethod
    def render(self, cap, output=None, with_audio=False):
        raise Exception("NotImplementedException")
        
