import player
import os
import re

def main():
    inp = input("Input youtube video link or local path: ")
    regex = re.compile(
        r'^(?:http|ftp)s?://'
        r'(?:(?:[A-Z0-9](?:[A-Z0-9-]{0,61}[A-Z0-9])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9-]{2,}\.?)|'
        r'localhost|'
        r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})'
        r'(?::\d+)?'
        r'(?:/?|[/?]\S+)$', re.IGNORECASE)
    if re.match(regex, inp) is not None:
        try:
            from pytube import YouTube
        except:
            print("PYTUBE NOT INSTALLED, INSTALLING NOW")
            import subprocess
            subprocess.call(['pip', 'install', 'git+https://github.com/ssuwani/pytube'])
            from pytube import Youtube
            
        yt = YouTube(inp)
        yt.streams.first().download()
        player.play(yt.streams.first().default_filename)
    else:
        player.play(inp)

if __name__ == '__main__':
    main()
