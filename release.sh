#!/bin/bash

#ROOT=$(cd $(dirname $0) && pwd )
#release.sh version 8M
ROOT=.
IMAGE_DIR=$ROOT/Release
RAW=$IMAGE_DIR/raw.bin
LINUX=$IMAGE_DIR/linux.bin
UPGRADE=$IMAGE_DIR/upgrade.bin


echo "ROOT=$ROOT"
echo "RAW=$RAW"
echo "LINUX=$LINUX"
echo "UPGRADE=$UPGRADE"

# size=`ls -l $RAW | awk '{print $5}'`
# # size=2
# echo size=$size
# #1-100的求和
# sum=0
# num=0
# for ((i=0;i<$size;i++))
# do
#     num=`od -An  -j $i -N 1 -t u $RAW | awk '{tt += $1} END{print tt}'`
#     # echo num=$num
#     let sum=sum+num
# done
# echo sum=$sum

# 下面是一个示例命令，假设要从二进制文件data.bin的128字节处开始，每16位（即2个字节）为一组按照小端模式读取数据并计算总和：
# od -A d -j 128 -N $((16*2)) -t u2 --endian=little data.bin | awk '{sum += $1} END{print sum}'

# ls -l $RAW | awk '{print $5}'
# od -An  -j 3 -N 1 -t x1
# od -An -j 0 -N 8388608 -w1 -t x1  ./raw.bin | awk '{sum += $1} END{print sum}'

time=$(date "+%Y%m%d-%H%M%S")
echo $time

#size=`ls -l $RAW | awk '{print $5}'`
#echo size=$size
#sum=`od -An -j 0 -N $size -w1 -t u1  $RAW | awk '{sum += $1} END{print sum}'`
#echo sum=$sum
#printf '%x\n' $sum

VERSION=$1
SIZE=$2

NEW_RAW=$IMAGE_DIR/$VERSION-$time-$SIZE.bin
NEW_LINUX=$IMAGE_DIR/$VERSION-$time-linux.bin
NEW_UPGRADE=$IMAGE_DIR/$VERSION-$time-upgrade.bin

NEW_RAW_TXT=$IMAGE_DIR/$VERSION-$time.txt
NEW_LINUX_TXT=$IMAGE_DIR/$VERSION-$time-linux.txt
NEW_UPGRADE_TXT=$IMAGE_DIR/$VERSION-$time-upgrade.txt

MD5_FILE=$IMAGE_DIR/$VERSION-$time-md5.txt

cp $RAW $NEW_RAW -a
md5sum $NEW_RAW > $MD5_FILE
rm $RAW

cp $LINUX $NEW_LINUX -a
md5sum $NEW_LINUX >> $MD5_FILE
rm $LINUX

cp $UPGRADE $NEW_UPGRADE -a
md5sum $NEW_UPGRADE >> $MD5_FILE
rm $UPGRADE

zip -r $VERSION-$time.zip $IMAGE_DIR

