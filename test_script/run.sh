#!/bin/sh

FS="aeon"
DEV=/dev/pmem0
MOUNT_POINT=/mnt

run () {
  sudo insmod ../$FS.ko
  sudo mount -t $FS -o init $DEV $MOUNT_POINT
}

rrun() {
  sudo umount $MOUNT_POINT
  sudo mount -t $FS $DEV $MOUNT_POINT
}

clean () {
  sudo umount $MOUNT_POINT
  sudo rmmod $FS
  #make clean
}

nvdimm_set () {
   sudo ndctl create-namespace -e "namespace0.0" -m memory -f
}

case "$1" in
  clean)
    clean
    ;;
  set)
    nvdimm_set
    ;;
  rm)
    rrun
    ;;
  *)
    run
    ;;
esac
exit 0