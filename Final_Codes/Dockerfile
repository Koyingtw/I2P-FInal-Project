# 選擇基礎映像
FROM ubuntu:20.04

# 設定不交互
ENV DEBIAN_FRONTEND=noninteractive

# 設定時區
ENV TZ=Asia/Taipei
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
    && apt-get update && apt-get install -y tzdata \
    && dpkg-reconfigure -f noninteractive tzdata

# 安裝必要的軟件包，包括 X11 和 Allegro 相關庫
RUN apt-get update && apt-get install -y \
    x11-apps \
    build-essential \
    pkg-config \
    pulseaudio \
    liballegro*5.2 liballegro*5-dev

# 配置 PulseAudio 虛擬音頻
RUN echo "load-module module-null-sink sink_name=virtual_speaker" >> /etc/pulse/default.pa \
    && echo "set-default-sink virtual_speaker" >> /etc/pulse/default.pa

# 複製遊戲源代碼到容器中
COPY . /usr/src/mygame

# 設置工作目錄
WORKDIR /usr/src/mygame

# 編譯遊戲
RUN make

# 添加啟動腳本
COPY start.sh /usr/src/mygame/
RUN chmod +x start.sh

RUN useradd -m myuser
USER myuser

# 使用 X11 forwarding 和啟動腳本運行遊戲
ENTRYPOINT ["./start.sh"]
